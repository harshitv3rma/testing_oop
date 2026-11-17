#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <exception>
#include <cstdlib>
#include "level.cpp"

using namespace std;

class MazeError : public std::exception
{
public:
    virtual const char *what() const noexcept override
    {
        return "Invalid input! Please use W, A, S, or D.";
    }
};

class WallError : public MazeError
{
public:
    const char *what() const noexcept override
    {
        return "Ouch! You bumped into a wall.";
    }
};

class level5 : public level
{
private:
    player &p;
    vector<string> maze;
    int playerX;
    int playerY;
    bool gameWon;
    bool skipped; // ADDED: Tracks if the player used the skip command

    string statusMessage;
    int moves;

public:
    level5(player &current_player) : level(5, "Maze Game"), p(current_player)
    {
        maze = {
            "#################################################################################",
            "#P        #       #               # #           #       #                   #   #",
            "######### # ####### ##### ####### # # # ####### # ##### # ########### ##### ### #",
            "#       # #     #       #   #   #   # # # #   #   # #   #     #     #   #   #   #",
            "# # ### # # ### # ######### # # ##### # # # # ##### # ####### # ### ### ### # # #",
            "# # #   # # # # #   #   #   # # #     # # # #   #   #       #   #   # #   #   # #",
            "### # ### # # # ### # # # ### # # ##### # # # ### # # ####### ##### # ### ##### #",
            "#   #   # #   #     # # #   # #   #     # # #     # # #       #   #   #   #   # #",
            "# ### ### ### ####### # # # # ##### ##### # ####### # # ####### # ##### ### # # #",
            "# #   #   # #       # # # # # # #   #     # # #   #   #       # #       # # # # #",
            "# ##### ### # ####### # # # # # # ### ### # # # # ########### # ######### # # # #",
            "#       #     #     # # # #     # # # # #     # # #           # #     #     # # #",
            "# ####### ##### ### # # # ####### # # # ####### # # ########### # ##### ##### # #",
            "#       # #     # #   # #     #   #     #     # # #   # #       #     # #   #   #",
            "####### ### ##### ##### ##### # ######### ### # # ### # # ####### ### # # #######",
            "#     #   # #     #   #     # #     #       #   #     # #       # # #   #       #",
            "# ### ### # # ### ### ##### ##### # # ############### # ####### # # ##### ##### #",
            "#   #   # # # #           #     # #   #   #   #     #     #   # #       # #   # #",
            "# ### # # # # ########### ##### ####### # # # # ### ####### # # ####### ### # # #",
            "# #   # #   #       #     # #   #       # # #   # # #       #   #     #   # #   #",
            "# # ############# # # ##### # ### ####### # ##### # # ########### # ##### # ### #",
            "# #             # # # #     # #       # #   #       # #       #   # #     # # # #",
            "# ####### ##### ### # ### # # ### ### # ##### ####### ####### # # ### ##### # # #",
            "# #     # #   #     #   # # #   #   # #     #       # #     # # #     #     #   #",
            "# # ### # # # ######### ### ### ##### # # ######### # # ### # # ####### # ##### #",
            "# # #     # #         #     # #   #   # #     #   # #     # # #       # #     # #",
            "# # ############# ### ##### # ### # ####### # ### # ####### # ####### # ##### ###",
            "# #             #   #     # #   #   #       #   # # #       # #     # #     #   #",
            "# ############# ### ##### # ### ##### # ####### # # # ####### # ### # # ####### #",
            "# # #         # #     #   # #     # # #       #   #   #     #   #   # # #     # #",
            "# # # # ##### # ####### # # # ### # # ####### ######### # ### ### ### # # ### # #",
            "# #   #   #   # #       # #   #     # #     # #         # #     # #   # # #   # #",
            "# ##### # ##### # ######### ####### # # ##### # ######### # ##### # ##### # ### #",
            "# #   # # #     #           #   #   # #   #   # #       #   #   # # #   # # #   #",
            "# # # ### # ############### # # ### # # # # ### # ### # ##### # # # # # # # # # #",
            "# # #     #   #   #   #   # # #   # # # # #     # #   #   #   #   # # #   #   # #",
            "# # ##### ### # # # # # # # # ### # # ### ######### ##### # # ##### # # ####### #",
            "# #     #   #   #   # # # # #   # # #     #       #     # # # #     # # #     # #",
            "# # ### ### ######### # # # ### # ####### ### ### ##### # # # # ##### ### ### ###",
            "# #   #   #   #     #   # # #   #       #   # # #       # # # # #   #   #   #   #",
            "# ### ### ### ##### ##### ### ####### ##### # # ######### # ### ### # # ### ### #",
            "# #   # #   #           # #   #     #     # # #         # #   #   #   #     #   #",
            "# ### # ### ########### # # ### # ####### # # ####### ### ### ### # ######### # #",
            "#   # #   #   #   #     # #   # # #       # #       #       #   # #     #   # # #",
            "### # # # ### # ### ##### ### # # # ##### # ### ### # ######### # ####### # # # #",
            "#   #   #   # #     # #     #   # # #   # #   #   # #   #       #       # #   # #",
            "# ########### # ##### # ### ##### # # # ##### ##### ### # # ########### # ##### #",
            "#   #   #     #   #   #   #     # # # #     # #     #   # # #     #     #   #   #",
            "### # # # ####### ### ### ####### # # ### ### # ##### ### ### ### # ####### # ###",
            "#     #   #             #         #     #       #       #       #           #  E#",
            "#################################################################################"};
        playerX = 1;
        playerY = 1;
        gameWon = false;
        skipped = false; // Initialize the skip flag
        statusMessage = "";
        moves = 0;
    }

