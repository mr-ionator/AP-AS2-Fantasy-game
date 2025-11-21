#ifndef COMBAT_H
#define COMBAT_H

#include <iostream>
#include <random>
#include <memory>
#include "Character.h"

/**
 * Conducts one round of combat between attacker and defender.
 * Uses modified stats (including item bonuses) as per PDF requirements.
 */
inline bool singleAttack(std::shared_ptr<Character> attacker, std::shared_ptr<Character> defender)
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<> dist(0.0, 1.0);

    double attackRoll = dist(rng);
    
    // Get modified attack/defence stats (includes item bonuses)
    int attackValue = attacker->getAttack();
    int defenceValue = defender->getDefence();
    
    std::cout << attacker->getRace() << " attempts an attack (chance "
              << attacker->getAttackChance() << ", Attack: " << attackValue << "): ";
    
    if (attackRoll > attacker->getAttackChance()) {
        std::cout << "Attack failed!\n";
        return false;
    }

    std::cout << "Attack succeeded!\n";
    
    // Calculate damage using modified stats
    int damage = std::max(0, attackValue - defenceValue);
    
    double defenceRoll = dist(rng);
    std::cout << defender->getRace() << " attempts to defend (chance "
              << defender->getDefenceChance() << ", Defence: " << defenceValue << "): ";
    
    if (defenceRoll < defender->getDefenceChance()) {
        std::cout << "Defence succeeded! Applying race-specific effect.\n";
        int reducedDamage = defender->specialDefence(damage);
        defender->takeDamage(reducedDamage);
        std::cout << defender->getRace() << " takes " << reducedDamage
                  << " damage (Health now " << defender->getHealth() << ").\n";
        return defender->getHealth() <= 0;
    } else {
        std::cout << "Defence failed!\n";
        defender->takeDamage(damage);
        std::cout << defender->getRace() << " takes " << damage
                  << " damage (Health now " << defender->getHealth() << ").\n";
        return defender->getHealth() <= 0;
    }
}

/**
 * Player attacks NPC. If NPC survives, it counter-attacks.
 * Returns true if both survived, false if one was defeated.
 */
inline bool combatRound(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy)
{
    std::cout << "\n--- Player attacks NPC ---\n";
    bool npcDefeated = singleAttack(player, enemy);
    if (npcDefeated) return false;

    std::cout << "\n--- NPC counter-attacks Player ---\n";
    bool playerDefeated = singleAttack(enemy, player);
    return !playerDefeated;
}

#endif