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

#ifndef _TEOPL_POOL_H_
#define _TEOPL_POOL_H_

#include "cpctelera_wrap.h"

#include <types.h>

#include "teopl_ringbuffer.h"
#include "teopl_entity.h"
#include "teopl_util.h"

#include "assets_pictures_talk.h"
#include "assets_texts.h"

#define TEOPL_POOL_MAX_NUM_ENTITIES 16

// REMARK: don't remove two spaces after define - it's used to detect non wincpctelera buffer size !!!
#ifdef WINCPCTELERA
#define TEOPL_POOL_MAX_ANYTHING 16000
#else
#define  TEOPL_POOL_MAX_ANYTHING 1797
#endif

// ---

typedef enum {

    TEOPL_POOL_RINGBUF_TYPE_ENTITIES

} TeoplPoolRingbufType;

typedef enum {

    // decompress directly to level data
    TEOPL_POOL_TYPE_LEVELS,

    // largest, but temp decompression to pool - done on level load
    TEOPL_POOL_TYPE_TEXTS,

    // during gameplay, song and talk pictures should be both decompressed in pool on different placess
    TEOPL_POOL_TYPE_SONG,
    TEOPL_POOL_TYPE_PICTURES_TALK

} TeoplPoolType;

// ---

extern u8               g_teopl_pool_pictures_talk_extracted;
extern u8               g_teopl_pool_levels_extracted;
extern u8               g_teopl_pool_song_extracted;
extern u8               g_teopl_pool_levels_texts_extracted;

extern u8               g_teopl_pool_anything[];

// ---

void                    teopl_pool_init_type(TeoplPoolRingbufType type);
void                    teopl_pool_init();

TeoplEntity*            teopl_pool_entity_new();
void                    teopl_pool_entity_delete(TeoplEntity* object);

void                    teopl_pool_flags_reset();
void                    teopl_pool_extract(TeoplPoolType type, u8 data);

#endif
