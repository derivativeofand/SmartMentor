#include "mymemorytranslator.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QThread>

MyMemoryTranslator::MyMemoryTranslator(QObject *parent) : QObject(parent)
{
    
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, 
            this, &MyMemoryTranslator::onTranslationFinished,
            Qt::QueuedConnection);
}

// translate() method remains the same
void MyMemoryTranslator::translate(const QString &text, const QString &sourceLang, const QString &targetLang, const QString &context)
{
    if (text.isEmpty()) {
        emit errorOccurred("Empty text to translate");
        return;
    }
    QUrl url("https://api.mymemory.translated.net/get");
    QUrlQuery query;
    query.addQueryItem("q", text);
    query.addQueryItem("langpair", sourceLang + "|" + targetLang);
    query.addQueryItem("de", "jha58@uwo.ca");

    QNetworkRequest request(url);
    request.setUrl(QUrl(url.toString() + "?" + query.toString()));
    
    QNetworkReply *reply = m_manager->get(request);
    m_activeRequests[reply] = {text, context};
}

// Updated onTranslationFinished with better error handling
void MyMemoryTranslator::onTranslationFinished(QNetworkReply *reply)
{
    QScopedPointer<QNetworkReply> replyDeleter(reply);     

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QMetaObject::invokeMethod(this, "processFinishedTranslation", 
            Qt::QueuedConnection,
            Q_ARG(QNetworkReply*, reply));
    });
    if (reply->error() != QNetworkReply::NoError) {
        QString errorStr = QString("Network error: %1").arg(reply->errorString());
        qDebug() << errorStr;
        qDebug() << "Response:" << reply->readAll();
        emit errorOccurred(errorStr);
        return;
    }

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll(), &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        QString errorStr = QString("JSON parse error: %1").arg(parseError.errorString());
        qDebug() << errorStr;
        emit errorOccurred(errorStr);
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    
    if (!jsonObj.contains("responseData")) {
        QString errorStr = "Missing responseData in JSON";
        qDebug() << errorStr;
        emit errorOccurred(errorStr);
        return;
    }

    QJsonObject responseData = jsonObj["responseData"].toObject();
    
    if (!responseData.contains("translatedText")) {
        QString errorStr = "Missing translatedText in responseData";
        qDebug() << errorStr;
        emit errorOccurred(errorStr);
        return;
    }

    auto request = m_activeRequests.take(reply);
    QString originalText = request.first;
    QString context = request.second;
    QString translated = responseData["translatedText"].toString();

    emit translationFinished(translated, context);
}

MyMemoryTranslator::~MyMemoryTranslator() {}

void MyMemoryTranslator::setGlobalLanguage(const QString &lang)
{
    m_currentLanguage = lang;
}

QString MyMemoryTranslator::currentLanguage() {
    return m_currentLanguage;
}
