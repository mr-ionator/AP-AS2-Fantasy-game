#include <bits/stdc++.h>
using namespace std;

enum class Category { Weapon, Armour, Shield, Ring };

struct Item {
    string name;
    Category cat;
    int weight = 0;
    int attack = 0;
    int defence = 0;
    int health = 0;
    int strength = 0;
};

struct Stats {
    int attack = 0;
    int defence = 0;
    int health = 0;
    int strength = 0;
};

static vector<Item> predefinedItems() {
    return {    
        {"Sword", Category::Weapon, 10, +10,  0,  0,  0},
        {"Dagger", Category::Weapon, 5,  +5,   0,  0,  0},
        {"Plate Armour", Category::Armour, 40, -5, +10, 0,  0}, // increases Defence 10, decreases Attack 5
        {"Leather Armour", Category::Armour, 20, 0,  +5,  0,  0},
        {"Large Shield", Category::Shield, 30, -5, +10, 0,  0},
        {"Small Shield", Category::Shield, 10, 0,  +5,  0,  0},
        {"Ring of Life", Category::Ring, 1,  0,  0, +10, 0},
        {"Ring of Strength", Category::Ring, 1,  0,  0, -10, +50}
    };
}

struct Character {
    string name;
    // base (unmodified) abilities
    int baseAttack = 0;
    int baseDefence = 0;
    int baseHealth = 100;
    int baseStrength = 10;

    // equipped items
    optional<Item> weapon;
    optional<Item> armour;
    optional<Item> shield;
    vector<Item> rings;

    // compute aggregated modifiers from equipped items
    Stats modifiers() const {
        Stats s{};
        auto apply = [&s](const Item &it){
            s.attack += it.attack;
            s.defence += it.defence;
            s.health += it.health;
            s.strength += it.strength;
        };
        if (weapon) apply(*weapon);
        if (armour) apply(*armour);
        if (shield) apply(*shield);
        for (auto &r : rings) apply(r);
        return s;
    }

    // effective stats (base + modifiers)
    Stats effective() const {
        Stats m = modifiers();
        return { baseAttack + m.attack,
                 baseDefence + m.defence,
                 baseHealth + m.health,
                 baseStrength + m.strength };
    }

    int totalWeightWith(const optional<Item>& extra = nullopt) const {
        int w = 0;
        if (weapon) w += weapon->weight;
        if (armour) w += armour->weight;
        if (shield) w += shield->weight;
        for (auto &r : rings) w += r.weight;
        if (extra) w += extra->weight;
        return w;
    }

    // Check whether item can be equipped (validating slots, weight cap and ring count)
    bool canEquip(const Item &item, string &reason) const {
        // slot availability
        if (item.cat != Category::Ring) {
            if ((item.cat == Category::Weapon && weapon) ||
                (item.cat == Category::Armour && armour) ||
                (item.cat == Category::Shield && shield)) {
                reason = "Slot already occupied for category (unequip first to replace).";
                return false;
            }
        }

        // compute prospective modifiers including this item
        Stats m = modifiers();
        m.attack += item.attack;
        m.defence += item.defence;
        m.health += item.health;
        m.strength += item.strength;

        int prospectiveStrength = baseStrength + m.strength;
        if (prospectiveStrength < 0) prospectiveStrength = 0; // safety

        int prospectiveWeight = totalWeightWith(item);
        if (prospectiveWeight > prospectiveStrength) {
            reason = "Would exceed carrying capacity (weight " + to_string(prospectiveWeight)
                     + " > strength/capacity " + to_string(prospectiveStrength) + ").";
            return false;
        }

        if (item.cat == Category::Ring) {
            int prospectiveRings = (int)rings.size() + 1;
            if (prospectiveRings > prospectiveStrength) {
                reason = "Would exceed max number of rings allowed by strength.";
                return false;
            }
        }

        return true;
    }

    bool equip(const Item &item, string &reason) {
        if (!canEquip(item, reason)) return false;
        switch (item.cat) {
            case Category::Weapon: weapon = item; break;
            case Category::Armour: armour = item; break;
            case Category::Shield: shield = item; break;
            case Category::Ring: rings.push_back(item); break;
        }
        return true;
    }

    bool unequipCategory(Category cat) {
        switch (cat) {
            case Category::Weapon: weapon.reset(); return true;
            case Category::Armour: armour.reset(); return true;
            case Category::Shield: shield.reset(); return true;
            case Category::Ring:
                if (!rings.empty()) { rings.pop_back(); return true; }
                return false;
        }
        return false;
    }

    string equippedSummary() const {
        auto fmt = [](const optional<Item>& o){ return o ? o->name : string("None"); };
        ostringstream ss;
        ss << "Weapon: " << fmt(weapon) << ", Armour: " << fmt(armour)
           << ", Shield: " << fmt(shield) << ", Rings: [";
        for (size_t i = 0; i < rings.size(); ++i) {
            if (i) ss << ", ";
            ss << rings[i].name;
        }
        ss << "]";
        return ss.str();
    }

    void printStatus() const {
        Stats e = effective();
        int weight = totalWeightWith();
        cout << "== " << name << " ==\n";
        cout << "Base: A" << baseAttack << " D" << baseDefence << " H" << baseHealth << " S" << baseStrength << "\n";
        cout << "Effective: A" << e.attack << " D" << e.defence << " H" << e.health << " S" << e.strength << "\n";
        cout << "Carried weight: " << weight << " / Capacity: " << e.strength << "\n";
        cout << "Equipped: " << equippedSummary() << "\n\n";
    }
};

int main() {
    auto items = predefinedItems();
    // Utility to find item by name
    auto findItem = [&](const string &name)->optional<Item>{
        for (auto &it : items) if (it.name == name) return it;
        return nullopt;
    };

    Character c{"Adventurer", 0, 0, 100, 10};

    c.printStatus();

    string reason;
    // Try equipping Sword
    if (auto it = findItem("Sword")) {
        if (c.equip(*it, reason)) cout << "Equipped Sword\n"; else cout << "Cannot equip Sword: " << reason << "\n";
    }
    // Equip Plate Armour
    if (auto it = findItem("Plate Armour")) {
        if (c.equip(*it, reason)) cout << "Equipped Plate Armour\n"; else cout << "Cannot equip Plate Armour: " << reason << "\n";
    }
    // Equip Ring of Strength (this should increase capacity and allow heavier gear)
    if (auto it = findItem("Ring of Strength")) {
        if (c.equip(*it, reason)) cout << "Equipped Ring of Strength\n"; else cout << "Cannot equip Ring of Strength: " << reason << "\n";
    }
    // Equip another Ring of Strength (demonstrate stacking)
    if (auto it = findItem("Ring of Strength")) {
        if (c.equip(*it, reason)) cout << "Equipped another Ring of Strength\n"; else cout << "Cannot equip another Ring of Strength: " << reason << "\n";
    }
    // Try to equip Large Shield
    if (auto it = findItem("Large Shield")) {
        if (c.equip(*it, reason)) cout << "Equipped Large Shield\n"; else cout << "Cannot equip Large Shield: " << reason << "\n";
    }

    cout << "\nFinal status:\n";
    c.printStatus();

    return 0;
}