#pragma once
#include "player.cpp"
#include <iostream>
using namespace std;

template <typename F>
class Finally
{
private:
    F action;

public:
    Finally(F a) : action(a) {}
    Finally(const Finally &) = delete;
    ~Finally() { action(); }
};

class Reward
{
private:
    const int points;
    const string title;

public:
    Reward(int p, string t) : points(p), title(t) {}
    int getPoints() const { return points; }
    string getTitle() const { return title; }
    bool operator==(const Reward &other) const
    {
        return points == other.points && title == other.title;
    }
};

class level
{
protected:
    string level_name;
    int level_number;
    inline static int levels_created = 0;
    void nextLevel();
    void skipLevel();
    void resetLevel();
    static void checkStats(player p) // STATIC method
    { 
        cout << "Player's Current Health : ";
        cout << p.curr_health << endl;
    }

public:
    level() {}
    level(int level_number, string level_name)
    {
        this->level_name = level_name; // THIS keyword
        this->level_number = level_number;
        levels_created++;
    }
    virtual ~level() {}
    static int getLevelsCreated() { return levels_created; }
    void display()
    {
        cout << level_name;
        cout << endl
             << level_number;
    }
    virtual void playLevel() = 0; // for level 2
};
