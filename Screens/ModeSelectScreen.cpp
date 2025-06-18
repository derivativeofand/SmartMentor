#include "ModeSelectScreen.h"
#include "MainMenu.h"
#include "QuizSetupScreen.h"
#include "BlitzQuizSetupScreen.h"
#include "CustomQuizSetupScreen.h"
#include <QDebug>
#include <QProgressBar>

ModeSelectScreen::ModeSelectScreen(QMainWindow *window, User *user, MyMemoryTranslator* translator)
    : QWidget(window), window(window), user(user), translator(translator)
{
    int height = window->size().height();
    int width = window->size().width();

    // Level and EXP display
    levelText = new QLabel(this);
    levelText->setAlignment(Qt::AlignRight);
    levelText->setGeometry(QRect(QPoint(width - 440, 10), QSize(200, 25)));
    levelText->setText(("Level " + to_string(user->getLevel()) + ": ").c_str());

    expBar = new QProgressBar(this);
    expBar->setGeometry(QRect(QPoint(width - 230, 5), QSize(200, 25)));
    expBar->setMaximum(user->getEXPThreshold());
    expBar->setValue(user->getEXP());

    // Title label
    text = new QLabel(this);
    text->setAlignment(Qt::AlignHCenter);
    text->setWordWrap(true);
    text->setGeometry(QRect(QPoint(0, 50), QSize(width, 200)));

    // Buttons
    standardButton = new QPushButton(this);
    standardButton->setGeometry(QRect(QPoint((width / 2) - 350, height - 100), QSize(200, 50)));

    blitzButton = new QPushButton(this);
    blitzButton->setGeometry(QRect(QPoint((width / 2) - 50, height - 100), QSize(200, 50)));

    customButton = new QPushButton(this);
    customButton->setGeometry(QRect(QPoint((width / 2) + 250, height - 100), QSize(200, 50)));

    backButton = new QPushButton(this);
    backButton->setGeometry(QRect(QPoint(50, height - 100), QSize(200, 50)));

    // Connections
    connect(standardButton, &QPushButton::released, window, [this, window, user, translator]() {
        QWidget* old = window->centralWidget();
        window->setCentralWidget(new QuizSetupScreen(window, user, translator));
        old->deleteLater();
    });

    connect(blitzButton, &QPushButton::released, window, [this, window, user, translator]() {
        QWidget* old = window->centralWidget();
        window->setCentralWidget(new BlitzQuizSetupScreen(window, user, translator));
        old->deleteLater();
    });

    connect(customButton, &QPushButton::released, window, [this, window, user, translator]() {
        QWidget* old = window->centralWidget();
        window->setCentralWidget(new CustomQuizSetupScreen(window, user, translator));
        old->deleteLater();
    });

    connect(backButton, &QPushButton::clicked, this, [this, window, user, translator]() {
        QWidget* old = window->centralWidget();
        window->setCentralWidget(new MainMenu(window, user, translator));
        old->deleteLater();
    });

    // Connect translator signals
    connect(this->translator, &MyMemoryTranslator::translationFinished,
        this, &ModeSelectScreen::handleTranslationFinished);

    // Initial translation
    retranslateUI();
}

void ModeSelectScreen::retranslateUI()
{
    QString lang = translator->currentLanguage();

    if (lang == "en") {
        text->setText("Mode Select");
        standardButton->setText("Standard");
        blitzButton->setText("Blitz");
        customButton->setText("Custom");
        backButton->setText("Back");
        levelText->setText(("Level " + to_string(user->getLevel()) + " : ").c_str());
        return;

    }

    translator->translate("Mode Select", "en", lang, "mode");
    translator->translate("Standard", "en", lang, "standard");
    translator->translate("Blitz", "en", lang, "blitz");
    translator->translate("Custom", "en", lang, "custom");
    translator->translate("Back", "en", lang, "back");
    translator->translate("Level", "en", lang, "level");
}

void ModeSelectScreen::handleTranslationFinished(const QString &translated, const QString &context)
{
    if (context == "mode") {
        text->setText(translated);
    } else if (context == "level") {
        levelText->setText(translated + (" " + to_string(user->getLevel()) + ": ").c_str());
    } else if (context == "standard") {
        standardButton->setText(translated);
    }
    else if (context == "blitz") {
        blitzButton->setText(translated);
    }
    else if (context == "custom") {
        customButton->setText(translated);
    }
    else if (context == "back") {
        backButton->setText(translated);
    }
}

ModeSelectScreen::~ModeSelectScreen()
{
    if (translator) {
        disconnect(translator, nullptr, this, nullptr);
    }
    disconnect(this, nullptr, nullptr, nullptr);
}
