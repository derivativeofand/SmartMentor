/**
 * \class BlitzQuizScreen
 * \brief Blitz quiz screen where user can answer questions in a quiz and have a time limit to answer each question.
 * \extends QuizScreen
 */

#include <QTimer>
#include <iomanip>
#include <sstream>
#include <QLabel>
#include <QProgressBar>
#include "QuizScreen.h"


#ifndef BLITZQUIZSCREEN_H
#define BLITZQUIZSCREEN_H
class BlitzQuizScreen : public QuizScreen
{
    Q_OBJECT
public:
    BlitzQuizScreen(QMainWindow *window, User *user, Quiz *quiz, int timePerQuestion, MyMemoryTranslator *translator = nullptr);
    ~BlitzQuizScreen();

protected:
    void SubmitAnswer(int idx) override;
    void NextQuestion() override;

private:
    int timePerQuestion;
    bool quizComplete;
    QLabel *levelText;
    QProgressBar *expBar;
    QTimer *updateTimer;
    QTimer *questionTimer;
    QLabel *timerText;

private slots:
    void showTime();
    // void handleTranslationFinished(const QString &translated, const QString &context);
};
 
#endif