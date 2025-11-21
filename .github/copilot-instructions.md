# Copilot / AI assistant instructions — AP-AS2-Fantasy-game

This small C++ console project is a text-based fantasy RPG (board + turn-based combat). The goal of this file is to give an AI coding agent the immediate, actionable knowledge it needs to make safe, useful edits.

Keep guidance concise and code-aware. When editing, prefer minimal, well-scoped changes and preserve existing design decisions unless a clear bug is present.

Key entry points and responsibilities
- `main.cpp` — program entrypoint and game loop. Responsible for: player selection, initial NPC placement, command parsing (n/s/e/w/a/q), moving the player on the `Board`, flipping day/night every 5 moves, and invoking combat via `combatRound`.
- `Board.h` — `Board` + `Square` types. `Board::getSquare(x,y)` returns nullptr out-of-bounds; the grid is stored as `grid[y][x]` (y = row). `Square` holds `shared_ptr` to player and npc.
- `race.h` — concrete race types derived from `Character` (Human, Elf, Hobbit, Dwarf). Each implements `printStats()` and `specialDefence(int damage)`.
- `Character.h` — base `Character` type (stats, gold, health helpers). Note: `specialDefence(int)` returns an int which is interpreted as the damage to apply to the defender (see `combat.h`).
- `combat.h` — inline combat implementation. `singleAttack` and `combatRound` live in header and use a single RNG instance (mt19937). Do not refactor into separate compilation units without preserving RNG lifetime/semantics.
- `item.h` / `items.cpp` — item classes implemented but NOT currently wired into `Character` or the world. Items exist and can be used as drop-in features, but codebase does not use them yet.

Important behavioral details (do not break these unintentionally)
- Combat flow (see `combat.h`): attacker rolls (uses attackChance), on success damage = max(0, attack - defence). Defender then rolls; if defence succeeds, `specialDefence(damage)` is called and its return value is applied via `defender->takeDamage(returnedValue)`. Thus `specialDefence` should return the final damage amount to apply. Example: `Elf::specialDefence` heals 1 HP then returns 0 (no damage applied).
- Randomness: `combat.h` uses `<random>` (mt19937 + uniform_real_distribution). `Hobbit::specialDefence` uses `rand() % 6`. This mixing of RNGs yields non-deterministic behavior and makes tests noisier. If you need reproducible tests, consolidate RNG use and seed control.
- Ownership: characters and square occupants use `std::shared_ptr<Character>` throughout. Be careful when changing lifetimes or ownership — removing `shared_ptr` usage requires thorough updates.
- Board coordinates: `Board` constructor takes (width, height) and creates `grid[height][width]`. Access pattern is `grid[y][x]`. `getSquare` returns nullptr for out-of-bounds — check callers.
- Gold reward: main.cpp grants gold as `npc->getDefence()` when NPC is defeated. Do not alter reward formula without updating UI and README.

Patterns & conventions you can rely on
- Lightweight header-only logic: `combat.h` contains inline functions. These are intentionally header-only to keep logic local to includes — be careful about ODR and linking if duplicated across translation units.
- Small, self-contained types: each header groups a single concern (Board, Character/races, Item). New features should follow the same per-concern header/impl split.
- Shared pointers for dynamic objects: prefer `std::shared_ptr<Character>` for characters stored on squares.
- Minimal UI: input is via std::getline/std::cin; keep prompts simple and console-only. Tests or automation should simulate stdin where needed.

Weak spots and safe improvement opportunities (documented so AI agents don't break behavior)
- Items are implemented but not used. Integrating items requires adding inventory/state to `Character` and updating `printStats()` and combat calculations.
- RNG mix: replace `rand()` usage (Hobbit) with std::random utilities for consistent behavior.
- Inline combat in header: if refactoring to .cpp, ensure the RNG stays a single shared instance or is seeded identically during tests.
- No unit tests or CI — prefer small, focused tests around `singleAttack` and `specialDefence` behavior before larger refactors.

Examples of targeted edits an AI agent can safely perform
- Fix a small bug (e.g., boundary check issue) by editing `main.cpp` movement logic and validating `getSquare` returns nullptr.
- Replace `rand()` in `Hobbit::specialDefence` with `std::uniform_int_distribution<int>` seeded by a test-controlled RNG.
- Add a minimal `Makefile` or `CMakeLists.txt` listing `main.cpp` and `items.cpp`.
- Wire `Item` usage into `Character`: add an inventory vector in `Character.h`, then update `printStats()` and damage calc in `combat.h`.

If unsure, follow these priorities when making changes
1. Preserve public behavior (gameplay messages and combat outcome semantics).
2. Keep edits minimal and well-scoped — prefer adding new files over large rewrites.
3. Add unit tests for any change that touches combat, RNG, or ownership.

If you need more context
- Ask the repo owner where they'd like items wired in (inventory structure and pickup/drop rules).
- Ask whether deterministic RNG (seeded) is OK for tests, or if randomness must remain unpredictable.

---
Please review any unclear or missing parts of this guidance (for example: intended item mechanics, desired build toolchain, or testing preferences) and I will iterate on this file.
