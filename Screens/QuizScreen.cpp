#include "QuizScreen.h"
#include "MainMenu.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QChar>

QuizScreen::QuizScreen(QMainWindow *window, User *user, Quiz *quiz,
                       MyMemoryTranslator *translator)
    : QWidget(window), window(window), user(user), quiz(quiz),
      translator(translator), currentQuestion(0)
{

    this->window = window;
    this->user = user;
    this->translator = translator;
    // speech = new QTextToSpeech(this);
    int height = window->size().height();
    int width = window->size().width();

    levelText = new QLabel(this);
    levelText->setAlignment(Qt::AlignRight);
    levelText->setGeometry(QRect(QPoint(width - 440, 10), QSize(200, 25)));
    levelText->setText(("Level " + to_string(user->getLevel()) + " : ").c_str());

    expBar = new QProgressBar(this);
    expBar->setGeometry(QRect(QPoint(width - 230, 5), QSize(200, 25)));
    expBar->setMaximum(user->getEXPThreshold());
    expBar->setValue(user->getEXP());

    // Question Display
    questionText = new QLabel(this);
    questionText->setAlignment(Qt::AlignHCenter);
    questionText->setWordWrap(true);
    questionText->setGeometry(QRect(QPoint(0, 50), QSize(width, 200)));

    // Answer Buttons
    aButton = new QPushButton("A", this);
    aButton->setGeometry(QRect(QPoint((width / 2) - 150, height - 100), QSize(50, 50)));
    connect(aButton, &QPushButton::released, this, [this]()
            { SubmitAnswer(0); });

    bButton = new QPushButton("B", this);
    bButton->setGeometry(QRect(QPoint((width / 2) - 50, height - 100), QSize(50, 50)));
    connect(bButton, &QPushButton::released, this, [this]()
            { SubmitAnswer(1); });

    cButton = new QPushButton("C", this);
    cButton->setGeometry(QRect(QPoint((width / 2) + 50, height - 100), QSize(50, 50)));
    connect(cButton, &QPushButton::released, this, [this]()
            { SubmitAnswer(2); });

    dButton = new QPushButton("D", this);
    dButton->setGeometry(QRect(QPoint((width / 2) + 150, height - 100), QSize(50, 50)));
    connect(dButton, &QPushButton::released, this, [this]()
            { SubmitAnswer(3); });

    // Navigation Button
    nextQuestionButton = new QPushButton("Next Question", this);
    nextQuestionButton->setGeometry(QRect(QPoint((width / 2) - 100, height - 100), QSize(200, 50)));
    nextQuestionButton->hide();
    connect(nextQuestionButton, &QPushButton::released, this, [this]()
            { NextQuestion(); });

    // Speech Controls
    hearQuestionButton = new QPushButton("Hear Question", this);
    hearQuestionButton->setGeometry(QRect(QPoint((width / 2) - 100, height - 250), QSize(200, 50)));
    connect(hearQuestionButton, &QPushButton::released, this, &QuizScreen::speakQuestion);

    stopHearingQuestion = new QPushButton("Stop Speaking", this);
    stopHearingQuestion->setGeometry(QRect(QPoint((width / 2) - 100, height - 200), QSize(200, 50)));
    connect(stopHearingQuestion, &QPushButton::released, this, &QuizScreen::stopSpeaking);
    NextQuestion();

    connect(translator, &MyMemoryTranslator::translationFinished,
            this, &QuizScreen::handleTranslationFinished);

    retranslateUI();
}

QuizScreen::~QuizScreen()
{
    /* if (speech) {
        speech->stop();
        speech->deleteLater();
    } */
    if (translator)
    {
        disconnect(translator, nullptr, this, nullptr);
    }
}

void QuizScreen::SubmitAnswer(int idx)
{
    QString lang = translator->currentLanguage();
    nextQuestionButton->show();
    aButton->hide();
    bButton->hide();
    cButton->hide();
    dButton->hide();
    string temp = "";
    temp += quiz->getQuestions()[currentQuestion]->getQuestionStr() + "\n";
    temp += quiz->submitAnswer(currentQuestion, idx);

    if(lang == "en") {
        questionText->setText(QString::fromStdString(temp));
    } else {
        translator->translate(QString::fromStdString(temp), "en", lang, "result");
    }
    currentQuestion++;
}

