# Game Design Document (GDD)

## 1. Game Identity
- Title: `Labyrinth of the Dragon`
- Genre: 8-bit turn-based RPG inspired by DND5E.
- Platform: Game Boy / Game Boy Color (GBDK-2020 toolchain).
- Core Fantasy: Build a party hero, explore kingdoms/dungeons, resolve a 12-act campaign, defeat high-tier threats.

## 2. Pillars
- Tactical turn-based combat with readable outcomes.
- High content density: story acts, chapters, main + side quests, dungeon progression.
- Retro hardware constraints with modern content generation tooling.

## 3. Player Experience Goals
- Clear strategic choices each encounter.
- Steady progression (levels, gear, class identity).
- Persistent world progression across chapters and acts.

## 4. Game Loop
1. Story chapter briefing in hub city.
2. Travel/exploration and side objective discovery.
3. Combat encounters and dungeon progression.
4. Reward, reputation, and quest completion.
5. Story advancement to next chapter/act.

## 5. Core Systems
- Character stats and class identity.
- Encounter and combat resolution.
- Itemization and equipment.
- Quest state and chapter state.
- Faction reputation and narrative branch flags.
- Save/load progression.

## 6. Progression
- 13 base DND5E-style classes (including Artificer) in generated content.
- XP and leveling model (target cap: level 20).
- Story pacing over 12 acts and 50 chapters.
- Endgame encounters via dungeon bosses and act finales.

## 7. Narrative Structure
- Campaign: `OG Story Mode`.
- Acts: 12.
- Chapters: 50.
- Main Quests: 50.
- Side Quests: 100.
- Total Quest Units: 150.

## 8. Content Scale Targets
- 12 kingdoms.
- 72 dungeons.
- Biome taxonomy (class/subclass/type/subtype).
- Large bestiary + generated compendium variants.

## 9. UX/Accessibility Constraints
- Fast read battle messaging.
- Minimal menu depth for repeated actions.
- Inputs optimized for D-pad + A/B + Start/Select.

## 10. Non-Goals (Current Milestone)
- Real-time action combat.
- Fully branching cinematic dialogue trees.
- Online multiplayer.

## 11. Shipping Milestones
- M1: Stable build + core combat + save/load.
- M2: World/story data integration + chapter progression.
- M3: Full campaign playable start-to-finish.
- M4: Balance pass + performance + QA lock.
