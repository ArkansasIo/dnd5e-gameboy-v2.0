#include "settings.h"
#include "core.h"
#include "palette.h"
#include "sound.h"
#include <string.h>

GameSettings settings = {
    VIDEO_AUTO,
    AUDIO_ON,
    10,
    10
};

// Helper: parse string to enum
static VideoMode parse_video_mode(const char* str) {
    if (strcmp(str, "bw") == 0) return VIDEO_BW;
    if (strcmp(str, "cgb") == 0) return VIDEO_CGB;
    if (strcmp(str, "gba") == 0) return VIDEO_GBA;
    if (strcmp(str, "gbasp") == 0) return VIDEO_GBASP;
    return VIDEO_AUTO;
}
static AudioMode parse_audio_mode(const char* str) {
    if (strcmp(str, "off") == 0) return AUDIO_OFF;
    if (strcmp(str, "music_only") == 0) return AUDIO_MUSIC_ONLY;
    if (strcmp(str, "sfx_only") == 0) return AUDIO_SFX_ONLY;
    return AUDIO_ON;
}

void apply_video_mode(VideoMode mode) {
    switch (mode) {
    case VIDEO_BW:
        // Set palettes for original Game Boy (grayscale/green)
        // Example: core.load_bg_palette(bw_palette, 0, 8);
        break;
    case VIDEO_CGB:
        // Set palettes for Game Boy Color
        // Example: core.load_bg_palette(cgb_palette, 0, 8);
        break;
    case VIDEO_GBA:
        // Set palettes for GBA (slightly brighter)
        // Example: core.load_bg_palette(gba_palette, 0, 8);
        break;
    case VIDEO_GBASP:
        // Set palettes for GBA SP (even brighter)
        // Example: core.load_bg_palette(gbasp_palette, 0, 8);
        break;
    case VIDEO_AUTO:
    default:
        // Detect and set best mode
        break;
    }
}

void apply_audio_mode(AudioMode mode) {
    settings.audio_mode = mode;
    switch (mode) {
    case AUDIO_OFF:
        break;
    case AUDIO_MUSIC_ONLY:
        break;
    case AUDIO_SFX_ONLY:
        break;
    case AUDIO_ON:
    default:
        break;
    }
}

void show_options_menu(void) {
    // TODO: Implement options menu UI for video/audio settings
    // Example: let user select video mode, audio mode, adjust volumes
}

void load_settings_from_file(const char* filename) {
    (void)filename;
    // File-based settings are not available in this target build yet.
}

void save_settings_to_file(const char* filename) {
    (void)filename;
    // File-based settings are not available in this target build yet.
}
