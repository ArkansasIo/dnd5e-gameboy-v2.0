#pragma bank 1

#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>

#include "core.h"
#include "main_menu.h"
#include "player.h"
#include "saveload.h"
#include "title_screen.h"


/**
 * Top level state for the main menu.
 */
MainMenuState main_menu_state = TITLE;
uint8_t cursor = 0;


Timer menu_walk_timer;
uint8_t menu_walk_frame;
Tilemap save_select_tilemap = { 20, 18, 1, tilemap_save_select };

/**
 * Initializes the save select screen.
 */
void init_save_select(void) {
  DISPLAY_OFF;

  // Load save slot states and display info for each slot
  for (uint8_t slot = 0; slot < SAVE_SLOT_COUNT; ++slot) {
    uint8_t y = 40 + slot * 16; // Adjust Y position for each slot
    if (save_exists(slot)) {
      SaveData data;
      load_game(slot, &data);
      // GBDK/SDCC builds can fail on snprintf; render compact fields directly.
      char name_buf[11];
      // Convert class enum to string (simple example)
      const char* class_str = "?";
      switch (data.player_class) {
        case 0: class_str = "DRU"; break;
        case 1: class_str = "FTR"; break;
        case 2: class_str = "MNK"; break;
        case 3: class_str = "SOR"; break;
        default: break;
      }

      // Copy up to 10 chars of the player name.
      uint8_t i = 0;
      while (i < 10 && data.player_name[i] != '\0') {
        name_buf[i] = data.player_name[i];
        i++;
      }
      name_buf[i] = '\0';

      // Draw compact save metadata on two lines.
      core.draw_text((uint8_t*)name_buf, 40, y);
      core.draw_text((uint8_t*)class_str, 112, y);
    } else {
      // Draw "EMPTY" at (x=40, y)
      core.draw_text((uint8_t*)"EMPTY", 40, y);
    }
  }

  // Load the save select screen tilemap
  core.draw_tilemap(save_select_tilemap, VRAM_BACKGROUND);

  // Reset the cursor sprite and state
  cursor = SAVE_SELECT_CURSOR_SAVE1;
  set_bkg_palette(4, 3, save_1_selected_palettes);
  set_sprite_palette(0, 1, save_1_selected_palettes);
  set_sprite_tile(SPRITE_CURSOR, 0x9E);
  set_sprite_prop(SPRITE_CURSOR, 0x08);
  move_sprite(SPRITE_CURSOR, 22, 52);

  // Set up the animated "hero" sprites for each save file
  set_sprite_palette(1, 4, hero_palettes);

  // Hero 1
  set_sprite_tile(SPRITE_HERO1, 0x00);
  set_sprite_tile(SPRITE_HERO1 + 1, 0x01);
  set_sprite_tile(SPRITE_HERO1 + 2, 0x10);
  set_sprite_tile(SPRITE_HERO1 + 3, 0x11);

  set_sprite_prop(SPRITE_HERO1, 0x02);
  set_sprite_prop(SPRITE_HERO1 + 1, 0x02);
  set_sprite_prop(SPRITE_HERO1 + 2, 0x02);
  set_sprite_prop(SPRITE_HERO1 + 3, 0x02);

  move_sprite(SPRITE_HERO1, HERO1_X, HERO1_Y);
  move_sprite(SPRITE_HERO1 + 1, HERO1_X + 8, HERO1_Y);
  move_sprite(SPRITE_HERO1 + 2, HERO1_X, HERO1_Y + 8);
  move_sprite(SPRITE_HERO1 + 3, HERO1_X + 8, HERO1_Y + 8);

  // Hero 2
  set_sprite_tile(SPRITE_HERO2, 0x40);
  set_sprite_tile(SPRITE_HERO2 + 1, 0x41);
  set_sprite_tile(SPRITE_HERO2 + 2, 0x50);
  set_sprite_tile(SPRITE_HERO2 + 3, 0x51);

  set_sprite_prop(SPRITE_HERO2, 0x01);
  set_sprite_prop(SPRITE_HERO2 + 1, 0x01);
  set_sprite_prop(SPRITE_HERO2 + 2, 0x01);
  set_sprite_prop(SPRITE_HERO2 + 3, 0x01);

  move_sprite(SPRITE_HERO2, HERO2_X, HERO2_Y);
  move_sprite(SPRITE_HERO2 + 1, HERO2_X + 8, HERO2_Y);
  move_sprite(SPRITE_HERO2 + 2, HERO2_X, HERO2_Y + 8);
  move_sprite(SPRITE_HERO2 + 3, HERO2_X + 8, HERO2_Y + 8);

  // Hero 3
  set_sprite_tile(SPRITE_HERO3, 0x60);
  set_sprite_tile(SPRITE_HERO3 + 1, 0x61);
  set_sprite_tile(SPRITE_HERO3 + 2, 0x70);
  set_sprite_tile(SPRITE_HERO3 + 3, 0x71);

  set_sprite_prop(SPRITE_HERO3, 0x01);
  set_sprite_prop(SPRITE_HERO3 + 1, 0x01);
  set_sprite_prop(SPRITE_HERO3 + 2, 0x01);
  set_sprite_prop(SPRITE_HERO3 + 3, 0x01);

  move_sprite(SPRITE_HERO3, HERO3_X, HERO3_Y);
  move_sprite(SPRITE_HERO3 + 1, HERO3_X + 8, HERO3_Y);
  move_sprite(SPRITE_HERO3 + 2, HERO3_X, HERO3_Y + 8);
  move_sprite(SPRITE_HERO3 + 3, HERO3_X + 8, HERO3_Y + 8);

  init_timer(menu_walk_timer, 12);

  DISPLAY_ON;
}

