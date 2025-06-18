#include "CustomQuizSetupScreen.h"
#include "QuizScreen.h"
#include "QuestionGeneration.h"

/**
 * \brief CustomQuizSetupScreen constructor.
 * \param window current window.
 * \param user current user.
 */
#include "CustomQuizSetupScreen.h"
#include "QuizScreen.h"
#include "QuestionGeneration.h"

/**
 * \brief CustomQuizSetupScreen constructor.
 * \param window current window.
 * \param user current user.
 */
CustomQuizSetupScreen::CustomQuizSetupScreen(QMainWindow *window, User *user, MyMemoryTranslator *translator) : QuizSetupScreen(window, user, translator)
{
    int height = window->size().height();
    int width = window->size().width();

    connect(this->translator, &MyMemoryTranslator::translationFinished,
        this, &CustomQuizSetupScreen::handleTranslationFinished);

    questionAmountText = new QLabel(this);
    questionAmountText->setAlignment(Qt::AlignRight);
    questionAmountText->setWordWrap(true);
    questionAmountText->setGeometry(QRect(QPoint(0, 300), QSize(90, 25)));
    questionAmountText->setText("Questions:");

    questionAmountSelect = new QLineEdit(this);
    questionAmountSelect->setInputMask("009");
    questionAmountSelect->setGeometry(QRect(QPoint(100, 300), QSize(200, 25)));
    questionAmountSelect->setText("1");

    questionsText = new QLabel(this);
    questionsText->setAlignment(Qt::AlignLeft);
    questionsText->setWordWrap(true);
    questionsText->setGeometry(QRect(QPoint(400, 100), QSize(width - 300, height - 400)));
    questionsText->setText("Custom Quiz:\n");

    addQuestionsButton = new QPushButton("Add Question", this);
    addQuestionsButton->setGeometry(QRect(QPoint(100, 500), QSize(200, 50)));

    QuestionGeneration *q = new QuestionGeneration();
    disconnect(startButton, &QPushButton::released, nullptr, nullptr);
    connect(startButton, &QPushButton::released, window, [this, window, user, q, translator]()
            { 
                vector<Question*> questions;
                for (vector<string> quest : quizQuestions)
                {
                    Quiz *temp = q->generateQuiz(user,stoi(quest[1]),quest[0],stoi(quest[2]));
                    for(Question *quizQuest : temp->getQuestions()) {
                        questions.push_back(quizQuest);
                    }
                }
                window->setCentralWidget(new QuizScreen(window, user, new Quiz(questions,user),translator)); });

    connect(addQuestionsButton, &QPushButton::released, this, [this]() {
            quizQuestions.push_back({
            categoryDropdown->currentText().toStdString(), 
            to_string(difficultySlider->value()), 
            questionAmountSelect->text().toStdString()}); 
            UpdateQuestionsText();});

    retranslateUI();
}

/**
 * \brief updates the questions textbox.
 */
void CustomQuizSetupScreen::UpdateQuestionsText()
{
    QString lang = translator->currentLanguage();
    string s = "Custom Quiz:\n";
    for (vector<string> q : quizQuestions)
    {
        s += q[2] + " Question(s) |    " + "Category: " + q[0] + "   Difficulty: " + q[1] + "\n";
    }
    
    if (lang == "en") {
        questionsText->setText(QString::fromStdString(s));
        return;
    }
    
    translator->translate("Custom Quiz:", "en", lang, "custom_quiz_header");
    
    translator->translate(QString::fromStdString(s), "en", lang, "custom_quiz_content");
}

void CustomQuizSetupScreen::retranslateUI()
{
    QString lang = translator->currentLanguage();

    if (lang == "en")
    {
        questionAmountText->setText("Questions:");
        addQuestionsButton->setText("Add Question");
        questionsText->setText("Custom Quiz:\n");
        return;
    }

    translator->translate("Questions:", "en", lang, "question_text");
    translator->translate("Add Question", "en", lang, "add_question");
    translator->translate("Custom Quiz:", "en", lang, "custom_quiz_header");
}

void CustomQuizSetupScreen::handleTranslationFinished(const QString &translated, const QString &context)
{
    if (context == "question_text")
    {
        questionAmountText->setText(translated);
    }
    else if (context == "custom_quiz_header")
    {
        // Update just the header part
        QString currentText = questionsText->text();
        QStringList parts = currentText.split('\n');
        if (parts.size() > 0) {
            parts[0] = translated;
            questionsText->setText(parts.join('\n'));
        }
    }
    else if (context == "custom_quiz_content")
    {
        questionsText->setText(translated);
    }
    else if (context == "add_question")
    {
        addQuestionsButton->setText(translated);
    }
}

CustomQuizSetupScreen::~CustomQuizSetupScreen()
{
    if (translator)
    {
        disconnect(translator, nullptr, this, nullptr);
    }
}
