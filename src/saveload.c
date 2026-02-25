#include "saveload.h"
#include <string.h>

// SRAM address for each save slot (adjust size as needed)
#define SAVE_SLOT_SIZE sizeof(SaveData)
#define SRAM_BASE 0xA000

static uint16_t save_slot_addr(uint8_t slot) {
    return SRAM_BASE + slot * SAVE_SLOT_SIZE;
}

bool save_game(uint8_t slot, const SaveData* data) {
    if (slot >= SAVE_SLOT_COUNT) return false;
    uint16_t addr = save_slot_addr(slot);
    ENABLE_RAM;
    ((SaveData*)addr)->magic = SAVE_MAGIC;
    memcpy((void*)addr, data, sizeof(SaveData));
    DISABLE_RAM;
    return true;
}

bool load_game(uint8_t slot, SaveData* data) {
    if (slot >= SAVE_SLOT_COUNT) return false;
    uint16_t addr = save_slot_addr(slot);
    ENABLE_RAM;
    memcpy(data, (void*)addr, sizeof(SaveData));
    DISABLE_RAM;
    return (data->magic == SAVE_MAGIC);
}

void erase_save(uint8_t slot) {
    if (slot >= SAVE_SLOT_COUNT) return;
    uint16_t addr = save_slot_addr(slot);
    ENABLE_RAM;
    ((SaveData*)addr)->magic = 0;
    DISABLE_RAM;
}

bool save_exists(uint8_t slot) {
    if (slot >= SAVE_SLOT_COUNT) return false;
    uint16_t addr = save_slot_addr(slot);
    ENABLE_RAM;
    uint16_t magic = ((SaveData*)addr)->magic;
    DISABLE_RAM;
    return (magic == SAVE_MAGIC);
}
