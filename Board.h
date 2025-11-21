#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <memory>
#include "Character.h"
#include "item.h"

class Square {
private:
    int x, y;
    std::shared_ptr<Character> npc;
    std::shared_ptr<Character> player;
    std::shared_ptr<Item> item;  // NEW: Items can be placed on squares!

public:
    Square(int x_, int y_) : x(x_), y(y_), npc(nullptr), player(nullptr), item(nullptr) {}

    void printInfo() const {
        std::cout << "You are at square (" << x << ", " << y << ")." << std::endl;
        if (player) std::cout << "Player here: " << player->getRace() << std::endl;
        if (npc) std::cout << "NPC here: " << npc->getRace() << std::endl;
        if (item) std::cout << "Item here: " << item->getName() << " [" << item->getType() << "]" << std::endl;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    
    void setPlayer(std::shared_ptr<Character> p) { player = p; }
    std::shared_ptr<Character> getPlayer() const { return player; }
    void removePlayer() { player = nullptr; }
    
    void setNpc(std::shared_ptr<Character> n) { npc = n; }
    std::shared_ptr<Character> getNpc() const { return npc; }
    void removeNpc() { npc = nullptr; }
    
    // NEW: Item management on squares
    void setItem(std::shared_ptr<Item> i) { item = i; }
    std::shared_ptr<Item> getItem() const { return item; }
    void removeItem() { item = nullptr; }
    bool hasItem() const { return item != nullptr; }
};

class Board {
private:
    int width;
    int height;
    std::vector<std::vector<std::shared_ptr<Square>>> grid;

public:
    Board(int width_, int height_) : width(width_), height(height_) {
        grid.resize(height);
        for (int y = 0; y < height; ++y) {
            grid[y].resize(width);
            for (int x = 0; x < width; ++x) {
                grid[y][x] = std::make_shared<Square>(x, y);
            }
        }
    }

    std::shared_ptr<Square> getSquare(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return nullptr;
        return grid[y][x];
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif