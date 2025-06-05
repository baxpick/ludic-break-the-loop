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

#include "cpctelera_wrap.h"

#include "teopl_pool.h"

#ifndef WINCPCTELERA
#include "exomizer/exomizer.h"
#endif

#include "teopl_video.h"
#include "teopl_ringbuffer.h"
#include "teopl_level.h"
#include "teopl_memory.h"
#include "teopl_sound.h"
#include "teopl_interrupt.h"

#include "assets_texts.h"

// ---

// general
 // FIXME: MAX_NUM_BYTES_FOR_DECOMPRESSED_ANYTHING, some texts... not counted in, they may overflow
u8                      g_teopl_pool_pictures_talk_extracted;
u8                      g_teopl_pool_levels_extracted;
u8                      g_teopl_pool_song_extracted;
u8                      g_teopl_pool_levels_texts_extracted;

// ---

// entities
static TeoplRingBuffer  _entities_free;
static TeoplEntity      _pool_entities[TEOPL_POOL_MAX_NUM_ENTITIES];

// ---

void teopl_pool_init() {

    g_teopl_pool_pictures_talk_extracted = 0;
    g_teopl_pool_levels_extracted = 0;
    g_teopl_pool_song_extracted = 0;
    g_teopl_pool_levels_texts_extracted = 0;

    teopl_pool_init_type(TEOPL_POOL_RINGBUF_TYPE_ENTITIES);
}

void teopl_pool_init_type(TeoplPoolRingbufType type) {

    u8 i;

    // ---

    switch (type) {
    case TEOPL_POOL_RINGBUF_TYPE_ENTITIES:
        teopl_ringbuffer_init(&_entities_free);

        for (i = 0; i < TEOPL_POOL_MAX_NUM_ENTITIES; ++i) {
            teopl_ringbuffer_add(&_entities_free, (TEOPL_MEMORY_POINTER_TYPE)&(_pool_entities[i]));
        }
        break;
    }
}

TeoplEntity* teopl_pool_entity_new() {

    TEOPL_MEMORY_POINTER_TYPE output;

    // ---

    if (teopl_ringbuffer_pull(&_entities_free, &output)) {
        return (TeoplEntity*)output;
    }

    return 0;
}

void teopl_pool_entity_delete(TeoplEntity* entity) {

    teopl_ringbuffer_add(&_entities_free, (TEOPL_MEMORY_POINTER_TYPE)entity);

    entity->type = TEOPL_ENTITY_TYPE_NONE; // invalidate
}

void teopl_pool_flags_reset() {

    // REMARK: when adding flag here, check if needs to be reseted in extract function
    g_teopl_pool_pictures_talk_extracted = 0;
    g_teopl_pool_levels_extracted = 0;
    g_teopl_pool_song_extracted = 0;
    g_teopl_pool_levels_texts_extracted = 0;
}

