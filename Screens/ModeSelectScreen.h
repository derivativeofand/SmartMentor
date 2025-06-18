/**
 * \class ModeSelectScreen
 * \brief Mode select screen where user selects between standard, blitz or custom quiz.
 * \extends QWidget
 */
#ifndef MODESELECTSCREEN_H
#define MODESELECTSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include "User.h"
#include "Quiz.h"
#include "translations/mymemorytranslator.h"

class ModeSelectScreen : public QWidget
{
    Q_OBJECT
public:
    ModeSelectScreen(QMainWindow *window, User *user, MyMemoryTranslator* translator);
    ~ModeSelectScreen();
    
private slots:
    void handleTranslationFinished(const QString &result, const QString &context);

private:
    void retranslateUI();

    QMainWindow *window;
    User *user;
    MyMemoryTranslator *translator;
    
    // UI Elements
    QLabel *text;
    QLabel *levelText;
    QProgressBar *expBar;
    QPushButton *standardButton;
    QPushButton *blitzButton;
    QPushButton *customButton;
    QPushButton *backButton;

};

#endif

