#ifndef _SAVELOAD_H
#define _SAVELOAD_H

#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>

#define SAVE_SLOT_COUNT 5
#define SAVE_MAGIC 0xBEEF

// Example player data structure to save
// You should expand this with all data you want to persist
typedef struct SaveData {
    uint16_t magic;
    char player_name[8];
    uint8_t player_class; // PlayerClass enum
    uint8_t player_rank;  // Custom rank value
    uint8_t player_level;
    uint16_t player_hp;
    uint16_t player_xp;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t reserved[16]; // Padding for future expansion
} SaveData;

// Save/load API
bool save_game(uint8_t slot, const SaveData* data);
bool load_game(uint8_t slot, SaveData* data);
void erase_save(uint8_t slot);
bool save_exists(uint8_t slot);

#endif // _SAVELOAD_H
