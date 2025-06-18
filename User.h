/**
 * \class User
 * \brief Class containing user object which holds a user's statistics. 
 */

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#ifndef USER_H
#define USER_H
using namespace std;
class User
{
public:
    User(string name);
    map<string, string> *getStats();
    string getName();
    void addEXP(double exp);
    int getEXP();
    int getLevel();
    int getEXPThreshold();
    string getMetals();
    void updateStats();
private:
    map<string, string> stats;
    string metals;
    string name;
    int exp;
    int expThreshold;
    int level;
};

#endif