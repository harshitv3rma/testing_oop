#pragma once // Prevents ODR violation errors
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "level.cpp"
using namespace std;

enum Element
{
    FIRE,
    WATER,
    EARTH,
    AIR,
    LIGHT,
    DARK
};

string elementName(Element e)
{
    string names[] = {"Fire", "Water", "Earth", "Air", "Light", "Dark"};
    return names[e];
}

bool beats(Element a, Element d)
{
    return (a == FIRE && d == EARTH) || (a == EARTH && d == WATER) ||
           (a == WATER && d == FIRE) || (a == LIGHT && d == DARK) ||
           (a == DARK && d == LIGHT);
}

double multiplier(Element a, Element d)
{
    if (beats(a, d))
        return 1.5;
    if (beats(d, a))
        return 0.5;
    return 1.0;
}

class Entity
{
protected:
    string name;
    Element element;
    int atk, def, maxHp, hp;

public:
    Entity(string n, Element e, int a, int d, int h)
    {
        name = n;
        element = e;
        atk = a;
        def = d;
        maxHp = h;
        hp = h;
    }

    virtual ~Entity() {}

    string getName() { return name; }
    Element getElement() { return element; }
    int getDef() { return def; }
    int getHp() { return hp; }
    int getMaxHp() { return maxHp; }
    bool isAlive() { return hp > 0; }

    void takeDamage(int d)
    {
        hp -= d;
        if (hp < 0)
            hp = 0;
    }

    void heal(int amount)
    {
        hp += amount;
        if (hp > maxHp)
            hp = maxHp;
    }

    virtual int damageTo(Entity &target)
    {
        int dmg = atk * multiplier(element, target.getElement()) - target.getDef();
        if (dmg < 1)
            dmg = 1;
        return dmg;
    }

    virtual void show()
    {
        cout << name << " (" << elementName(element) << ") HP:" << hp << "/" << maxHp;
    }
};

class Monster : public Entity
{
public:
    Monster(string n, Element e, int a, int d, int h) : Entity(n, e, a, d, h) {}
};

class Card : virtual public Entity
{
protected:
    string role;
    int healPower;

public:
    bool guarding;

    Card(string n, Element e, int a, int d, int h, string r, int hp2 = 0)
        : Entity(n, e, a, d, h)
    {
        role = r;
        healPower = hp2;
        guarding = false;
    }

    int getHealPower() { return healPower; }

    void show() override
    {
        cout << name << " [" << elementName(element) << "/" << role << "] HP:" << hp
             << "/" << maxHp << " ATK:" << atk << " DEF:" << def;
        if (healPower > 0)
            cout << " HEAL:" << healPower;
    }
};

class Support : virtual public Entity
{
protected:
    int shieldPower;

public:
    Support(string n, Element e, int a, int d, int h, int s)
        : Entity(n, e, a, d, h)
    {
        shieldPower = s;
    }

    int getShieldPower() { return shieldPower; }
};

class HealerCard : public Card
{
public:
    HealerCard(string n, Element e, int a, int d, int h, int heal)
        : Entity(n, e, a, d, h), Card(n, e, a, d, h, "Healer", heal) {}

    int damageTo(Entity &target) override
    {
        int dmg = atk * 0.5 * multiplier(element, target.getElement()) - target.getDef();
        if (dmg < 1)
            dmg = 1;
        return dmg;
    }
};

class Paladin : public Card, public Support
{
public:
    Paladin(string n, Element e, int a, int d, int h, int heal, int s)
        : Entity(n, e, a, d, h), Card(n, e, a, d, h, "Paladin", heal), Support(n, e, a, d, h, s) {}

    void show() override
    {
        Card::show();
        cout << " SHIELD:" << shieldPower;
    }
};

vector<Card *> makeDeck()
{
    vector<Card *> deck;
    deck.push_back(new Card("Blaze Warrior", FIRE, 25, 10, 100, "Attacker"));
    deck.push_back(new Card("Aqua Guardian", WATER, 12, 25, 120, "Defender"));
    deck.push_back(new Card("Stone Sentinel", EARTH, 10, 30, 130, "Defender"));
    deck.push_back(new Card("Gale Striker", AIR, 22, 12, 95, "Attacker"));
    deck.push_back(new HealerCard("Solar Priestess", LIGHT, 8, 10, 90, 20));
    deck.push_back(new Card("Shadow Assassin", DARK, 28, 8, 85, "Attacker"));
    deck.push_back(new HealerCard("Ember Sprite", FIRE, 10, 8, 80, 15));
    deck.push_back(new Card("Tidecaller", WATER, 24, 10, 100, "Attacker"));
    deck.push_back(new Card("Rock Golem", EARTH, 20, 20, 110, "Attacker"));
    deck.push_back(new HealerCard("Wind Sage", AIR, 6, 10, 85, 18));
    deck.push_back(new Paladin("Holy Paladin", LIGHT, 18, 18, 105, 12, 5));
    return deck;
}