    void draw()
    {
        cout << "\033[2J\033[1;1H";

        cout << "========== LEVEL 5 ==========\n";
        cout << "          MAZE GAME          \n";
        cout << "Player: " << p.name << " | Health: " << p.curr_health << "/" << player::max_health << " | Moves: " << moves << "\n";
        cout << "-----------------------------\n";

        for (const string &row : maze)
        {
            for (char c : row)
            {
                if (c == '#')
                {
                    cout << "█";
                }
                else
                {
                    cout << c;
                }
            }
            cout << "\n";
        }

        if (!statusMessage.empty())
        {
            cout << "\n[!] " << statusMessage << "\n";
            statusMessage = ""; 
        }

        if (!gameWon && !skipped)
            // ADDED: Prompt the user with the skip option
            cout << "\nMove (W/A/S/D) or type 'skip' (-300 HP), and press Enter: ";
    }

    void movePlayer(int dx, int dy)
    {
        int nextX = playerX + dx;
        int nextY = playerY + dy;

        if (maze[nextY][nextX] == 'E')
        {
            gameWon = true;
            maze[playerY][playerX] = ' ';
            maze[nextY][nextX] = 'P';
        }
        else if (maze[nextY][nextX] == '#')
        {
            throw WallError();
        }
        else
        {
            maze[playerY][playerX] = ' ';
            playerX = nextX;
            playerY = nextY;
            maze[playerY][playerX] = 'P';
        }
    }

    void processInput()
    {
        string inStr;
        if (!(cin >> inStr)) {
            cin.clear();
            cin.ignore(10000, '\n');
            statusMessage = "Invalid input stream detected!";
            return;
        }

        // Convert input to lowercase to reliably check for "skip"
        string lowerInput = inStr;
        for (char &c : lowerInput) {
            c = tolower(c);
        }

        // ADDED: Logic to handle the skip command
        if (lowerInput == "skip") {
            skipped = true;
            return;
        }

        char input = lowerInput[0];

        Finally counter([this]()
                        { moves++; });

        try
        {
            if (input == 'w')
                movePlayer(0, -1);
            else if (input == 's')
                movePlayer(0, 1);
            else if (input == 'a')
                movePlayer(-1, 0);
            else if (input == 'd')
                movePlayer(1, 0);
            else
            {
                throw MazeError();
            }
        }
        catch (const WallError &e)
        {
            statusMessage = e.what();
        }
        catch (const MazeError &e)
        {
            statusMessage = e.what();
        }
    }

    bool isOver()
    {
        // ADDED: The level is over if the player either wins or skips
        return gameWon || skipped;
    }

    void playLevel() override
    {
        level5 *ptr = this;

        while (!ptr->isOver())
        {
            ptr->draw();
            ptr->processInput();

            // ADDED: Deduct health and exit immediately if skip was activated
            if (ptr->skipped) {
                cout << "\n[SKIP ACTIVATED] You chose to bypass the maze!\n";
                
                p -= 300; 
                
                cout << "Global Player Health decreased by 300!\n";
                cout << "Current Global Health: " << p.curr_health << " / " << player::max_health << "\n";
                return; 
            }
        }

        // Only prints if the player successfully finishes the maze
        if (ptr->gameWon) {
            ptr->draw();
            cout << "\nCongratulations, " << p.name << "! You escaped the maze!\n";
        }
    }
};
