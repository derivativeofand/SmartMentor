/**
 * \class Quiz
 * \brief Class containing quiz object which represents a collection of question which can be answered 
 */

#include "Question.h"
#include "User.h"
#ifndef QUIZ_H
#define QUIZ_H
class Quiz
{
    public:
        Quiz(vector<Question*> questions, User *user);
        vector<Question*>  getQuestions();
        string submitAnswer(int questionIdx, int answerIdx);
        void updateUserStats();

    protected:
        vector<Question*> questions;
        vector<Question*> questionsCorrect;
        User *user;
        int score;
        int answered;
        int answeredCorrectly;
        int streak; 
        int maxStreak;
        int expGained;
};
#endif