void move_save_select_cursor(void) {
  if (was_pressed(J_UP)) {
    cursor = (cursor == SAVE_SELECT_CURSOR_SAVE1)
      ? SAVE_SELECT_CURSOR_ERASE
      : cursor - 1;
  } else if (was_pressed(J_DOWN)) {
    cursor = (cursor == SAVE_SELECT_CURSOR_ERASE)
      ? SAVE_SELECT_CURSOR_SAVE1
      : cursor + 1;
  } else {
    return;
  }

  switch (cursor) {
  case SAVE_SELECT_CURSOR_SAVE1:
    move_sprite(SPRITE_CURSOR, 22, 52);
    set_bkg_palette(4, 3, save_1_selected_palettes);
    for (uint8_t s = 0; s < 4; s++) {
      set_sprite_prop(SPRITE_HERO1 + s, 0x02);
      set_sprite_prop(SPRITE_HERO2 + s, 0x01);
      set_sprite_prop(SPRITE_HERO3 + s, 0x01);
    }
    break;
  case SAVE_SELECT_CURSOR_SAVE2:
    move_sprite(SPRITE_CURSOR, 22, 84);
    set_bkg_palette(4, 3, save_2_selected_palettes);
    for (uint8_t s = 0; s < 4; s++) {
      set_sprite_prop(SPRITE_HERO1 + s, 0x01);
      set_sprite_prop(SPRITE_HERO2 + s, 0x03);
      set_sprite_prop(SPRITE_HERO3 + s, 0x01);
    }
    break;
  case SAVE_SELECT_CURSOR_SAVE3:
    move_sprite(SPRITE_CURSOR, 22, 116);
    set_bkg_palette(4, 3, save_3_selected_palettes);
    for (uint8_t s = 0; s < 4; s++) {
      set_sprite_prop(SPRITE_HERO1 + s, 0x01);
      set_sprite_prop(SPRITE_HERO2 + s, 0x01);
      set_sprite_prop(SPRITE_HERO3 + s, 0x04);
    }
    break;
  case SAVE_SELECT_CURSOR_ERASE:
    move_sprite(SPRITE_CURSOR, 100, 143);
    set_bkg_palette(4, 3, save_none_selected_palettes);
    for (uint8_t s = 0; s < 4; s++) {
      set_sprite_prop(SPRITE_HERO1 + s, 0x01);
      set_sprite_prop(SPRITE_HERO2 + s, 0x01);
      set_sprite_prop(SPRITE_HERO3 + s, 0x01);
    }
    break;
  }
}

void update_save_select_sprites(void) {
  if (update_timer(menu_walk_timer)) {
    reset_timer(menu_walk_timer);
    menu_walk_frame ^= 1;
  }

  const uint8_t walk_tile_offset = menu_walk_frame << 1;
  const uint8_t hero1_offset = (cursor == SAVE_SELECT_CURSOR_SAVE1)
    ? walk_tile_offset
    : 0;
  const uint8_t hero2_offset = (cursor == SAVE_SELECT_CURSOR_SAVE2)
    ? walk_tile_offset
    : 0;
  const uint8_t hero3_offset = (cursor == SAVE_SELECT_CURSOR_SAVE3)
    ? walk_tile_offset
    : 0;

  set_sprite_tile(SPRITE_HERO1 + 0, 0x00 + hero1_offset);
  set_sprite_tile(SPRITE_HERO1 + 1, 0x01 + hero1_offset);
  set_sprite_tile(SPRITE_HERO1 + 2, 0x10 + hero1_offset);
  set_sprite_tile(SPRITE_HERO1 + 3, 0x11 + hero1_offset);

  set_sprite_tile(SPRITE_HERO2 + 0, 0x40 + hero2_offset);
  set_sprite_tile(SPRITE_HERO2 + 1, 0x41 + hero2_offset);
  set_sprite_tile(SPRITE_HERO2 + 2, 0x50 + hero2_offset);
  set_sprite_tile(SPRITE_HERO2 + 3, 0x51 + hero2_offset);

  set_sprite_tile(SPRITE_HERO3 + 0, 0x60 + hero3_offset);
  set_sprite_tile(SPRITE_HERO3 + 1, 0x61 + hero3_offset);
  set_sprite_tile(SPRITE_HERO3 + 2, 0x70 + hero3_offset);
  set_sprite_tile(SPRITE_HERO3 + 3, 0x71 + hero3_offset);
}




void init_main_menu(void) {
  DISPLAY_OFF;


  init_title_screen();

  /*
  // Load tiles
  core.load_font();
  core.load_all_heros();


  // Initialize menu state
  cursor = 0;

  // init_fire_animation();
  init_smoke_animation();
  */

  DISPLAY_ON;
}

void update_main_menu(void) {
  switch (main_menu_state) {
  case TITLE:
    update_title_screen();

    // Example: transition to save select
    // if (was_pressed(J_A | J_B | J_START)) {
    //   main_menu_state = SAVE_SELECT;
    //   init_save_select();
    // }
    break;
  case SAVE_SELECT:
    move_save_select_cursor();
    update_save_select_sprites();

    // Example: handle save/load/erase actions
    /*
    if (was_pressed(J_A)) {
      SaveData data;
      // Fill data from current player state
      data.player_level = player.level;
      data.player_hp = player.hp;
      data.player_xp = player.exp;
      save_game(cursor, &data);
    } else if (was_pressed(J_B)) {
      SaveData data;
      if (load_game(cursor, &data)) {
        // Load data into player struct
        player.level = data.player_level;
        player.hp = data.player_hp;
        player.exp = data.player_xp;
        // etc.
      }
    } else if (was_pressed(J_SELECT)) {
      erase_save(cursor);
    }
    */
    break;
  }
}

void draw_main_menu(void) {
}
