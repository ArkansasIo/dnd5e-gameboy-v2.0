# Test Plan

## 1. Build Tests
- `make assets` succeeds.
- `make` succeeds and emits `.gbc`.
- No fatal compile/link errors.

## 2. Runtime Smoke Tests
- Boot to title screen.
- Navigate main menu and hero select.
- Start a battle and resolve at least one turn.
- Save and load from at least one slot.

## 3. Content Validation
- Story totals: 12 acts / 50 chapters.
- Quest totals: 50 main / 100 side.
- Dungeon total: 72.
- Player classes total: 13.

## 4. Regression Tests
- Title screen state transitions.
- Main menu cursor logic.
- Asset conversion tools output stability.

## 5. Performance Checks
- Battle input responsiveness.
- Transition latency between map and combat.
- Memory/bank pressure on content-heavy scenes.

## 6. Acceptance Criteria
- Full campaign data loads without corruption.
- No progression blockers in critical path chapters.
- Build reproducible from clean environment.
