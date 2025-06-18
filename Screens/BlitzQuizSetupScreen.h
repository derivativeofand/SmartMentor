/**
 * \class BlitzQuizSetupScreen
 * \brief blitz quiz setup screen where user chooses a difficulty, category and a time limit for their quiz.
 * \extends QuizSetupScreen
 */

 #ifndef BLITZQUIZSETUPSCREEN_H
 #define BLITZQUIZSETUPSCREEN_H
 
 #include <QLineEdit>
 #include "QuizSetupScreen.h"
 
 class BlitzQuizSetupScreen : public QuizSetupScreen
 {
     Q_OBJECT
 public:
     BlitzQuizSetupScreen(QMainWindow *window, User *user, MyMemoryTranslator *translator);
     ~BlitzQuizSetupScreen();
     
 protected:
     QLineEdit *timeSelect;
     QLabel *timeLabel;
     int timerPerQuestion;
     int inputStringToTimeMs(string input);
 
 private slots:
     void handleTranslationFinished(const QString &translated, const QString &context);
     void retranslateUI() override;
 };
 
 #endif
  