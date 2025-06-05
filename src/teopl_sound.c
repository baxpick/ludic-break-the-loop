//-----------------------------LICENSE NOTICE-----------------------------------
//     __    __  ______  __________
//    / /   / / / / __ \/  _/ ____/
//   / /   / / / / / / // // /
//  / /___/ /_/ / /_/ // // /___
// /_____/\____/_____/___/\____/
// (break the loop)
//
// This file is part of Amstrad CPC game "LUDIC break the loop".
//
//  "LUDIC break the loop" is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  "LUDIC break the loop" is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Code, Concept, Story Copyright (C) 2019 Naad (@teopl)
//  Art, Music, Story Copyright (C) 2019 Branho (@brando464)
//
//
//
// Developed for the CPCRetrodev contest 2019 edition.
//
//------------------------------------------------------------------------------

#include "teopl_sound.h"

#include "cpctelera_wrap.h"
#include <types.h>

#include "teopl_util.h"
#include "teopl_logic.h"
#include "teopl_billboard.h"
#include "teopl_interrupt.h"
#include "teopl_pool.h"
#include "teopl_level.h"

#ifndef WINCPCTELERA

#include "teopl_pool.h"

#include "assets/songs/song_intro.h"
#include "assets/songs/song_dialog.h"
#include "assets/songs/song_gameplay.h"
#include "assets/songs/song_boss.h"
#include "assets/songs/song_ending.h"
#include "assets/songs/song_mute.h"

#include "exomizer/exomizer.h"

extern u16      SFX_MAIN_SOUNDEFFECTS;

#endif

// REMARK: decrease duration by this value to allow early interruption
#define TEOPL_SOUND_SFX_DURATION_FADEOUT_MS 0

static u32  _time_sfx_set;

