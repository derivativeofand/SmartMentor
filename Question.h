/**
 * \class Question
 * \brief Class containing question object which represents a single question in a quiz.
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <format>
#ifndef QUESTION_H
#define QUESTION_H
using namespace std;
class Question
 {
public:
    Question(string questionText, vector<string> answers, int correctAnswerIdx, int difficulty, string category);
    bool submitAnswer(int answerIdx);
    string getQuestionStr();
    vector<string> getAnswers();
    int getCorrectAnswerIdx();
    int getDifficulty();
    string getCategory();
    string toString();

private:
    string questionText;
    vector<string> answers;
    int correctAnswerIdx;
    int difficulty;
    string category;
};

#endif