#include "BlitzQuizScreen.h"

/**
 * \brief BlitzQuizScreen constructor.
 * \param window current window.
 * \param user current user.
 * \param quiz quiz the user is taking.
 * \param timePerQuestion time (seconds) the user has to answer each question.
 */

BlitzQuizScreen::BlitzQuizScreen(QMainWindow *window, User *user, Quiz *quiz, int timePerQuestion, MyMemoryTranslator *translator)
    : QuizScreen(window, user, quiz, translator), timePerQuestion(timePerQuestion), quizComplete(false)
{
    int height = window->size().height();
    int width = window->size().width();

    // Timer setup
    updateTimer = new QTimer(this);
    updateTimer->callOnTimeout(this, &BlitzQuizScreen::showTime);

    questionTimer = new QTimer(this);
    questionTimer->setSingleShot(true);
    questionTimer->callOnTimeout(this, [this]()
                                 { 
         if (!quizComplete) {
             SubmitAnswer(-1); 
         } });

    // Timer display
    timerText = new QLabel(this);
    timerText->setAlignment(Qt::AlignHCenter);
    timerText->setGeometry(QRect(QPoint(50, 50), QSize(100, 100)));

    // Start timers
    updateTimer->start(1000);
    questionTimer->start(timePerQuestion);
    showTime();

    // Connect translation signals
    connect(translator, &MyMemoryTranslator::translationFinished,
            this, &BlitzQuizScreen::handleTranslationFinished);
}

BlitzQuizScreen::~BlitzQuizScreen()
{
    // Stop timers first to prevent any callbacks during destruction
    if (updateTimer)
        updateTimer->stop();
    if (questionTimer)
        questionTimer->stop();

    // Disconnect all signals
    if (translator)
        disconnect(translator, nullptr, this, nullptr);
    if (updateTimer)
        disconnect(updateTimer, nullptr, this, nullptr);
    if (questionTimer)
        disconnect(questionTimer, nullptr, this, nullptr);
}

void BlitzQuizScreen::SubmitAnswer(int idx)
{
    if (quizComplete)
        return;

    updateTimer->stop();
    questionTimer->stop();

    timerText->setText("");

    // Check if this was the last question using currentQuestion from QuizScreen
    if (currentQuestion >= quiz->getQuestions().size() - 1)
    {
        quizComplete = true;
        timerText->hide();
    }

    QuizScreen::SubmitAnswer(idx);
}

void BlitzQuizScreen::NextQuestion()
{
    if (!quizComplete)
    {
        updateTimer->start(1000);
        questionTimer->start(timePerQuestion);
        showTime();
    }
    QuizScreen::NextQuestion();
}

void BlitzQuizScreen::showTime()
{
    if (quizComplete)
        return;

    int time = questionTimer->remainingTime() / 1000;
    int minutes = time / 60;
    int seconds = time % 60;
    stringstream timeStream;
    timeStream << setw(2) << setfill('0') << minutes << ":"
               << setw(2) << setfill('0') << seconds;
    timerText->setText(timeStream.str().c_str());
}
