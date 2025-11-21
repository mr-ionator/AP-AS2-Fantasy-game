#ifndef COMBAT_H
#define COMBAT_H

#include <iostream>
#include <memory>
#include <random>
#include "Character.h"

/**
 * Conducts one round of combat between attacker and defender.
 * - Attacker rolls for attack; if fails, round ends.
 * - If attack succeeds, defender rolls for defence:
 *   - On successful defence, applies race-specific effect via defender->specialDefence(damage).
 *   - On failed defence, defender loses (attack - defence) HP.
 * - Returns true if defender defeated.
 */
inline bool singleAttack(std::shared_ptr<Character> attacker, std::shared_ptr<Character> defender)
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<> dist(0.0, 1.0);

    double attackRoll = dist(rng);
    std::cout << attacker->getRace() << " attempts an attack (chance "
              << attacker->getAttackChance() << "): ";
    if (attackRoll > attacker->getAttackChance()) {
        std::cout << "Attack failed!\n";
        return false;
    }
    std::cout << "Attack succeeded!\n";

    int damage = std::max(0, attacker->getAttack() - defender->getDefence());

    double defenceRoll = dist(rng);
    std::cout << defender->getRace() << " attempts to defend (chance "
              << defender->getDefenceChance() << "): ";
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
 * Player attacks NPC/enemy. If the enemy survives, it counter-attacks the player.
 * Both attacks follow special race rules.
 * Returns true if BOTH survived, false if one was defeated.
 */
inline bool combatRound(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy)
{
    std::cout << "\n--- Player attacks NPC ---\n";
    bool npcDefeated = singleAttack(player, enemy);
    if (npcDefeated) return false;

    std::cout << "\n--- NPC counter-attacks Player ---\n";
    bool playerDefeated = singleAttack(enemy, player);
    return !playerDefeated; // return true if both are alive, false if player died
}

#endif // COMBAT_H
