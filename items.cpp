#include "item.h"

// Base Item class implementation
Item::Item(std::string n, int w, int a, int d, int h, int s)
    : name(n), weight(w), attackModifier(a), defenceModifier(d), 
      healthModifier(h), strengthModifier(s) {}

std::string Item::getName() const { return name; }
int Item::getWeight() const { return weight; }
int Item::getAttackModifier() const { return attackModifier; }
int Item::getDefenceModifier() const { return defenceModifier; }
int Item::getHealthModifier() const { return healthModifier; }
int Item::getStrengthModifier() const { return strengthModifier; }

// Sword implementation
Sword::Sword() : Item("Sword", 10, 10, 0, 0, 0) {}
std::string Sword::getType() const { return "Weapon"; }

// Dagger implementation
Dagger::Dagger() : Item("Dagger", 5, 5, 0, 0, 0) {}
std::string Dagger::getType() const { return "Weapon"; }

// Plate Armour implementation
PlateArmour::PlateArmour() : Item("Plate Armour", 40, -5, 10, 0, 0) {}
std::string PlateArmour::getType() const { return "Armour"; }

// Leather Armour implementation
LeatherArmour::LeatherArmour() : Item("Leather Armour", 20, 0, 5, 0, 0) {}
std::string LeatherArmour::getType() const { return "Armour"; }

// Large Shield implementation
LargeShield::LargeShield() : Item("Large Shield", 30, -5, 10, 0, 0) {}
std::string LargeShield::getType() const { return "Shield"; }

// Small Shield implementation
SmallShield::SmallShield() : Item("Small Shield", 10, 0, 5, 0, 0) {}
std::string SmallShield::getType() const { return "Shield"; }

// Ring of Life implementation
RingOfLife::RingOfLife() : Item("Ring of Life", 1, 0, 0, 10, 0) {}
std::string RingOfLife::getType() const { return "Ring"; }

// Ring of Strength implementation
RingOfStrength::RingOfStrength() : Item("Ring of Strength", 1, 0, 0, -10, 50) {}
std::string RingOfStrength::getType() const { return "Ring"; }