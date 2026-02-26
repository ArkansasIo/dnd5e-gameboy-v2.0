# Quest Design Guide

## Quest Types
- Main Quest: mandatory chapter progression.
- Side Quest: optional progression, lore, economy, faction leverage.

## Quest Template
- Quest ID
- Title
- Narrative hook
- Objectives (3-step recommended)
- Encounter budget target
- Recommended level band
- Reward package
- Fail/partial-success behavior

## Design Rules
- Every chapter must ship with 1 main + 2 side quests.
- Main quests must include at least one social or exploration gate before final combat.
- Side quests should either:
  - grant progression resources, or
  - alter faction reputation, or
  - reveal strategic intel for later chapters.

## Balance Heuristics
- Main quest boss CR should be near chapter recommended ceiling.
- Side quests should run one tier below chapter mainline difficulty.
- Ensure at least one low-risk recovery option every two chapters.

## QA Checklist
- Quest can complete.
- Quest can fail cleanly.
- Rewards cannot soft-lock future progression.
- Journal text/state transitions are coherent.