// REMARK: data is used as additional info, like the level index in case od text descompressing
void teopl_pool_extract(TeoplPoolType type, u8 data) {

    switch (type) {

    case TEOPL_POOL_TYPE_LEVELS:
    {
        u8* level_internal = TEOPL_DEFINES_POOL_LEVEL;

        TeoplLevelData* ptr = &g_teopl_level_data;
        u8              index = ptr->level_index - (ptr->level_index / TEOPL_DEFINES_LEVEL_SPECIAL_FREQUENCY); // REMARK: since every 3rd level is generated

        u16 zip_size = (u16)(g_levelset_local_map[index * 3]);
        u8* zip_e = (u8*)(g_levelset_local_map[index * 3 + 1]);

        u16 unzip_size = (u16)(g_levelset_local_map[index * 3 + 2]);
        u8* unzip_e = (level_internal + unzip_size - 1);

        // REMARK: reset the flags for resources which share *same parts* of *same temp space*
        //g_teopl_pool_pictures_talk_extracted = 0;
        //g_teopl_pool_levels_extracted = 0;
        //g_teopl_pool_song_extracted = 0;
        //g_teopl_pool_levels_texts_extracted = 0;

        if (g_teopl_pool_levels_extracted == 0) {

            g_teopl_pool_levels_extracted = 1;

#ifdef WINCPCTELERA
            teopl_util_decompress_zx7(zip_e, zip_size, unzip_e, unzip_size);
#else
            teopl_util_decompress_zx7(zip_e, unzip_e);
#endif
        }
        break;
    }

    case TEOPL_POOL_TYPE_TEXTS:

        teopl_interrupt_and_sound(0);

        // REMARK: reset the flags for resources which share *same parts* of *same temp space*
        g_teopl_pool_pictures_talk_extracted = 0;
        //g_teopl_pool_levels_extracted = 0;
        g_teopl_pool_song_extracted = 0;
        //g_teopl_pool_levels_texts_extracted = 0;

        if (g_teopl_pool_levels_texts_extracted == 0) {

            g_teopl_pool_levels_texts_extracted = 1;

            cpct_memset(g_teopl_pool_anything, 0, TEOPL_POOL_MAX_ANYTHING);

#ifdef WINCPCTELERA
            teopl_util_decompress_zx7(texts_levels_end, texts_levels_size_z,
                g_teopl_pool_anything + texts_levels_size - 1,
                texts_levels_size
            );
#else
            decompress(texts_levels, g_teopl_pool_anything);
#endif
            g_teopl_pool_anything[texts_levels_size] = 0; // mark ending (level indexes start from 1, so 0 can be used as marker)

            // locate only text for selected level and copy all encoded texts to level data
            teopl_level_load_texts_all(g_teopl_pool_anything, data);
        }

        break;

    case TEOPL_POOL_TYPE_SONG:
        //g_teopl_pool_pictures_talk_extracted = 0;
        //g_teopl_pool_levels_extracted = 0;
        //g_teopl_pool_song_extracted = 0;
        //g_teopl_pool_tcpc_extracted = 0;
        g_teopl_pool_levels_texts_extracted = 0;

        if (g_teopl_pool_song_extracted == 0) {

            g_teopl_pool_song_extracted = 1;

#ifdef WINCPCTELERA

#else
            decompress(g_teopl_sound_songs[g_teopl_sound_song_index], g_teopl_pool_anything);
#endif
        }
        break;

        // REMARK: trick to extract only one type of talk animations: 2 AI or 2 LUDIC frames
        // variable g_teopl_pool_pictures_talk_extracted will be:
        // 0 - none extracted
        // 1 - ai extracted
        // 2 - ludic extracted
    case TEOPL_POOL_TYPE_PICTURES_TALK:

        //g_teopl_pool_pictures_talk_extracted = 0;
        //g_teopl_pool_levels_extracted = 0;
        //g_teopl_pool_song_extracted = 0;
        g_teopl_pool_levels_texts_extracted = 0;

        if (g_teopl_pool_pictures_talk_extracted != data) {

            g_teopl_pool_pictures_talk_extracted = data;

#ifdef WINCPCTELERA
            if (data == TEOPL_BILLBOARD_PICTURE_TYPE_AI) {
                teopl_util_decompress_zx7(ai_talk_01_end, ai_talk_01_size_z,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1,
                    ai_talk_01_size
                );
                teopl_util_decompress_zx7(ai_talk_00_end, ai_talk_00_size_z,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1 - ai_talk_01_size,
                    ai_talk_00_size
                );
            }
            else if (data == TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC) {
                teopl_util_decompress_zx7(ludic_talk_01_end, ludic_talk_01_size_z,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1,
                    ludic_talk_01_size
                );
                teopl_util_decompress_zx7(ludic_talk_00_end, ludic_talk_00_size_z,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1 - ludic_talk_01_size,
                    ludic_talk_00_size
                );
            }
#else
            if (data == TEOPL_BILLBOARD_PICTURE_TYPE_AI) {
                teopl_util_decompress_zx7(ai_talk_01_end,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1
                );
                teopl_util_decompress_zx7(ai_talk_00_end,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1 - ai_talk_01_size
                );
            }
            else if (data == TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC) {
                teopl_util_decompress_zx7(ludic_talk_01_end,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1
                );
                teopl_util_decompress_zx7(ludic_talk_00_end,
                    g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - 1 - ludic_talk_01_size
                );
            }
#endif
        }
        break;
    }
}
