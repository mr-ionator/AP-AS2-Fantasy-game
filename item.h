#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>


//class Item
//Abstract base class for all items in the game

class Item {
protected:
    std::string name;
    int weight;
    int attackModifier;
    int defenceModifier;
    int healthModifier;
    int strengthModifier;

public:
    
     // Constructor
     // n= Item name
     // w= Weight
     // a= Attack modifier
     // d= Defence modifier
     // h= Health modifier
     // s= Strength modifier
     //maybe make them more descriptive later but i have commented their full forms till then
    Item(std::string n, int w, int a, int d, int h, int s);
    
    virtual ~Item() = default;
    
    
    //Pure virtual function for item type
    //will return the category of item
    
    virtual std::string getType() const = 0;
    
    // Getters
    std::string getName() const;
    int getWeight() const;
    int getAttackModifier() const;
    int getDefenceModifier() const;
    int getHealthModifier() const;
    int getStrengthModifier() const;
};

// Weapon Items
class Sword : public Item {
public:
    Sword();
    std::string getType() const override;
};

class Dagger : public Item {
public:
    Dagger();
    std::string getType() const override;
};

// Armour Items
class PlateArmour : public Item {
public:
    PlateArmour();
    std::string getType() const override;
};

class LeatherArmour : public Item {
public:
    LeatherArmour();
    std::string getType() const override;
};

// Shield Items
class LargeShield : public Item {
public:
    LargeShield();
    std::string getType() const override;
};

class SmallShield : public Item {
public:
    SmallShield();
    std::string getType() const override;
};

// Ring Items
class RingOfLife : public Item {
public:
    RingOfLife();
    std::string getType() const override;
};

class RingOfStrength : public Item {
public:
    RingOfStrength();
    std::string getType() const override;
};

#endif