int readInt(int low, int high)
{
    int x;
    while (!(cin >> x) || x < low || x > high)
    {
        cout << "Enter a number between " << low << " and " << high << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return x;
}

bool teamAlive(vector<Card *> &team)
{
    for (int i = 0; i < team.size(); i++)
        if (team[i]->isAlive())
            return true;
    return false;
}


class level4 : public level
{
private:
    player &p;

public:
    level4(player &current_player) : level(4, "Card Battle"), p(current_player) {}

    void playLevel() override
    {
        srand(time(0));
        vector<Card *> deck = makeDeck();
        Finally cleanup([&deck]()
                        {
            for (int i = 0; i < deck.size(); i++)
                delete deck[i]; });
        vector<Card *> team;
        
        cout << "\n\n===== LEVEL 4: Card Battle =====\n";
        cout << "Player: " << p.name << " | Health: " << p.curr_health << "/" << player::max_health << "\n\n";
        cout << "===== YOUR DECK =====\n";
        for (int i = 0; i < deck.size(); i++)
        {
            cout << i + 1 << ". ";
            deck[i]->show();
            cout << "\n";
        }

        cout << "\nPick 3 different cards by number:\n";
        while (team.size() < 3)
        {
            Card *c = deck[readInt(1, deck.size()) - 1];
            bool already = false;
            for (int i = 0; i < team.size(); i++)
                if (team[i] == c)
                    already = true;

            if (already)
                cout << "Already picked. Choose another.\n";
            else
                team.push_back(c);
        }

        cout << "\nTeam preview:\n";
        for (int i = 0; i < team.size(); i++)
        {
            Entity preview = *team[i];
            preview.show();
            cout << "\n";
        }

        Monster monsters[] = {
            Monster("Goblin Grunt", EARTH, 15, 5, 80),
            Monster("Fire Imp", FIRE, 18, 4, 70),
            Monster("Water Serpent", WATER, 16, 6, 90),
            Monster("Storm Harpy", AIR, 20, 4, 75),
            Monster("Shadow Wraith", DARK, 22, 5, 85),
            Monster("Crystal Golem", EARTH, 12, 15, 150)};
        Monster enemy = monsters[rand() % 6];

        cout << "\nA wild ";
        enemy.show();
        cout << " appears!\n";

        while (enemy.isAlive() && teamAlive(team))
        {
            cout << "\n--- Status ---\nEnemy: ";
            enemy.show();
            cout << "\n";
            for (int i = 0; i < team.size(); i++)
            {
                team[i]->guarding = false;
                cout << i + 1 << ". ";
                team[i]->show();
                if (!team[i]->isAlive())
                    cout << " (KO)";
                cout << "\n";
            }

            cout << "Choose a card to act: ";
            int pick;
            do
            {
                pick = readInt(1, 3) - 1;
            } while (!team[pick]->isAlive());
            Card *actor = team[pick];

            cout << "1. Attack\n2. Guard\n";
            int maxChoice = 2;
            if (actor->getHealPower() > 0)
            {
                cout << "3. Heal weakest ally\n";
                maxChoice = 3;
            }
            int action = readInt(1, maxChoice);

            if (action == 1)
            {
                int dmg = actor->damageTo(enemy);
                enemy.takeDamage(dmg);
                cout << actor->getName() << " hits for " << dmg << ". Enemy HP: " << enemy.getHp() << "\n";
            }
            else if (action == 2)
            {
                actor->guarding = true;
                cout << actor->getName() << " is guarding.\n";
            }
            else
            {
                Card *weakest = actor;
                for (int i = 0; i < team.size(); i++)
                    if (team[i]->isAlive() && team[i]->getHp() < weakest->getHp())
                        weakest = team[i];
                weakest->heal(actor->getHealPower());
                cout << actor->getName() << " heals " << weakest->getName() << ".\n";
            }

            if (!enemy.isAlive())
                break;

            Card *target;
            do
            {
                target = team[rand() % 3];
            } while (!target->isAlive());

            int dmg = enemy.damageTo(*target);
            if (target->guarding)
                dmg = dmg / 2;
            if (dmg < 1)
                dmg = 1;
            target->takeDamage(dmg);
            cout << enemy.getName() << " attacks " << target->getName() << " for " << dmg << ".\n";
        }

        if (enemy.isAlive()) {
            cout << "\nYour team fainted. You lost the battle.\n";
            p -= 300; 
            cout << "Global Player Health decreased by 300!\n";
        } else {
            cout << "\nYou defeated " << enemy.getName() << "! You win!\n";
        }
    }
};