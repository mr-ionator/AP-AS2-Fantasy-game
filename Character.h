#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <algorithm>
//#include <iostream>

class Character {
protected:
    std::string name;
    std::string race;
    int attack;
    double attackChance;
    int defence;
    double defenceChance;
    int health;
    int strength;
    int gold; // --- NEW: gold

public:
    Character(std::string raceName, int a, double ac, int d, double dc, int h, int s)
        : race(raceName), attack(a), attackChance(ac), defence(d),
        defenceChance(dc), health(h), strength(s), gold(0) // --- gold set to 0
    {}

    virtual ~Character() = default;

    virtual void printStats() const = 0;

    std::string getRace() const { return race; }
    int getAttack() const { return attack; }
    double getAttackChance() const { return attackChance; }
    int getDefence() const { return defence; }
    double getDefenceChance() const { return defenceChance; }
    int getHealth() const { return health; }
    int getStrength() const { return strength; }

    // --- Gold accessors and mutators
    int getGold() const { return gold; }
    void addGold(int amount) { gold += amount; }
    void setGold(int amount) { gold = amount; }

    // --- Health mutators (used in combat)
    void setHealth(int h) { health = std::max(0, h); }
    void takeDamage(int dmg) { health = std::max(0, health - dmg); }

    // --- Special defence hook for race logic
    virtual int specialDefence(int damage) { (void)damage; return damage; }
};

#endif
