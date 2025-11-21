# Fantasy RPG Combat Simulator

A text-based fantasy role-playing game where players choose from different character races, explore a dynamic world, and engage in strategic turn-based combat against various NPC enemies.

## 🎮 Game Overview

Embark on a fantasy adventure where you control a character from one of four unique races, each with distinct strengths, weaknesses, and special abilities. Explore the game world, encounter enemies, and engage in combat to earn gold and prove your worth as a warrior.

## 🌟 Features

### Character System
- **Four Playable Races**: Human, Elf, Hobbit, and Dwarf
- **Unique Statistics**: Each race has customized attack, defense, health, and strength values
- **Special Abilities**: Race-specific defensive powers that activate during combat
- **Progression**: Earn gold by defeating enemies and track your wealth

### World Exploration
- **Grid-Based Map**: 10x6 game board for strategic movement
- **Dynamic Environment**: Day and night cycle that changes every 5 moves
- **NPC Encounters**: Various enemies placed throughout the world
- **Boundary Awareness**: Cannot move beyond the edges of the game board

### Combat System
- **Turn-Based Battles**: Strategic combat with attack and defense phases
- **Probability Mechanics**: Success based on character statistics and random chance
- **Counter-Attacks**: Enemies retaliate when they survive your assault
- **Visual Feedback**: Detailed combat logs showing each action and result

## 👥 Character Races

### Human 🧑
- **Combat Stats**: 
  - Attack: 30 | Defense: 20 | Health: 60 | Strength: 100
- **Success Rates**:
  - Attack Chance: 66.7% (2/3) | Defense Chance: 50% (1/2)
- **Special Ability**: No special defensive effect - reliable and balanced

### Elf 🧝
- **Combat Stats**:
  - Attack: 40 | Defense: 10 | Health: 40 | Strength: 70
- **Success Rates**:
  - Attack Chance: 100% (1/1) | Defense Chance: 25% (1/4)
- **Special Ability**: **Natural Healing** - Recovers 1 HP when successfully defending

### Hobbit 🍃
- **Combat Stats**:
  - Attack: 25 | Defense: 20 | Health: 70 | Strength: 85
- **Success Rates**:
  - Attack Chance: 33.3% (1/3) | Defense Chance: 66.7% (2/3)
- **Special Ability**: **Lucky Dodge** - Takes random reduced damage (0-5) when defending

### Dwarf ⚒️
- **Combat Stats**:
  - Attack: 30 | Defense: 20 | Health: 50 | Strength: 130
- **Success Rates**:
  - Attack Chance: 66.7% (2/3) | Defense Chance: 66.7% (2/3)
- **Special Ability**: No special defensive effect - tough and resilient

## 🎯 How to Play

### Getting Started
1. Launch the game and you'll be prompted to choose your character race
2. Select from numbers 1-4:
   - 1: Human
   - 2: Elf  
   - 3: Hobbit
   - 4: Dwarf
3. Review your character's statistics before beginning your adventure

### Controls
- **Movement**:
  - `n` - Move North (up)
  - `s` - Move South (down)
  - `e` - Move East (right)
  - `w` - Move West (left)
- **Combat**: `a` - Attack (only available when an NPC is present)
- **Game**: `q` - Quit the game

### Gameplay Flow

#### Exploration Phase
- You start at the center of the 10x6 game board
- Move around using the directional commands
- Every 5 moves, the time shifts between Day and Night
- The current square displays your position and any characters present

#### Combat Phase
When you encounter an NPC (indicated in square information):
1. Use the `a` command to initiate combat
2. **Player Attack Round**:
   - Game checks if your attack succeeds based on attack chance
   - If successful, NPC attempts to defend
   - Successful NPC defense triggers their special ability
   - Failed NPC defense results in full damage
3. **NPC Counter-Attack Round** (if NPC survives):
   - NPC attacks using the same mechanics
   - You defend with your own chance and special ability
4. Combat continues until one character is defeated

#### Rewards and Progression
- **Gold System**: Defeating an NPC rewards you with gold equal to their defense value
- **Health Management**: Monitor your health throughout combat
- **Victory Condition**: Survive and collect as much gold as possible
- **Defeat Condition**: Your health reaches 0

## 🛡️ Combat Mechanics

### Attack Sequence
1. **Attack Roll**: Random check against attacker's attack chance
2. **Damage Calculation**: `max(0, attacker_attack - defender_defense)`
3. **Defense Roll**: Random check against defender's defense chance
4. **Outcome**:
   - Defense Success: Special ability activates, reduced damage
   - Defense Failure: Full damage applied

### Special Ability Effects
- **Elf**: +1 HP when defending successfully
- **Hobbit**: Takes 0-5 random damage instead of calculated damage
- **Human/Dwarf**: No special effect (take normal reduced damage)

### Strategic Considerations
- **Elves** have perfect attack accuracy but poor defense
- **Hobbits** are defensive specialists with healing capabilities  
- **Humans** are well-rounded with no particular weaknesses
- **Dwarves** have balanced offensive and defensive capabilities



## 💰 Economy System

- **Gold Sources**: Only from defeating NPCs
- **Reward Calculation**: Gold = NPC's defense value
- **Tracking**: Current gold displayed after each move and when defeating enemies

## 🎲 Game Information Display

The game provides comprehensive information:
- Current position coordinates
- Characters present on your square
- Player health and gold status
- Time of day (Day/Night)
- Available commands based on situation
- Detailed combat logs with success/failure messages

## ❓ Frequently Asked Questions

**Q: What happens if I try to move off the board?**
A: The game will display "Cannot move there" and you remain in your current position.

**Q: Can I avoid combat with NPCs?**
A: Yes, you can choose to move away instead of attacking when an NPC is present.

**Q: Is there a way to recover health?**
A: Currently, only Elves can recover health through their special ability. Other races must manage their health carefully.

**Q: What's the win condition?**
A: There's no specific win condition - see how much gold you can collect before being defeated or choosing to quit.

**Q: Are there different types of NPCs?**
A: Yes, NPCs include all four race types, each with their respective statistics and abilities.

Enjoy your adventure in this fantasy world! Choose your race wisely and may your attacks always hit true!
