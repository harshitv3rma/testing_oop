// Pattern Recognition
#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // for usleep()
#include "level.cpp"

using namespace std;

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

class level2 : public level
{
private:
    player &p; // Reference to main player to track global health
    int score;

    // OOP Concept: Encapsulation / Nested Class
    class Generator
    {
    public:
        string make(int length)
        {
            string digits = "";
            for (int i = 0; i < length; i++)
            {
                digits += char('0' + rand() % 10);
            }
            return digits;
        }
    };

public:
    // Constructor takes the player object
    level2(player &current_player) : level(2, "Pattern Recognition"), p(current_player)
    {
        score = 0;
    }

    void playLevel() override
    {
        score = 0;

        display();
        cout << "Remember the digits shown one by one.\n";
        cout << "Enter 'skip' to activate cheat.\n";

        for (int round = 1; round <= 3; round++)
        {
            int length = round + 2;
            Generator generator;

            string number = generator.make(length);

            cout << "\nRound " << round << endl;
            usleep(1000000); 

            for (int i = 0; i < length; i++)
            {
                system(CLEAR_SCREEN); 

                cout << "Round " << round << endl;
                cout << "\nRemember: " << number[i] << endl;

                usleep(1000000); 
            }

            system(CLEAR_SCREEN);

            string answer;
            cout << "Enter the number you remember: ";
            
            if (!(cin >> answer)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\n[Warning] Invalid input stream! Treating as incorrect.\n";
                answer = "INVALID"; 
            }

            if (answer == "skip")
            {
                cout << "\nCheat activated!\n";
                cout << "Level 2 skipped.\n";
                return; 
            }

            if (answer == number)
            {
                cout << "Correct!\n";
                score++;
            }
            else
            {
                cout << "Wrong!\n";
                cout << "Correct number was: " << number << endl;
                
                // OOP Concept: Operator Overloading used here
                // Updated deduction to 200
                p -= 200; 
                
                cout << "Global Player Health decreased by 200!\n";
                cout << "Current Global Health: " << p.getHealth() << " / " << player::max_health << "\n";
                
                if (p.getHealth() <= 0) {
                    cout << "Your health has depleted to 0! GAME OVER.\n";
                    return; // Exits the level completely if dead
                }
                
                cout << "Moving to the next round...\n";
                usleep(1500000);
            }
        }

        cout << "\n===== LEVEL 2 COMPLETE =====\n";
        cout << "Final Score: " << score << "/3\n";

        if (score == 3)
        {
            cout << "Congratulations! You completed Level 2 flawlessly!\n";
        } 
        else 
        {
            cout << "You survived Level 2!\n";
        }
    }
};
