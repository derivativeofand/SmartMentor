#ifndef MYMEMORYTRANSLATOR_H  
#define MYMEMORYTRANSLATOR_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>
#include <QMap>

class MyMemoryTranslator : public QObject
{
    Q_OBJECT
public:
    explicit MyMemoryTranslator(QObject *parent = nullptr);
    ~MyMemoryTranslator();
    void translate(const QString &text, const QString &sourceLang, const QString &targetLang, const QString &context);
    void setGlobalLanguage(const QString &lang);
    QString currentLanguage();
    
    signals:
        void translationFinished(const QString &translatedText, const QString &context);
        void errorOccurred(const QString &errorMessage);
        
private slots:
    void onTranslationFinished(QNetworkReply *reply);

private:
    QMap<QNetworkReply*, QPair<QString, QString>> m_activeRequests;
    QNetworkAccessManager *m_manager;
    QString m_currentLanguage = "en"; 
};

#endif