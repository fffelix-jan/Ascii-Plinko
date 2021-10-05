// ASCII Plinko
// Copyright (c) 2021 Felix An

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>


using namespace std::this_thread;
using namespace std::chrono;


inline bool randomBool()
{
    static const int shift = static_cast<int>(std::log2(RAND_MAX));
    return (rand() >> shift) & 1;
}

int gamePhase(int startPos)
{
    srand(time(0));
    // "clear" the screen by printing 100 empty lines
    std::string boardPrintString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

    // Store the locations of the prizes
    const int prizes[] = { 100, 500, 1000, 0, 10000, 0, 1000, 500, 100 };

    int struck[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    int plannedPath[] = { startPos, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    bool linesThatGoLeft[12] = {};
    int lineLength;

    for (int lineGen = 0; lineGen <= 10; lineGen++) {
        if ((lineGen + 1) % 2 != 0) {   // odd numbered lines is next
            if (plannedPath[lineGen] <= 0) {
                plannedPath[lineGen + 1] = 0;
            }
            else if (plannedPath[lineGen] >= 8) {
                linesThatGoLeft[lineGen] = true;
                plannedPath[lineGen + 1] = 7;
            }
            else {
                if (randomBool()) {
                    linesThatGoLeft[lineGen] = false;
                    plannedPath[lineGen + 1] = plannedPath[lineGen];
                }
                else {
                    linesThatGoLeft[lineGen] = true;
                    plannedPath[lineGen + 1] = plannedPath[lineGen] - 1;
                }
            }
        }
        else {    // even numbered lines is next
            if (randomBool()) {
                linesThatGoLeft[lineGen] = true;
                plannedPath[lineGen + 1] = plannedPath[lineGen];
            }
            else {
                linesThatGoLeft[lineGen] = false;
                plannedPath[lineGen + 1] = plannedPath[lineGen] + 1;
            }
        }
    }

    // build the output, revealing 1 line at a time
    for (int lineNum = 0; lineNum <= 10; lineNum++) {
        boardPrintString += "-- P L I N K O --\n";
        struck[lineNum] = plannedPath[lineNum];
        // progress through building the lines
        for (int linePrint = 0; linePrint <= 10; linePrint++) {
            if (linePrint % 2 == 1) {
                lineLength = 7;
            }
            else {
                lineLength = 8;
            }
            if (lineLength == 7) {
                boardPrintString += " ";
            }
            // add each character
            for (int linePos = 0; linePos <= lineLength; linePos++) {
                if (linePos == struck[linePrint]) {
                    if (linePrint == lineNum) {
                        boardPrintString += "O";    // disk
                    }
                    else {
                        if (linesThatGoLeft[linePrint]) {
                            boardPrintString += "/";   // left arrow
                        }
                        else {
                            boardPrintString += "\\";   // right arrow
                        }
                    }


                }
                else {
                    boardPrintString += "X";   // a bullet symbol
                }
                boardPrintString += " ";
            }
            boardPrintString += "\n";
        }

        // legend and bell character to make the terminal ding
        boardPrintString += "1|5|1|0|1|0|1|5|1\n0|0|K| |0| |K|0|0\n0|0| | |K| | |0|0\u0007\n\n\n";
        std::cout << boardPrintString;
        sleep_for(seconds(1)); // delay for fun

        // "clear" the screen by printing 100 newlines
        boardPrintString = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    }
    return prizes[plannedPath[10]]; // TODO: return prize amount
}

int main(void)
{
    std::cout << "---------------- P L I N K O ----------------\nWelcome to Plinko!\n";

    // user input phase
    int dropPos = -1;
    // legend and prompt
    std::cout << "1|5|1|0|1|0|1|5|1\n0|0|K| |0| |K|0|0\n0|0| | |K| | |0|0\nWhere would you like to drop your Plinko chip?\n";
    while (true)
    {
        std::cin >> dropPos;

        while (std::cin.fail())
        {
            std::cout << "That doesn't look like a number. Try again!\n";
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cin >> dropPos;
        }

        if (dropPos >= 1 && dropPos <= 9)
        {
            break;
        }
        else
        {
            std::cout << "You must choose a number between 1-9\nwhere you would like to drop your chip. Try again!\n";
        }
    }

    // game phase
    std::cout << "Game start!\n";
    const int finalPrizePrintAmt = gamePhase(dropPos - 1);

    // reward phase
    std::cout << "\r***YOU WON $" << finalPrizePrintAmt << "!!!*** --- Game will exit in 5 seconds...";
    sleep_for(seconds(5));

    return 0;
}