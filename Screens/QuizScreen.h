/**
 * \class QuizScreen
 * \brief Quiz screen where user can answer questions in a quiz.
 * \extends QWidget
 */

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
//#include <QTextToSpeech>
#include <QComboBox>
#include <QProgressBar>

#include "User.h"
#include "Quiz.h"
#include "translations/mymemorytranslator.h"
#ifndef QUIZSCREEN_H
#define QUIZSCREEN_H
class QuizScreen : public QWidget
{
    Q_OBJECT
public:
    QuizScreen(QMainWindow *window, User *user, Quiz *quiz, MyMemoryTranslator *translator);
    ~QuizScreen();
private:
    void retranslateUI();
    QString getTextForSpeech();

protected:
    MyMemoryTranslator *translator;
    virtual void NextQuestion();
    virtual void SubmitAnswer(int idx);
    QMainWindow *window;
    User *user;
    Quiz *quiz;
    int currentQuestion;

    // UI Elements
    QLabel *levelText;
    QProgressBar *expBar;
    QLabel *questionText;
    QPushButton *backToMenuButton;
    QPushButton *nextQuestionButton;
    QPushButton *aButton;
    QPushButton *bButton;
    QPushButton *cButton;
    QPushButton *dButton;
    //QTextToSpeech *speech;
    QPushButton *hearQuestionButton;
    QPushButton *stopHearingQuestion;
protected slots:
    void speakQuestion();
    void stopSpeaking();
    void handleTranslationFinished(const QString &translated, const QString &context);
 };

 #endif