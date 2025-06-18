/**
 * \brief BlitzQuizSetupScreen constructor.
 * \param window current window.
 * \param user current user.
 */
#include "BlitzQuizSetupScreen.h"
#include "BlitzQuizScreen.h"
#include "QuestionGeneration.h"

BlitzQuizSetupScreen::BlitzQuizSetupScreen(QMainWindow *window, User *user, MyMemoryTranslator *translator)
    : QuizSetupScreen(window, user, translator)
{

    timeLabel = new QLabel(this);
    timeLabel->setAlignment(Qt::AlignRight);
    timeLabel->setWordWrap(true);
    timeLabel->setGeometry(QRect(QPoint(0, 300), QSize(90, 25)));
    timeLabel->setText("Time: ");

    timeSelect = new QLineEdit(this);
    timeSelect->setInputMask("99:99");
    timeSelect->setGeometry(QRect(QPoint(100, 300), QSize(200, 25)));
    timeSelect->setText("00:10");
    QuestionGeneration *q = new QuestionGeneration();

    disconnect(startButton, &QPushButton::released, nullptr, nullptr);
    connect(startButton, &QPushButton::released, window, [this, window, user, q, translator]()
            { window->setCentralWidget(new BlitzQuizScreen(window, user, q->generateQuiz(user, difficultySlider->value(), categoryDropdown->currentText().toStdString(), 5), inputStringToTimeMs(timeSelect->text().toStdString()),translator)); });

    connect(this->translator, &MyMemoryTranslator::translationFinished,
            this, &BlitzQuizSetupScreen::handleTranslationFinished);

    // Initial translation
    retranslateUI();
}

BlitzQuizSetupScreen::~BlitzQuizSetupScreen()
{
    if (translator)
    {
        disconnect(translator, nullptr, this, nullptr);
    }
}

int BlitzQuizSetupScreen::inputStringToTimeMs(string input)
{
    int time = 0;
    string min = input.substr(0, 2);
    string sec = input.substr(3, 4);

    time += stoi(min) * 60;
    time += stoi(sec);
    time *= 1000;
    return time;
}

void BlitzQuizSetupScreen::retranslateUI()
{
    QString lang = translator->currentLanguage();
    if (lang == "en") {
        timeLabel->setText("Time: ");
        return;
    }

    translator->translate("Time", "en", lang, "time");
}

void BlitzQuizSetupScreen::handleTranslationFinished(const QString &translated, const QString &context)
{
    if (context == "time")
    {
        timeLabel->setText(translated + ": ");
    }
}
