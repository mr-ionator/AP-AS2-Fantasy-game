#include <iostream>
#include <string>
#include <memory>
#include <cctype>
#include "Board.h"
#include "race.h"
#include "combat.h"

int main() {
    int width = 10;
    int height = 6;
    Board board(width, height);

    int playerX = width / 2;
    int playerY = height / 2;

    int moveCount = 0;
    bool isNight = false;
    std::string command;

    std::shared_ptr<Character> player;
    int choice = 0;

    // === PLAYER SELECTION ===
    std::cout << "Please choose your character:\n";
    std::cout << "(1) Human\n(2) Elf\n(3) Hobbit\n(4) Dwarf\n";

    while (true) {
        std::cout << "Enter choice (1-4): ";
        std::cin >> choice;
        if (choice >= 1 && choice <= 4) break;
        std::cout << "Invalid choice, try again.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n'); // clear newline

    switch (choice) {
    case 1: player = std::make_shared<Human>();  break;
    case 2: player = std::make_shared<Elf>();    break;
    case 3: player = std::make_shared<Hobbit>(); break;
    case 4: player = std::make_shared<Dwarf>();  break;
    }

    std::cout << "\nYour character's stats:\n";
    player->printStats();

    // Place player on starting square (center)
    board.getSquare(playerX, playerY)->setPlayer(player);

    // === MANUAL NPC PLACEMENT ===
    board.getSquare(5, 2)->setNpc(std::make_shared<Human>());
    board.getSquare(width - 1, 0)->setNpc(std::make_shared<Elf>());
    board.getSquare(0, height - 1)->setNpc(std::make_shared<Hobbit>());
    board.getSquare(width - 1, height - 1)->setNpc(std::make_shared<Dwarf>());

    std::cout << "\nMove with n/s/e/w/q\n";

    // === MAIN GAME LOOP ===
    while (true)
    {
        auto square = board.getSquare(playerX, playerY);
        square->printInfo();
        std::cout << "Player Health: " << player->getHealth()
                  << ", Gold: " << player->getGold() << "\n";
        std::cout << "It is currently " << (isNight ? "NIGHT" : "DAY") << ".\n";

        if (square->getNpc()) {
            std::cout << "Available commands: n/s/e/w/q/a (a=attack)\n";
        } else {
            std::cout << "Available commands: n/s/e/w/q\n";
        }

        std::cout << "Command: ";
        std::getline(std::cin, command);
        if (command.empty()) continue;

        char c = std::tolower(command[0]);
        bool moved = false;
        int newX = playerX, newY = playerY;

        // Attack logic
        if (c == 'a' && square->getNpc()) {
            combatRound(player, square->getNpc());
            if (square->getNpc()->getHealth() <= 0) {
                int reward = square->getNpc()->getDefence(); // Gold reward is NPC's defence
                std::cout << "You defeated the " << square->getNpc()->getRace() << "!\n";
                std::cout << "You receive " << reward << " gold pieces!\n";
                player->addGold(reward);
                std::cout << "You now have " << player->getGold() << " gold.\n";
                square->removeNpc();
            }
            if (player->getHealth() <= 0) {
                std::cout << "Your character has been defeated!\n";
                std::cout << "Game ended.\n";
                std::cout << "Your gold: " << player->getGold() << "\n";
                break;
            }
            continue; // End turn after attack
        }
        // Quit logic
        else if (c == 'q') {
            break;
        }
        // Movement logic
        else if (c == 'n') { newY--; }
        else if (c == 's') { newY++; }
        else if (c == 'e') { newX++; }
        else if (c == 'w') { newX--; }
        // Invalid command
        else {
            std::cout << "Invalid command.\n";
            continue;
        }

        if ((c == 'n' || c == 's' || c == 'e' || c == 'w')) {
            auto destSquare = board.getSquare(newX, newY);
            if (destSquare) {
                board.getSquare(playerX, playerY)->removePlayer();
                playerX = newX; playerY = newY;
                destSquare->setPlayer(player);
                moved = true;
            } else {
                std::cout << "Cannot move there.\n";
            }
        }

        if (moved) {
            moveCount++;
            if (moveCount % 5 == 0) {
                isNight = !isNight;
                std::cout << "The time shifts! It is now " << (isNight ? "NIGHT" : "DAY") << ".\n";
            }
        }
    }

    std::cout << "Game ended.\n";
    return 0;
}
