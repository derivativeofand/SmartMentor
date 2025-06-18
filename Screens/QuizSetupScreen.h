/**
 * \class QuizSetupScreen
 * \brief Quiz setup screen where user chooses a difficulty and category for their quiz.
 * \extends QWidget
 */

#ifndef QUIZSETUPSCREEN_H
#define QUIZSETUPSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QComboBox>
#include <QSlider>
#include <QProgressBar>

#include "User.h"
#include "Quiz.h"
#include "translations/mymemorytranslator.h"

class QuizSetupScreen : public QWidget
{
    Q_OBJECT

public:
    QuizSetupScreen(QMainWindow *window, User *user, MyMemoryTranslator* translator);
    ~QuizSetupScreen();
    virtual void retranslateUI();

private slots:
    void handleTranslationFinished(const QString &translated, const QString &context);

protected:

QMainWindow *window;
MyMemoryTranslator* translator;
User *user;

QLabel *levelText;
QProgressBar *expBar;
QComboBox *categoryDropdown;
QSlider *difficultySlider;
QPushButton *startButton;
QPushButton *backButton;
QLabel *categoryLabel;
QLabel *difficultyLabel;
QLabel *text;

};
 
 #endif
 