#include <iostream>
#include <string>
#include <memory>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "Board.h"
#include "race.h"
#include "combat.h"
#include "item.h"

int main() {
    srand(time(0));  // Seed random number generator
    
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

    std::cin.ignore(10000, '\n');

    switch (choice) {
        case 1: player = std::make_shared<Human>(); break;
        case 2: player = std::make_shared<Elf>(); break;
        case 3: player = std::make_shared<Hobbit>(); break;
        case 4: player = std::make_shared<Dwarf>(); break;
    }

    std::cout << "\nYour character's stats:\n";
    player->printStats();
    std::cout << "Max Weight Capacity: " << player->getMaxWeight() << "\n\n";

    // Place player on starting square
    board.getSquare(playerX, playerY)->setPlayer(player);

    // === PLACE NPCs ON BOARD ===
    board.getSquare(5, 2)->setNpc(std::make_shared<Orc>());
    board.getSquare(width - 1, 0)->setNpc(std::make_shared<Troll>());
    board.getSquare(0, height - 1)->setNpc(std::make_shared<Goblin>());
    board.getSquare(width - 1, height - 1)->setNpc(std::make_shared<Giant>());

    // === PLACE ITEMS ON BOARD (per PDF requirements) ===
    board.getSquare(2, 1)->setItem(std::make_shared<Sword>());
    board.getSquare(7, 3)->setItem(std::make_shared<PlateArmour>());
    board.getSquare(3, 4)->setItem(std::make_shared<LargeShield>());
    board.getSquare(8, 1)->setItem(std::make_shared<RingOfLife>());
    board.getSquare(1, 5)->setItem(std::make_shared<Dagger>());
    board.getSquare(6, 2)->setItem(std::make_shared<LeatherArmour>());
    board.getSquare(4, 0)->setItem(std::make_shared<SmallShield>());
    board.getSquare(9, 4)->setItem(std::make_shared<RingOfStrength>());

    std::cout << "=== COMMANDS ===\n";
    std::cout << "Movement: n/s/e/w (north/south/east/west)\n";
    std::cout << "Combat: a (attack enemy on current square)\n";
    std::cout << "Items: p (pick up item), d <item> (drop item)\n";
    std::cout << "Info: l (look at current square), i (inventory)\n";
    std::cout << "Quit: q\n\n";

    // === MAIN GAME LOOP ===
    while (true) {
        auto square = board.getSquare(playerX, playerY);
        
        std::cout << "\n--- Current Position ---\n";
        square->printInfo();
        
        std::cout << "Player Health: " << player->getHealth()
                  << ", Gold: " << player->getGold() 
                  << ", Weight: " << player->getCurrentWeight() 
                  << "/" << player->getMaxWeight() << "\n";
        std::cout << "Time: " << (isNight ? "NIGHT" : "DAY") << "\n";

        std::cout << "\nCommand: ";
        std::getline(std::cin, command);
        if (command.empty()) continue;

        // Parse command
        std::string cmd = command.substr(0, command.find(' '));
        for (char& c : cmd) c = std::tolower(c);

        // === LOOK COMMAND ===
        if (cmd == "l" || cmd == "look") {
            square->printInfo();
            continue;
        }

        // === INVENTORY COMMAND ===
        else if (cmd == "i" || cmd == "inv" || cmd == "inventory") {
            player->printInventory();
            continue;
        }

        // === PICK UP ITEM ===
        else if (cmd == "p" || cmd == "pick" || cmd == "pickup") {
            if (square->hasItem()) {
                auto item = square->getItem();
                if (player->canPickUp(item)) {
                    std::cout << "Picking up " << item->getName() << "...\n";
                    player->pickUpItem(item);
                    square->removeItem();
                    std::cout << "Weight: " << player->getCurrentWeight() 
                              << "/" << player->getMaxWeight() << "\n";
                } else {
                    std::cout << "Cannot pick up " << item->getName() 
                              << " - too heavy! (Weight: " << item->getWeight() << ")\n";
                    std::cout << "Current: " << player->getCurrentWeight() 
                              << "/" << player->getMaxWeight() << "\n";
                }
            } else {
                std::cout << "No item here to pick up.\n";
            }
            continue;
        }

        // === DROP ITEM ===
        else if (cmd == "d" || cmd == "drop") {
            size_t pos = command.find(' ');
            if (pos != std::string::npos) {
                std::string itemName = command.substr(pos + 1);
                
                if (square->hasItem()) {
                    std::cout << "There is already an item here. Cannot drop another.\n";
                } else {
                    auto droppedItem = player->removeFromInventory(itemName);
                    if (droppedItem) {
                        square->setItem(droppedItem);
                        std::cout << "Dropped " << itemName << " on this square.\n";
                    } else {
                        std::cout << "You don't have " << itemName << ".\n";
                    }
                }
            } else {
                std::cout << "Usage: d <item name>\n";
            }
            continue;
        }

        // === ATTACK LOGIC ===
        else if (cmd == "a" || cmd == "attack") {
            if (square->getNpc()) {
                combatRound(player, square->getNpc());
                
                if (square->getNpc()->getHealth() <= 0) {
                    int reward = square->getNpc()->getDefence();
                    std::cout << "\n*** You defeated the " << square->getNpc()->getRace() << "! ***\n";
                    std::cout << "You receive " << reward << " gold pieces!\n";
                    player->addGold(reward);
                    std::cout << "Total gold: " << player->getGold() << "\n";
                    square->removeNpc();
                }

                if (player->getHealth() <= 0) {
                    std::cout << "\n*** GAME OVER ***\n";
                    std::cout << "Your character has been defeated!\n";
                    std::cout << "Final gold: " << player->getGold() << "\n";
                    break;
                }
            } else {
                std::cout << "There is no enemy to attack here.\n";
            }
            continue;
        }

        // === QUIT LOGIC ===
        else if (cmd == "q" || cmd == "quit") {
            std::cout << "Thanks for playing!\n";
            std::cout << "Final gold: " << player->getGold() << "\n";
            break;
        }

        // === MOVEMENT LOGIC ===
        bool moved = false;
        int newX = playerX, newY = playerY;

        if (cmd == "n" || cmd == "north") { newY--; moved = true; }
        else if (cmd == "s" || cmd == "south") { newY++; moved = true; }
        else if (cmd == "e" || cmd == "east") { newX++; moved = true; }
        else if (cmd == "w" || cmd == "west") { newX--; moved = true; }
        else {
            std::cout << "Invalid command.\n";
            continue;
        }

        // === PROCESS MOVEMENT ===
        if (moved) {
            auto destSquare = board.getSquare(newX, newY);
            if (destSquare) {
                board.getSquare(playerX, playerY)->removePlayer();
                playerX = newX; 
                playerY = newY;
                destSquare->setPlayer(player);
                
                moveCount++;
                if (moveCount % 5 == 0) {
                    isNight = !isNight;
                    std::cout << "\n*** Time shifts! It is now " << (isNight ? "NIGHT" : "DAY") << ". ***\n";
                }
            } else {
                std::cout << "Cannot move there - out of bounds!\n";
            }
        }
    }

    std::cout << "\nGame ended.\n";
    return 0;
}