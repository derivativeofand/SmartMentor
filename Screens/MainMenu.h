/**
 * \class MainMenu
 * \brief Main menu screen.
 * \extends QWidget
 */

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QComboBox>
#include "User.h"
#include "translations/mymemorytranslator.h"

#ifndef MAINMENU_H
#define MAINMENU_H

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    MainMenu(QMainWindow *window, User *user, MyMemoryTranslator *translator);
    ~MainMenu();
    void handle();

private:
    void retranslateUI();
    User *user;
    MyMemoryTranslator* translator;
    QLabel *welcomeLabel;
    QPushButton *statsButton;
    QPushButton *quizButton;
    QLabel *levelText;
    QProgressBar *expBar;
    QLabel *metalsText;
    QComboBox *languageCombo;
    QPushButton *applyButton;

private slots:
    void handleTranslationFinished(const QString &result, const QString &context);

signals:
    void languageChanged(const QString& newLanguage);
};

#endif