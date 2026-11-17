#pragma once
#include <iostream>
#include <string>
#include <limits>
using namespace std;

template <typename T>
T clampValue(T value, T low, T high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

class level;
class level1;
class Level3;
class level4;
class level5;

class player
{
private:
    string name;
    int curr_level;
    int curr_health;

public:
    inline static int max_health = 1000;

    player(string name, int curr_level)
    {
        this->name = name;
        this->curr_level = curr_level;
        this->curr_health = max_health;
    }

    string getName() const { return name; }
    int getLevel() const { return curr_level; }
    int getHealth() const { return curr_health; }
    void setLevel(int l) { curr_level = l; }

    friend class level;
    friend class level1;
    friend class Level3;
    friend class level4;
    friend class level5;
    friend player &operator-=(player &p, int damage);
    friend ostream &operator<<(ostream &out, const player &p);
    friend istream &operator>>(istream &in, player &p);
};

player &operator-=(player &p, int damage)
{
    p.curr_health = clampValue(p.curr_health - damage, 0, player::max_health);
    return p;
}

ostream &operator<<(ostream &out, const player &p)
{
    out << p.name << " | Level: " << p.curr_level << " | Health: " << p.curr_health << "/" << player::max_health;
    return out;
}

istream &operator>>(istream &in, player &p) 
{
    // Added safety check for bad input stream
    if (in >> p.name) {
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        p.name = "Hero"; // Default fallback
    }
    return in;
}
