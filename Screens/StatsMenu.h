/**
 * \class StatsMenu
 * \brief Statistics menu screen, where user can see their statistics.
 * \extends QWidget
 */

#include <QWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include "User.h"
#include "translations/mymemorytranslator.h"
#ifndef STATSMENU_H
#define STATSMENU_H
class StatsMenu : public QWidget
{
    Q_OBJECT
public:
    StatsMenu(QMainWindow *window, User *user, MyMemoryTranslator *translator);
    ~StatsMenu();
private:
    User *user;
    MyMemoryTranslator *translator;
    QLabel *levelText;
    QProgressBar *expBar;
    QLabel *statsText;
    QPushButton *backButton;
    void retranslateUI();

private slots:
    void handleTranslationFinished(const QString &result, const QString &context);
};
#endif