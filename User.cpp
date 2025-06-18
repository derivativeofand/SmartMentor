#include "User.h"
#include <iostream>

/**
 * \brief User object constructor.
 * \param name string name of the user.
 */
User::User(string name)
{
    this->name = name;
    stats.insert_or_assign("Answered", "0");
    stats.insert_or_assign("Answered Correctly", "0");
    stats.insert_or_assign("Score", "0");
    stats.insert_or_assign("Highest Streak", "0");
    stats.insert_or_assign("Level", "1");
    stats.insert_or_assign("Total exp earned", "0");
    stats.insert_or_assign("Current exp", "0");
    stats.insert_or_assign("Next exp requirement", "0");
    stats.insert_or_assign("Math questions answered correctly", "0");
    stats.insert_or_assign("Science questions answered correctly", "0");
    stats.insert_or_assign("History questions answered correctly", "0");

    level = 1;
    expThreshold = 500;
    exp = 0;
}

/**
 * \brief Getter for stats map.
 * \return stats map.
 */
map<string, string> *User::getStats()
{
    return &stats;
}

/**
 * \brief Getter for user's name.
 * \return user's name.
 */
string User::getName()
{
    return name;
}

/**
 * \brief Increments the user's experince by the ammount and levels them up if they are above the threashold for a level up.
 * \param exp experince to add to the user.
 */
void User::addEXP(double exp)
{
    this->exp += exp;
checkThreshold:
    if (this->exp >= expThreshold)
    {
        level++;
        this->exp -= expThreshold;
        expThreshold = 500 + (100 * (level - 1));
        goto checkThreshold;
    }
}
/**
 * \brief Getter for exp.
 * \return user's experince.
 */
int User::getEXP()
{
    return exp;
}

/**
 * \brief Getter for user's level.
 * \return user's level.
 */
int User::getLevel()
{
    return level;
}

/**
 * \brief Getter for the exp required for the next level.
 * \return exp threshold.
 */
int User::getEXPThreshold()
{
    return expThreshold;
}

/**
 * \brief Getter for user's metals.
 * \return vector of all the users metals.
 */
string User::getMetals()
{
    return metals;
}

/**
 * \brief Updates the user's statistics and metals. Must be run each time after changes are made to the stats map. 
 */
void User::updateStats()
{
    float answered = stoi(stats.at("Answered"));
    float correct = stoi(stats.at("Answered Correctly"));
    stats.insert_or_assign("Accuracy", to_string(correct / answered));
    stats.insert_or_assign("Level", to_string(level));
    stats.insert_or_assign("Current exp", to_string(exp));
    stats.insert_or_assign("Next exp requirement", to_string(expThreshold));

    
    if(stoi(stats.at("History questions answered correctly")) >= 100) metals += "History";
    if(stoi(stats.at("Math questions answered correctly")) >= 100) metals += "Math";
    if(stoi(stats.at("Science questions answered correctly")) >= 100) metals += "Science";
}
