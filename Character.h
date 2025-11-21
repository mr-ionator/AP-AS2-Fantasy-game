#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
#include "item.h"

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
    int gold;
    
    // --- INVENTORY SYSTEM (per PDF requirements) ---
    std::vector<std::shared_ptr<Item>> inventory;
    std::shared_ptr<Item> equippedWeapon;
    std::shared_ptr<Item> equippedArmour;
    std::shared_ptr<Item> equippedShield;
    std::vector<std::shared_ptr<Item>> equippedRings;  // Multiple rings allowed!

public:
    Character(std::string raceName, int a, double ac, int d, double dc, int h, int s)
        : race(raceName), attack(a), attackChance(ac), defence(d),
          defenceChance(dc), health(h), strength(s), gold(0),
          equippedWeapon(nullptr), equippedArmour(nullptr), 
          equippedShield(nullptr)
    {}

    virtual ~Character() = default;
    virtual void printStats() const = 0;

    // --- BASE STAT GETTERS ---
    std::string getRace() const { return race; }
    int getBaseAttack() const { return attack; }
    int getBaseDefence() const { return defence; }
    int getBaseHealth() const { return health; }
    int getBaseStrength() const { return strength; }
    
    double getAttackChance() const { return attackChance; }
    double getDefenceChance() const { return defenceChance; }

    // --- MODIFIED STAT GETTERS (with item bonuses) ---
    int getAttack() const {
        int bonus = 0;
        if (equippedWeapon) bonus += equippedWeapon->getAttackModifier();
        if (equippedArmour) bonus += equippedArmour->getAttackModifier();
        if (equippedShield) bonus += equippedShield->getAttackModifier();
        for (const auto& ring : equippedRings) {
            bonus += ring->getAttackModifier();
        }
        return std::max(0, attack + bonus);
    }

    int getDefence() const {
        int bonus = 0;
        if (equippedWeapon) bonus += equippedWeapon->getDefenceModifier();
        if (equippedArmour) bonus += equippedArmour->getDefenceModifier();
        if (equippedShield) bonus += equippedShield->getDefenceModifier();
        for (const auto& ring : equippedRings) {
            bonus += ring->getDefenceModifier();
        }
        return std::max(0, defence + bonus);
    }

    int getHealth() const {
        int bonus = 0;
        if (equippedWeapon) bonus += equippedWeapon->getHealthModifier();
        if (equippedArmour) bonus += equippedArmour->getHealthModifier();
        if (equippedShield) bonus += equippedShield->getHealthModifier();
        for (const auto& ring : equippedRings) {
            bonus += ring->getHealthModifier();
        }
        return std::max(0, health + bonus);
    }

    int getStrength() const {
        int bonus = 0;
        if (equippedWeapon) bonus += equippedWeapon->getStrengthModifier();
        if (equippedArmour) bonus += equippedArmour->getStrengthModifier();
        if (equippedShield) bonus += equippedShield->getStrengthModifier();
        for (const auto& ring : equippedRings) {
            bonus += ring->getStrengthModifier();
        }
        return std::max(0, strength + bonus);
    }

    // --- GOLD ACCESSORS ---
    int getGold() const { return gold; }
    void addGold(int amount) { gold += amount; }
    void setGold(int amount) { gold = amount; }

    // --- HEALTH MUTATORS ---
    void setHealth(int h) { health = std::max(0, h); }
    void takeDamage(int dmg) { 
        health = std::max(0, health - dmg);
    }

    // --- WEIGHT CALCULATION (based on Strength) ---
    int getMaxWeight() const {
        return getStrength();  // Max weight = character's strength
    }

    int getCurrentWeight() const {
        int total = 0;
        for (const auto& item : inventory) {
            total += item->getWeight();
        }
        if (equippedWeapon) total += equippedWeapon->getWeight();
        if (equippedArmour) total += equippedArmour->getWeight();
        if (equippedShield) total += equippedShield->getWeight();
        for (const auto& ring : equippedRings) {
            total += ring->getWeight();
        }
        return total;
    }

    // --- INVENTORY MANAGEMENT (per PDF: pick up items from squares) ---
    bool canPickUp(std::shared_ptr<Item> item) const {
        return (getCurrentWeight() + item->getWeight() <= getMaxWeight());
    }

    bool pickUpItem(std::shared_ptr<Item> item) {
        if (canPickUp(item)) {
            inventory.push_back(item);
            
            // Auto-equip if slot is empty (per PDF requirements)
            std::string type = item->getType();
            if (type == "Weapon" && !equippedWeapon) {
                equippedWeapon = item;
                inventory.pop_back();
                std::cout << "Auto-equipped " << item->getName() << " as weapon.\n";
            } else if (type == "Armour" && !equippedArmour) {
                equippedArmour = item;
                inventory.pop_back();
                std::cout << "Auto-equipped " << item->getName() << " as armour.\n";
            } else if (type == "Shield" && !equippedShield) {
                equippedShield = item;
                inventory.pop_back();
                std::cout << "Auto-equipped " << item->getName() << " as shield.\n";
            } else if (type == "Ring") {
                equippedRings.push_back(item);
                inventory.pop_back();
                std::cout << "Auto-equipped " << item->getName() << " as ring.\n";
            } else {
                std::cout << "Added " << item->getName() << " to inventory.\n";
            }
            return true;
        }
        return false;
    }

    bool dropItem(const std::string& itemName) {
        // Check inventory
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if ((*it)->getName() == itemName) {
                inventory.erase(it);
                return true;
            }
        }
        
        // Check equipped items
        if (equippedWeapon && equippedWeapon->getName() == itemName) {
            equippedWeapon = nullptr;
            return true;
        }
        if (equippedArmour && equippedArmour->getName() == itemName) {
            equippedArmour = nullptr;
            return true;
        }
        if (equippedShield && equippedShield->getName() == itemName) {
            equippedShield = nullptr;
            return true;
        }
        for (auto it = equippedRings.begin(); it != equippedRings.end(); ++it) {
            if ((*it)->getName() == itemName) {
                equippedRings.erase(it);
                return true;
            }
        }
        
        return false;
    }

    std::shared_ptr<Item> removeFromInventory(const std::string& itemName) {
        // Remove from inventory
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if ((*it)->getName() == itemName) {
                auto item = *it;
                inventory.erase(it);
                return item;
            }
        }
        
        // Remove from equipped
        if (equippedWeapon && equippedWeapon->getName() == itemName) {
            auto item = equippedWeapon;
            equippedWeapon = nullptr;
            return item;
        }
        if (equippedArmour && equippedArmour->getName() == itemName) {
            auto item = equippedArmour;
            equippedArmour = nullptr;
            return item;
        }
        if (equippedShield && equippedShield->getName() == itemName) {
            auto item = equippedShield;
            equippedShield = nullptr;
            return item;
        }
        for (auto it = equippedRings.begin(); it != equippedRings.end(); ++it) {
            if ((*it)->getName() == itemName) {
                auto item = *it;
                equippedRings.erase(it);
                return item;
            }
        }
        
        return nullptr;
    }

    // --- DISPLAY FUNCTIONS ---
    void printInventory() const {
        std::cout << "\n=== INVENTORY ===\n";
        std::cout << "Weight: " << getCurrentWeight() << "/" << getMaxWeight() << "\n";
        
        std::cout << "\nEquipped:\n";
        if (equippedWeapon) {
            std::cout << "  Weapon: " << equippedWeapon->getName() 
                      << " (Atk+" << equippedWeapon->getAttackModifier() << ")\n";
        } else {
            std::cout << "  Weapon: None\n";
        }
        
        if (equippedArmour) {
            std::cout << "  Armour: " << equippedArmour->getName() 
                      << " (Def+" << equippedArmour->getDefenceModifier() << ")\n";
        } else {
            std::cout << "  Armour: None\n";
        }
        
        if (equippedShield) {
            std::cout << "  Shield: " << equippedShield->getName() 
                      << " (Def+" << equippedShield->getDefenceModifier() << ")\n";
        } else {
            std::cout << "  Shield: None\n";
        }
        
        if (!equippedRings.empty()) {
            std::cout << "  Rings:\n";
            for (const auto& ring : equippedRings) {
                std::cout << "    - " << ring->getName() 
                          << " (HP+" << ring->getHealthModifier() 
                          << ", Str+" << ring->getStrengthModifier() << ")\n";
            }
        } else {
            std::cout << "  Rings: None\n";
        }
        
        std::cout << "\nItems in backpack:\n";
        if (inventory.empty()) {
            std::cout << "  (empty)\n";
        } else {
            for (const auto& item : inventory) {
                std::cout << "  - " << item->getName() 
                          << " [" << item->getType() << "] "
                          << "(Weight: " << item->getWeight() << ")\n";
            }
        }
        
        std::cout << "\nCurrent Stats (with equipment bonuses):\n";
        std::cout << "  Attack: " << getAttack() << " (Base: " << attack << ")\n";
        std::cout << "  Defence: " << getDefence() << " (Base: " << defence << ")\n";
        std::cout << "  Health: " << getHealth() << " (Base: " << health << ")\n";
        std::cout << "  Strength: " << getStrength() << " (Base: " << strength << ")\n";
    }

    const std::vector<std::shared_ptr<Item>>& getInventory() const {
        return inventory;
    }

    // --- SPECIAL DEFENCE HOOK ---
    virtual int specialDefence(int damage) { 
        (void)damage; 
        return damage; 
    }
};

#endif