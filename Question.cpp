#include "Question.h"


/**
 * \brief Question object constructor.
 * \param questionText string representing just the question being asked.
 * \param answers vector of all the asnwers to the MCQ.
 * \param correctAnswerIdx index of the correct asnwer in the asnwers vector.
 * \param category subject of the question.
 */
Question::Question(string questionText, vector<string> answers, int correctAnswerIdx, int difficulty, string category)
{
    this->questionText = questionText;
    this->answers = answers;
    this->correctAnswerIdx = correctAnswerIdx;
    this->difficulty = difficulty;
    this->category = category;
}

/**
 * \brief Checks if given answer indes is correct.
 * \param answerIdx asnwer inxed.
 * \return true if the correct asnwer is at the given index false otherwise.
 */
bool Question::submitAnswer(int answerIdx)
{
    return correctAnswerIdx == answerIdx;
}


/**
 * \brief Getter for question text.
 * \return string question text.
 */
string Question::getQuestionStr()
{
    return questionText;
}

/**
 * \brief Getter for answers vector.
 * \return answers vector.
 */
vector<string> Question::getAnswers()
{
    return answers;
}


/**
 * \brief Getter for correct answer index.
 * \return correct answer index.
 */
int Question::getCorrectAnswerIdx()
{
    return correctAnswerIdx;
}

/**
 * \brief Getter for difficulty.
 * \return difficulty.
 */
int Question::getDifficulty()
{
    return difficulty;
}

/**
 * \brief Getter for question subject.
 * \return subject.
 */
string Question::getCategory()
{
    return category;
}

/**
 * \return Question in the form of a string.
 */
string Question::toString()
{
    string s = questionText + "\n";
    for (int i = 0; i < answers.size(); i++)
    {
        s += "\t" + string(1, 'A' + i) + ". " + answers[i] + "\n";
    }
    return s;
}
