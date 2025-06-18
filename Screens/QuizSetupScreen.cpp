#include "QuizSetupScreen.h"
#include "QuizScreen.h"
#include "ModeSelectScreen.h"
#include "QuestionGeneration.h"

/**
 * \brief QuizSetupScreen constructor.
 * \param window current window.
 * \param user current user.
 */
#include "QuizSetupScreen.h"
#include "QuizScreen.h"
#include "ModeSelectScreen.h"
#include "QuestionGeneration.h"

QuizSetupScreen::QuizSetupScreen(QMainWindow *window, User *user, MyMemoryTranslator *translator)
    : QWidget(window), window(window), user(user), translator(translator)
{
        int height = window->size().height();
        int width = window->size().width();

        connect(translator, &MyMemoryTranslator::translationFinished,
                this, &QuizSetupScreen::handleTranslationFinished); 

        levelText = new QLabel(this);
        levelText->setAlignment(Qt::AlignRight);
        levelText->setGeometry(QRect(QPoint(width - 440, 10), QSize(200, 25)));

        expBar = new QProgressBar(this);
        expBar->setGeometry(QRect(QPoint(width - 230, 5), QSize(200, 25)));
        expBar->setMaximum(user->getEXPThreshold());
        expBar->setValue(user->getEXP());

        // Title label
        text = new QLabel(this);
        text->setAlignment(Qt::AlignHCenter);
        text->setWordWrap(true);
        text->setGeometry(QRect(QPoint(0, 50), QSize(width, 200)));

        // Category selection
        categoryDropdown = new QComboBox(this);
        categoryDropdown->addItems({"Math", "Science", "History"});
        categoryDropdown->setGeometry(QRect(QPoint(100, 100), QSize(200, 25)));

        categoryLabel = new QLabel(this);
        categoryLabel->setAlignment(Qt::AlignRight);
        categoryLabel->setWordWrap(true);
        categoryLabel->setGeometry(QRect(QPoint(0, 100), QSize(90, 25)));

        // Difficulty selection
        difficultySlider = new QSlider(Qt::Orientation::Horizontal, this);
        difficultySlider->setMinimum(1);
        difficultySlider->setMaximum(10);
        difficultySlider->setGeometry(QRect(QPoint(100, 200), QSize(200, 25)));

        difficultyLabel = new QLabel(this);
        difficultyLabel->setAlignment(Qt::AlignRight);
        difficultyLabel->setWordWrap(true);
        difficultyLabel->setGeometry(QRect(QPoint(0, 200), QSize(90, 25)));


        // Buttons
        startButton = new QPushButton(this);
        startButton->setGeometry(QRect(QPoint((width / 2) - 50, height - 100), QSize(200, 50)));

        backButton = new QPushButton(this);
        backButton->setGeometry(QRect(QPoint(50, height - 100), QSize(200, 50)));

        // Question generation and connections
        QuestionGeneration *q = new QuestionGeneration();
        connect(startButton, &QPushButton::released, window, [this, window, user, q, translator]()
                {
        QWidget* old = window->centralWidget();
        Quiz* quiz = q->generateQuiz(user, difficultySlider->value(), 
                                   categoryDropdown->currentText().toStdString(), 5);
        window->setCentralWidget(new QuizScreen(window, user, quiz, translator));
        old->deleteLater(); });

        connect(backButton, &QPushButton::released, window, [this, window, user, translator]()
                {
        QWidget* old = window->centralWidget();
        window->setCentralWidget(new ModeSelectScreen(window, user, translator));
        old->deleteLater(); });

        retranslateUI();
}

void QuizSetupScreen::retranslateUI()
{
        QString lang = translator->currentLanguage();

        if (lang == "en")
        {
                text->setText("Select Quiz Settings");
                levelText->setText(("Level: " + to_string(user->getLevel())).c_str());
                categoryLabel->setText("Category:");
                difficultyLabel->setText("Difficulty:");
                startButton->setText("Start");
                backButton->setText("Back");
                return;
        }

        translator->translate("Select Quiz Settings", "en", lang, "setting");
        translator->translate("Category", "en", lang, "category");
        translator->translate("Difficulty", "en", lang, "difficulty");
        translator->translate("Level", "en", lang, "level");
        translator->translate("Start", "en", lang, "start");
        translator->translate("Back", "en", lang, "back");

        // Translate category items
        for (int i = 0; i < categoryDropdown->count(); i++)
        {
                QString item = categoryDropdown->itemText(i);
                if (item == "Math")
                {
                        translator->translate("Math", "en", lang, "math");
                }
                else if (item == "Science")
                {
                        translator->translate("Science", "en", lang, "science");
                }
                else if (item == "History")
                {
                        translator->translate("History", "en", lang, "history");
                }
        }
}

void QuizSetupScreen::handleTranslationFinished(const QString &translated, const QString &context)
{
        if (context == "setting")
        {
                text->setText(translated);
        }
        else if (context == "level") 
        {
                levelText->setText(translated + (" " + to_string(user->getLevel()) + ": ").c_str());
        }
        else if (context == "category")
        {
                categoryLabel->setText(translated + ": ");
        }
        else if (context == "difficulty")
        {
                difficultyLabel->setText(translated + ": ");
        }
        else if (context == "start")
        {
                startButton->setText(translated);
        }
        else if (context == "back")
        {
                backButton->setText(translated);
        }
        else if (context == "math")
        {
                int index = categoryDropdown->findText("Math");
                if (index != -1)
                {
                        categoryDropdown->setItemText(index, translated);
                }
        }
        else if (context == "science")
        {
                int index = categoryDropdown->findText("Science");
                if (index != -1)
                {
                        categoryDropdown->setItemText(index, translated);
                }
        }
        else if (context == "history")
        {
                int index = categoryDropdown->findText("History");
                if (index != -1)
                {
                        categoryDropdown->setItemText(index, translated);
                }
        }
}

QuizSetupScreen::~QuizSetupScreen()
{
        if (translator)
        {
                disconnect(translator, nullptr, this, nullptr);
        }
}