void QuizScreen::NextQuestion()
{
    nextQuestionButton->hide();
    aButton->show();
    bButton->show();
    cButton->show();
    dButton->show();
    QString lang = translator->currentLanguage();

    if (currentQuestion < quiz->getQuestions().size())
    {
        Question *q = quiz->getQuestions()[currentQuestion];
        string temp = quiz->getQuestions()[currentQuestion]->toString();
        questionText->setText(QString::fromStdString(temp));
        if (lang != "en")
        {
            // Translate question
            translator->translate(QString::fromStdString(q->getQuestionStr()), "en", lang, "question_" + QString::number(currentQuestion));

            // Translate answers
            for (size_t i = 0; i < q->getAnswers().size(); i++)
            {
                translator->translate(QString::fromStdString(q->getAnswers()[i]), "en", lang, "answer_" + QString::number(currentQuestion) + "_" + QString::number(i));
            }
        }
    }
    else
    {
        aButton->hide();
        bButton->hide();
        cButton->hide();
        dButton->hide();
        hearQuestionButton->hide();
        stopHearingQuestion->hide();

        QString complete = "Quiz Completed!";
        QString return_to_menu = "Return to Menu";

        backToMenuButton = new QPushButton("Back to menu", this);

        int height = window->size().height();
        int width = window->size().width();
        backToMenuButton->setGeometry(QRect(QPoint((width / 2) - 100, height - 100), QSize(200, 50)));
        connect(backToMenuButton, &QPushButton::released, window, [this]()
                { // Proper widget transition
                    quiz->updateUserStats();
                    MainMenu* mainMenu = new MainMenu(window, user, translator);
                    QWidget* old = window->takeCentralWidget();
                    window->setCentralWidget(mainMenu);
                    if (old) {
                        old->deleteLater();
                    } 
                });
        backToMenuButton->show();

        if (lang == "en")
        {
            questionText->setText(complete);
            nextQuestionButton->setText(return_to_menu);
        }
        else
        {
            translator->translate(complete, "en", lang, "quiz_complete");
            translator->translate(return_to_menu, "en", lang, "return_menu");
        }
    }
}

QString QuizScreen::getTextForSpeech()
{
    if (currentQuestion >= quiz->getQuestions().size())
    {
        return QString();
    }

    Question *q = quiz->getQuestions()[currentQuestion];
    QString speechText = QString::fromStdString(q->getQuestionStr()) + "? ";
    speechText += "The options are:";

    for (size_t i = 0; i < q->getAnswers().size(); i++)
    {
        speechText += QString(" %1. %2.")
                          .arg(i + 1)
                          .arg(QString::fromStdString(q->getAnswers()[i]));
    }
}

void QuizScreen::speakQuestion()
{
    /*  QString speechText = getTextForSpeech();
     if (speech) {
         speech->say(speechText);
     } */
}

void QuizScreen::stopSpeaking()
{
    /* if (speech && speech->state() == QTextToSpeech::Speaking) {
        speech->stop();
    } */
}

void QuizScreen::retranslateUI()
{
    QString lang = translator->currentLanguage();

    if (lang == "en")
    {
        levelText->setText(("Level " + to_string(user->getLevel()) + " : ").c_str());
        nextQuestionButton->setText("Next Question");
        hearQuestionButton->setText("Hear Question");
        stopHearingQuestion->setText("Stop Speaking");
        return;
    }

    // Translate UI elements
    translator->translate("Level", "en", lang, "level");
    translator->translate("Next Question", "en", lang, "next_question");
    translator->translate("Hear Question", "en", lang, "hear_question");
    translator->translate("Stop Speaking", "en", lang, "stop_speaking");
}

void QuizScreen::handleTranslationFinished(const QString &translated, const QString &context)
{
    if (context == "next_question")
    {
        nextQuestionButton->setText(translated);
    }
    else if (context == "level") 
    {
        levelText->setText(translated + (" " + to_string(user->getLevel()) + ": ").c_str());
    }
    else if (context == "hear_question")
    {
        hearQuestionButton->setText(translated);
    }
    else if (context == "stop_speaking")
    {
        stopHearingQuestion->setText(translated);
    }
    else if (context == "quiz_complete")
    {
        if (currentQuestion >= quiz->getQuestions().size())
        {
            questionText->setText(translated);
        }
    }
    else if (context.startsWith("question_"))
    {
        // Update the question text
        int questionIdx = context.mid(9).toInt();
        if (questionIdx == currentQuestion)
        {
            QString currentText = questionText->text();
            QStringList lines = currentText.split('\n');
            if (!lines.isEmpty())
            {
                lines[0] = translated; // First line is the question
                questionText->setText(lines.join('\n'));
            }
        }
    }
    else if (context.startsWith("answer_"))
    {
        // Update a specific answer
        QStringList parts = context.split('_');
        if (parts.size() == 3)
        {
            int questionIdx = parts[1].toInt();
            int answerIdx = parts[2].toInt();

            if (questionIdx == currentQuestion)
            {
                QString currentText = questionText->text();
                QStringList lines = currentText.split('\n');
                if (answerIdx + 1 < lines.size())
                { 
                    // Convert to letter (A=0, B=1, C=2, D=3)
                    QChar answerLetter = QChar('A' + answerIdx);
                    lines[answerIdx + 1] = QString("%1. %2")
                                            .arg(answerLetter)
                                            .arg(translated);
                    questionText->setText(lines.join('\n'));
                }
            }
        }
    }
    else if (context == "return_menu")
    {
        backToMenuButton->setText(translated);
    }
    else if (context == "result")
    {
        questionText->setText(translated);
    }
}

// QString QuizScreen::preserveMathSymbols(const QString &input) {
//     QString temp = input;
//     temp.replace("+", "[[PLUS]]")
//         .replace("-", "[[MINUS]]")
//         .replace("×", "[[MULTIPLY]]")
//         .replace("÷", "[[DIVIDE]]")
//         .replace("=", "[[EQUALS]]");
//     return temp;
// }

// QString QuizScreen::restoreMathSymbols(const QString& input) {
//     QString temp = input;
//     temp.replace("[[PLUS]]", "+")
//         .replace("[[MINUS]]", "-")
//         .replace("[[MULTIPLY]]", "×")
//         .replace("[[DIVIDE]]", "÷")
//         .replace("[[EQUALS]]", "=");
//     return temp;
// }


