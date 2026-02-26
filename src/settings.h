#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <stdint.h>
#include <stdbool.h>

// Video modes
typedef enum VideoMode {
    VIDEO_AUTO = 0,
    VIDEO_BW,      // Original Game Boy (black & white/green)
    VIDEO_CGB,     // Game Boy Color
    VIDEO_GBA,     // Game Boy Advance
    VIDEO_GBASP    // Game Boy Advance SP (brighter)
} VideoMode;

// Audio modes
typedef enum AudioMode {
    AUDIO_ON = 0,
    AUDIO_OFF,
    AUDIO_MUSIC_ONLY,
    AUDIO_SFX_ONLY
} AudioMode;

// Settings struct
typedef struct GameSettings {
    VideoMode video_mode;
    AudioMode audio_mode;
    uint8_t music_volume; // 0-10
    uint8_t sfx_volume;   // 0-10
} GameSettings;

extern GameSettings settings;

void apply_video_mode(VideoMode mode);
void apply_audio_mode(AudioMode mode);
void show_options_menu(void);

void load_settings_from_file(const char* filename);
void save_settings_to_file(const char* filename);

#endif
