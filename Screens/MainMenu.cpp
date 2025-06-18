#include "MainMenu.h"
#include "StatsMenu.h"
#include "QuizScreen.h"
#include "ModeSelectScreen.h"
#include <QDebug>

/**
 * \brief MainMenu constructor.
 * \param window current window.
 * \param user current user.
 */
MainMenu::MainMenu(QMainWindow *window, User *user, MyMemoryTranslator *translator)
{
        this->user = user;
        this->translator = translator;
        int height = window->size().height();
        int width = window->size().width();
        
        connect(translator, &MyMemoryTranslator::translationFinished, this, &MainMenu::handleTranslationFinished);

        // Language Selection
        languageCombo = new QComboBox(this);
        languageCombo->addItem("English", "en");
        languageCombo->addItem("French", "fr");
        languageCombo->addItem("Spanish", "es");
        languageCombo->setGeometry(QRect(QPoint(width - 280, 5), QSize(150, 30)));
        languageCombo->raise();

        applyButton = new QPushButton("Apply", this);
        applyButton->setGeometry(QRect(QPoint(width - 130, 5), QSize(100, 30)));


        connect(applyButton, &QPushButton::clicked, this, &MainMenu::retranslateUI);

        welcomeLabel = new QLabel(this);
        welcomeLabel->setAlignment(Qt::AlignHCenter);
        welcomeLabel->setWordWrap(true);
        welcomeLabel->setGeometry(QRect(QPoint(0, 50), QSize(width, 200)));

        string temp = "Welcome, " + user->getName();
        welcomeLabel->setText(temp.c_str());

        metalsText = new QLabel(this);
        metalsText->setAlignment(Qt::AlignLeft);
        metalsText->setGeometry(QRect(QPoint(10, 5), QSize(400, 25)));

        metalsText->setText(("Metals: " + user->getMetals()).c_str());

        statsButton = new QPushButton("Stats", this);
        statsButton->setGeometry(QRect(QPoint((width / 2) - 250, height - 100), QSize(200, 50)));

        quizButton = new QPushButton("Start Quiz", this);
        quizButton->setGeometry(QRect(QPoint((width / 2) + 50, height - 100), QSize(200, 50)));

        levelText = new QLabel(this);
        levelText->setAlignment(Qt::AlignRight);
        levelText->setGeometry(QRect(QPoint(width - 440, 95), QSize(200, 25)));
        levelText->setText(("Level " + to_string(user->getLevel()) + " : ").c_str());

        expBar = new QProgressBar(this);
        expBar->setGeometry(QRect(QPoint(width - 230, 90), QSize(200, 25)));
        expBar->setMaximum(user->getEXPThreshold());
        expBar->setValue(user->getEXP());

        connect(statsButton, &QPushButton::released, window, [this, window, user, translator]()
        {       
                StatsMenu* stats = new StatsMenu(window, user, translator);
                QWidget* old = window->takeCentralWidget();
                window->setCentralWidget(stats);
                if (old) {
                        old->deleteLater();
                }
        });

        connect(quizButton, &QPushButton::released, window, [window, user, translator]()
                { window->setCentralWidget(new ModeSelectScreen(window, user, translator)); });
        handle();
}

void MainMenu::retranslateUI()
{
        QString lang = languageCombo->currentData().toString();
        translator->setGlobalLanguage(lang);
        emit languageChanged(lang);

        if (lang == "en")
        {
                welcomeLabel->setText(QString("Welcome, %1").arg(QString::fromStdString(user->getName())));
                statsButton->setText("Stats");
                quizButton->setText("Start Quiz");
                applyButton->setText("Apply");
                levelText->setText(("Level " + to_string(user->getLevel()) + " : ").c_str());
                metalsText->setText("Metals: ");
                return;
        }
        
        QString welcomeText = QString("Welcome, %1").arg(QString::fromStdString(user->getName()));
        translator->translate(welcomeText, "en", lang, "welcome");
        translator->translate("Start Quiz", "en", lang, "quiz");
        translator->translate("Stats", "en", lang, "stats");
        translator->translate("Apply", "en", lang, "apply");
        translator->translate("Level", "en", lang, "level");
        translator->translate("Metals", "en", lang, "metal");
}

void MainMenu::handleTranslationFinished(const QString &translated, const QString &context)
{
        if (context == "welcome")
        {
                welcomeLabel->setText(translated);
        }
        else if (context == "stats")
        {
                statsButton->setText(translated);
        }
        else if (context == "quiz")
        {
                quizButton->setText(translated);
        }
        else if (context == "apply")
        {
                applyButton->setText(translated);
        } 
        else if (context == "level")
        {
                levelText->setText(translated + (" " + to_string(user->getLevel()) + ": ").c_str());
        }
        else if (context == "metal") 
        {
                metalsText->setText(translated + ": ");
        }
}

MainMenu::~MainMenu() {
        if (translator) {
                disconnect(translator, nullptr, this, nullptr);
        }
}

void MainMenu::handle() {
        QString lang = translator->currentLanguage();

        if (lang == "en")
        {
                welcomeLabel->setText(QString("Welcome, %1").arg(QString::fromStdString(user->getName())));
                statsButton->setText("Stats");
                quizButton->setText("Start Quiz");
                applyButton->setText("Apply");
                levelText->setText(("Level " + to_string(user->getLevel()) + " : ").c_str());
                metalsText->setText("Metals: ");
                return;
        }
        
        QString welcomeText = QString("Welcome, %1").arg(QString::fromStdString(user->getName()));
        translator->translate(welcomeText, "en", lang, "welcome");
        translator->translate("Start Quiz", "en", lang, "quiz");
        translator->translate("Stats", "en", lang, "stats");
        translator->translate("Apply", "en", lang, "apply");
        translator->translate("Level", "en", lang, "level");
        translator->translate("Metals", "en", lang, "metal");
}