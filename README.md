# Labyrinth of the Dragon (GBC)
An 8-bit Adventure RPG with D&D Monsters!

## How to Build the ROM

### Depedencies
* [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) - The Game Boy Development
  kit. Includes the libraries and binaries for C development on the Game Boy.
* [GNU Make](https://gnuwin32.sourceforge.net/packages/make.htm) - Build system
  tool (installation should only be required on Windows).
* [NodeJS](https://nodejs.org) - Used to run custom tools I made in the course
  of developing the game.

### Use Make to Build the ROM
Update the `MakeFile` or define a shell variable named `GBDK_HOME` pointing to
the directory where you installed GBDK.

To build the ROM run the following commands:

* `npm install`
* `make assets`
* `make`

## Save and Load System

This game supports saving and loading progress using the Game Boy's battery-backed SRAM. There are 3 save slots. The save system stores basic player data (level, HP, XP, etc.) and can be expanded as needed.

### How it works
- **Saving:** When you save, the current player state is written to SRAM in the selected slot.
- **Loading:** When you load, the player state is restored from SRAM.
- **Erasing:** You can erase a save slot to clear its data.

### Technical details
- Save data is defined in `src/saveload.h` and implemented in `src/saveload.c`.
- Integration with the main menu is in `src/main_menu.c`.
- SRAM is accessed using GBDK's `ENABLE_RAM`/`DISABLE_RAM` macros.

### Extending Save Data
To add more fields to the save file, update the `SaveData` struct in `src/saveload.h` and adjust the save/load logic as needed.
