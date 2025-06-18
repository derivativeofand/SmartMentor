/**
 * \class QuestionGeneration
 * \brief Class to generate questions using ollama AI
 * Usew the ollama AI and the following API to prompt ollama: https://github.com/jmont-dev/ollama-hpp
 */

#include "ollama.hpp"
#include "Quiz.h"
#include <regex>
#include <set>

#ifndef QUESTIONGENERATOION_H
#define QUESTIONGENERATOION_H
class QuestionGeneration
{
public:
    QuestionGeneration();
    Quiz* generateQuiz(User *user, int difficulty, string category, int numQuestions);
    void replaceMathSymbols(string& text);
};
 
#endif