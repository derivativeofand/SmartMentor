/**
 * \class CustomQuizSetupScreen
 * \brief Custom quiz setup screen where user can make a custom quiz to complete. 
 * \extends QuizSetupScreen
 */

 #ifndef CUSTOMQUIZSETUPSCREEN_H
 #define CUSTOMQUIZSETUPSCREEN_H
 
 #include "QuizSetupScreen.h"
 #include <QLineEdit>
 #include <QLabel>
 #include <QProgressBar>
 #include <vector>
 #include <utility>
 
class CustomQuizSetupScreen : public QuizSetupScreen
{
    Q_OBJECT
public:
    CustomQuizSetupScreen(QMainWindow *window, User *user, MyMemoryTranslator *translator);
    ~CustomQuizSetupScreen();



private slots:
    void handleTranslationFinished(const QString &translated, const QString &context);
    void retranslateUI() override;

protected:
    void UpdateQuestionsText();
    vector<vector<string>> quizQuestions;
    QLabel *questionsText;
    QLineEdit *questionAmountSelect;
    QLabel *questionAmountText;
    QPushButton *addQuestionsButton;
 
 
 };
 
 #endif
 