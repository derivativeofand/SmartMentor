#include "Quiz.h"


/**
 * \brief Quiz object constructor.
 * \param questions vector of questions to be used in the quiz
 * \param user current user
 */
Quiz::Quiz(vector<Question *> questions, User *user)
{
    this->questions = questions;
    this->user = user;
    this->score = 0;
    this->answered = 0;
    this->answeredCorrectly = 0;
    this->streak = 0;
    this->maxStreak = 0;
    this->expGained = 0;
}

/**
 * \brief Getter for questions vector
 * \return questions vector
 */
vector<Question *> Quiz::getQuestions()
{
    return questions;
}

/**
 * \brief Submit an answer to a question in the quiz.
 * \param questionIdx index of the question being answered
 * \param answerIdx index of the given answer to the question
 * \return string to be shown to the user after answering the question. 
 */
string Quiz::submitAnswer(int questionIdx, int answerIdx)
{
    Question *q = getQuestions()[questionIdx];
    string temp = "";
    if (q->getCorrectAnswerIdx() == answerIdx)
    {
        score += q->getDifficulty();
        answeredCorrectly++;
        streak++;
        expGained += 100 + (10 * q->getDifficulty());
        maxStreak = max(streak, maxStreak);
        questionsCorrect.emplace_back(q);
        temp += "Correct!\n";
    }
    else
    {
        streak = 0;
        expGained += 25;
        temp += "Incorrect, the correct answer is: " + (q->getAnswers())[q->getCorrectAnswerIdx()] + "\n";
    }
    temp += "Score: " + to_string(score) + " Streak: " + to_string(streak);
    answered++;
    return temp;
}

/**
 * \brief Update the user object's stats map.
 */
void Quiz::updateUserStats()
{
    map<string, string> *m = user->getStats();
    int temp;
    // Score
    temp = stoi(m->at("Score"));
    temp += score;
    m->insert_or_assign("Score", to_string(temp));

    // Answered Correctly
    temp = stoi(m->at("Answered Correctly"));
    temp += answeredCorrectly;
    m->insert_or_assign("Answered Correctly", to_string(temp));

    // Answered
    temp = stoi(m->at("Answered"));
    temp += answered;
    m->insert_or_assign("Answered", to_string(temp));

    // Max Streak
    temp = stoi(m->at("Highest Streak"));
    temp = max(temp, maxStreak);
    m->insert_or_assign("Highest Streak", to_string(temp));

    // EXP
    user->addEXP(expGained);
    temp = stoi(m->at("Total exp earned"));
    m->insert_or_assign("Total exp earned", to_string(temp + expGained));

    // Metal progress
    // for (auto q = questionsCorrect.begin(); q != questionsCorrect.end(); q++)
    // {
    //     Question *question = *q;
    //     string statString = question->getCategory() + " questions answered correctly";
    //     temp = stoi(m->at(statString));
    //     m->insert_or_assign(statString, to_string(temp + 1));
    // }

    for (auto q = questionsCorrect.begin(); q != questionsCorrect.end(); q++) {
        Question *question = *q;
        string category = question->getCategory();
        string statString = category + " questions answered correctly";
        
        // Initialize if missing (though User constructor should have done this)
        if (m->count(statString) == 0) {
            (*m)[statString] = "0";
        }
        
        temp = stoi(m->at(statString));
        m->insert_or_assign(statString, to_string(temp + 1));
    }
    user->updateStats();
}
