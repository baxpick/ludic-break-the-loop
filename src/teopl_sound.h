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

#ifndef _TEOPL_SOUND_H_
#define _TEOPL_SOUND_H_

#include "cpctelera_wrap.h"

#include <types.h>

#define TEOPL_SOUND_LARGEST_SONG 720

#define TEOPL_SOUND_SFX_PRIORITY_LOW        0
#define TEOPL_SOUND_SFX_PRIORITY_DEFAULT    1
#define TEOPL_SOUND_SFX_PRIORITY_HIGH       2

#define TEOPL_SOUND_SFX_VOLUME_DEFAULT      2

// song files
enum {

    TEOPL_SOUND_FILE_SONG_INTRO = 0,
    TEOPL_SOUND_FILE_SONG_DIALOG,
    TEOPL_SOUND_FILE_SONG_MAIN,
    TEOPL_SOUND_FILE_SONG_BOSS,
    TEOPL_SOUND_FILE_SONG_ENDING,
    TEOPL_SOUND_FILE_SONG_MUTE,

    // ---

    TEOPL_SOUND_FILE_SONG_NUM
};

// sfxs indexes
typedef enum {

    TEOPL_SOUND_SFX_NONE                    = 0,

    TEOPL_SOUND_FILE_SFX_MAIN_01            = 0x01,
    TEOPL_SOUND_FILE_SFX_MAIN_02            = 0x02,
    TEOPL_SOUND_FILE_SFX_MAIN_03            = 0x03,
    TEOPL_SOUND_FILE_SFX_MAIN_04            = 0x04,
    TEOPL_SOUND_FILE_SFX_MAIN_05            = 0x05,
    TEOPL_SOUND_FILE_SFX_MAIN_06            = 0x06,
    TEOPL_SOUND_FILE_SFX_MAIN_07            = 0x07,
    TEOPL_SOUND_FILE_SFX_MAIN_08            = 0x08,
    TEOPL_SOUND_FILE_SFX_MAIN_09            = 0x09,
    TEOPL_SOUND_FILE_SFX_MAIN_0A            = 0x0A,
    TEOPL_SOUND_FILE_SFX_MAIN_0B            = 0x0B,
    TEOPL_SOUND_FILE_SFX_MAIN_0C            = 0x0C,

    TEOPL_SOUND_SFX_NUM

} TeoplSoundSfxs;

// ---

extern u8       g_teopl_sound_initialized;

extern u8       g_teopl_sound_muted_song;
extern u8       g_teopl_sound_muted_sfx;

extern void*    g_teopl_sound_songs[];
extern u8       g_teopl_sound_song_index;

extern u8       g_teopl_sound_sfx_index;
extern u8       g_teopl_sound_sfx_channel;

// ---

void    teopl_sound_init();

u8      teopl_sound_song_file_index_get();

void    teopl_sound_song_init(u8 index, u8 force);

void    teopl_sound_play();
void    teopl_sound_stop();
void    teopl_sound_sfx_set_with_priority_default(TeoplSoundSfxs sfx_index);
void    teopl_sound_sfx_set_with_priority(TeoplSoundSfxs sfx_index, u8 priority, u8 volume, u8 interrupt_same_priority, u8 only_when_music_muted);

void    teopl_sound_mute_song_toggle();

#endif