// TABLE 2
const u16 _sfx_duration[] = {
0,

// speed        last line index
 ((1    + 1) * (0x08            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x13            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x0F            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x05            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x07            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x04            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x0B            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((1    + 1) * (0x04            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x2C            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((2    + 1) * (0x3E            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((0    + 1) * (0x09            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10,
 ((2    + 1) * (0x11            + 1) * 20 - TEOPL_SOUND_SFX_DURATION_FADEOUT_MS) * 3 / 10
};

// ---

u8                      g_teopl_sound_initialized;

u8                      g_teopl_sound_muted_song;
u8                      g_teopl_sound_muted_sfx;

void*                   g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_NUM];
u8                      g_teopl_sound_song_index;

u8                      g_teopl_sound_sfx_index;
u8                      g_teopl_sound_sfx_channel;
u8                      g_teopl_sound_sfx_priority;

// ---

#ifndef WINCPCTELERA
#ifndef ENTERPRISE
// data     = address of a sfx
static void _sfx_init(void* data) __z88dk_callee __naked {

    // get params from stack
    __asm__("pop iy");  // ret address first because __z88dk_callee convention
    __asm__("pop hl");
    __asm__("push iy"); // ret address last  because __z88dk_callee convention
    __asm__("call _PLY_LW_INITSOUNDEFFECTS");

    __asm__("ret");
}

// REMARK: channel and volume are merged to one 2-byte variable
// ---
// channel  = channel (0 - 2)
// volume   = inverted volume (0 - 16)
// sfx      = sound effect number (>= 1)
static void _sfx_play(u16 channel_and_volume, u16 sfx) __z88dk_callee __naked {

    // get params from stack
    __asm__("pop iy");  // ret address first because __z88dk_callee convention
    __asm__("pop bc");
    __asm__("pop hl");
    __asm__("push iy"); // ret address last  because __z88dk_callee convention
    __asm__("ld a, l"); // ; Sound effect number(>= 1)
    __asm__("call _PLY_LW_PLAYSOUNDEFFECT");

    __asm__("ret");
}

// channel  = channel (0 - 2)
void _sfx_stop(u16 channel) __z88dk_callee __naked {

    // get params from stack    
    __asm__("pop iy");  // ret address first because __z88dk_callee convention
    __asm__("pop hl");
    __asm__("ld a, l");
    __asm__("push iy"); // ret address last  because __z88dk_callee convention

    __asm__("call _PLY_LW_STOPSOUNDEFFECTFROMCHANNEL");

    __asm__("ret");
}

// data     = address of a song
static void _song_init(void* data, u16 subsong) __z88dk_callee __naked {

    // get params from stack
    __asm__("pop iy");  // ret address first because __z88dk_callee convention
    __asm__("pop hl");
    __asm__("pop bc");
    __asm__("push iy"); // ret address last  because __z88dk_callee convention
    __asm__("ld a, c"); // subsong index (>=0)
    __asm__("call _PLY_LW_INIT");

    __asm__("ret");
}
#endif // (not) ENTERPRISE
#endif // (not) WINCPCTELERA

// ---

u8 teopl_sound_song_file_index_get() {

    TeoplLogicState state = teopl_logic_state_get();

    // ---

    g_teopl_sound_muted_sfx = 0;

    if (g_teopl_sound_muted_song) {
        return TEOPL_SOUND_FILE_SONG_MUTE;
    }
    if (state == TEOPL_LOGIC_STATE_GAME_TITLE_MENU) {
        return TEOPL_SOUND_FILE_SONG_INTRO;
    }
    if (state == TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD) {
        return TEOPL_SOUND_FILE_SONG_INTRO;
    }
    if (state == TEOPL_LOGIC_STATE_LEVEL_WELCOME) {
        return TEOPL_SOUND_FILE_SONG_DIALOG;
    }
    if (state == TEOPL_LOGIC_STATE_LEVEL_PLAYABLE) {
        if (teopl_level_is_ending()) {
            //g_teopl_sound_muted_sfx = 1;
            return TEOPL_SOUND_FILE_SONG_ENDING;
        }
        else if (g_teopl_level_data.level_special) {
            g_teopl_sound_muted_sfx = 0;
            return TEOPL_SOUND_FILE_SONG_BOSS;
        }
        return TEOPL_SOUND_FILE_SONG_MAIN;
    }

    return TEOPL_SOUND_FILE_SONG_MUTE;
}

void teopl_sound_init() {

#ifndef WINCPCTELERA

    g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_INTRO] = song_intro;
    g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_DIALOG] = song_dialog;
    g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_MAIN] = song_gameplay;
    g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_BOSS] = song_boss;
    g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_ENDING] = song_ending;
    g_teopl_sound_songs[TEOPL_SOUND_FILE_SONG_MUTE] = song_mute;

#endif

    _time_sfx_set = 0;

    // ---

    // one time init
    if (!g_teopl_sound_initialized) {

        g_teopl_sound_muted_song = 0;
        g_teopl_sound_muted_sfx = 0;
    }

    g_teopl_sound_song_index = TEOPL_SOUND_FILE_SONG_NUM;

    g_teopl_sound_initialized = 1;

    // ---

    g_teopl_sound_sfx_index = TEOPL_SOUND_SFX_NONE;
    g_teopl_sound_sfx_channel = 2;
    g_teopl_sound_sfx_priority = 0;

#ifndef WINCPCTELERA
#ifndef ENTERPRISE
    _sfx_init(&SFX_MAIN_SOUNDEFFECTS);
#else
    cpct_akpLW_SFXInit(&SFX_MAIN_SOUNDEFFECTS);
#endif
#endif
}

void teopl_sound_song_init(u8 index, u8 force) {

    if (g_teopl_sound_song_index != index || force) {
        g_teopl_sound_song_index = index;

#ifndef WINCPCTELERA

        g_teopl_pool_song_extracted = 0;

        teopl_pool_extract(TEOPL_POOL_TYPE_SONG, 0);

#ifndef ENTERPRISE
        _song_init(g_teopl_pool_anything, 0);
#else
        cpct_akpLW_musicInit(g_teopl_pool_anything, 0);
#endif

#endif
    }
}

void teopl_sound_mute_song_toggle() {

    teopl_interrupt_and_sound(0);

    g_teopl_sound_muted_song = g_teopl_sound_muted_song ? 0 : 1;
}

static void _sfx_set(TeoplSoundSfxs sfx_index, u8 volume) {

    u16 channel_and_volume;
    u16 index = (u16)(sfx_index);

    // ---

    g_teopl_sound_sfx_index = sfx_index;

    _time_sfx_set = g_teopl_interrupt_ticks_total;

    channel_and_volume = ((u16)volume << 8) | g_teopl_sound_sfx_channel;

#ifndef ENTERPRISE
    _sfx_play(channel_and_volume, index);
#else
    cpct_akpLW_SFXPlay(index, g_teopl_sound_sfx_channel, volume);
#endif
}

// REMARK: stops both music and sfx
void teopl_sound_stop() {

#ifndef ENTERPRISE
    _sfx_stop(g_teopl_sound_sfx_channel);
    __asm__("call _PLY_LW_STOP");
#else
    cpct_akpLW_SFXStop(g_teopl_sound_sfx_channel);
    cpct_akpLW_stop();
#endif
}

void teopl_sound_sfx_set_with_priority_default(TeoplSoundSfxs sfx_index) {

    teopl_sound_sfx_set_with_priority(sfx_index, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 0, 0);
}

void teopl_sound_sfx_set_with_priority(TeoplSoundSfxs sfx_index, u8 priority, u8 volume, u8 interrupt_same_priority, u8 only_when_music_muted) {

    u8 play = 0;
    u8 sfx_finished = g_teopl_interrupt_ticks_total - _time_sfx_set > _sfx_duration[g_teopl_sound_sfx_index];

    // ---

    if (g_teopl_sound_muted_sfx) {
        return;
    }
    
    if (only_when_music_muted && !g_teopl_sound_muted_song) {
        return;
    }

    // priority higher
    if (priority > g_teopl_sound_sfx_priority) {

        play = 1;
    }
    // priority same
    else if (priority == g_teopl_sound_sfx_priority) {
        if (sfx_finished || interrupt_same_priority) {
            play = 1;
        }
    }
    else if (sfx_finished) {
        play = 1;
    }

    // ---

    if (play) {
        g_teopl_sound_sfx_priority = priority;
        _sfx_set(sfx_index, volume);
    }
}

// ---

void teopl_sound_play() {
#ifndef WINCPCTELERA
#ifndef ENTERPRISE
    __asm__("call _PLY_LW_PLAY");
#else
    cpct_akpLW_musicPlay();
#endif
#endif
}
