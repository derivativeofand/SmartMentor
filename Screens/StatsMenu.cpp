#include "StatsMenu.h"
#include "MainMenu.h"

/**
 * \brief StatsMenu constructor.
 * \param window current window.
 * \param user current user.
 */
StatsMenu::StatsMenu(QMainWindow *window, User *user, MyMemoryTranslator *translator)
{
    this->user = user;
    this->translator = translator;
    int height = window->size().height();
    int width = window->size().width();

    connect(translator, &MyMemoryTranslator::translationFinished,
        this, &StatsMenu::handleTranslationFinished); 

    levelText = new QLabel(this);
    levelText->setAlignment(Qt::AlignRight);
    levelText->setGeometry(QRect(QPoint(width - 440, 10), QSize(200, 25)));
    levelText->setText(("Level: " + to_string(user->getLevel())).c_str());

    expBar = new QProgressBar(this);
    expBar->setGeometry(QRect(QPoint(width - 230, 5), QSize(200, 25)));
    expBar->setMaximum(user->getEXPThreshold());
    expBar->setValue(user->getEXP());

    statsText = new QLabel(this);
    statsText->setAlignment(Qt::AlignHCenter);
    statsText->setWordWrap(true);
    statsText->setGeometry(QRect(QPoint(0, 50), QSize(width, 200)));
    
    backButton = new QPushButton("Back", this);
    backButton->setGeometry(QRect(QPoint(50, height - 100), QSize(200, 50)));

    retranslateUI();

    connect(backButton, &QPushButton::released, window, [window, user, translator]()
        { 
            MainMenu* mainMenu = new MainMenu(window, user, translator);
            // Replace the current widget
            QWidget* old = window->takeCentralWidget();
            window->setCentralWidget(mainMenu);
            if (old) old->deleteLater(); 
        });
}


void StatsMenu::retranslateUI() {
    QString lang = translator->currentLanguage();

    string statsString = "Stats for user, " + user->getName() + ":\n";
    for (map<string, string>::iterator it = user->getStats()->begin();
         it != user->getStats()->end(); ++it) {
        statsString += it->first;
        statsString += ": ";
        statsString += it->second;
        statsString += "\n";
    }

    // Translate static UI elements
    if(lang == "en") {
        statsText->setText(QString::fromStdString(statsString));
        backButton->setText("Back");
        levelText->setText(("Level: " + to_string(user->getLevel())).c_str());
        return;
    }

    translator->translate(QString::fromStdString(statsString), "en", lang, "stats_text");
    translator->translate("Back", "en", lang, "back_button");
    translator->translate("Level", "en", lang, "level");
}

void StatsMenu::handleTranslationFinished(const QString &translated, const QString &context) {\
    
    if (context == "stats_text")
    {
        statsText->setText(translated);
    }
        else if (context == "level") 
    {
        levelText->setText(translated + (" " + to_string(user->getLevel()) + ": ").c_str());
    }
        else if (context == "back_button")
    {
        backButton->setText(translated);
    }
}

StatsMenu::~StatsMenu() {
    if (translator) {
        disconnect(translator, nullptr, this, nullptr);
    }
}
