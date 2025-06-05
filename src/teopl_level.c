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

#include <types.h>

#include "teopl_level.h"

#include "teopl_pool.h"
#include "teopl_memory.h"
#include "teopl_ringbuffer.h"
#include "teopl_video.h"
#include "teopl_logic.h"
#include "teopl_sound.h"
#include "teopl_font.h"
#include "teopl_util.h"
#include "teopl_interrupt.h"
#include "teopl_events.h"
#include "teopl_defines.h"
#include "teopl_collision.h"

#include "assets/sprites/font.h"

#include "assets_texts.h"

extern u16 g_main_num_chips_total;

extern u8 g_teopl_main_loop_game_exit;
extern u8 g_teopl_main_pill;

static TeoplLevelData* const ptr = &g_teopl_level_data;

static u8 percent_block, percent_plant, percent_captured;

// REMARK: don't start with zero, password string should be 7 characters fixed
static const u32 _passwords[] = {
    0,          //         (1) // REMARK: do not change password for first level!
    48152798,   // 2DEC0DE (2)
    62980078,   // 3C0FFEE (3)
    218415326,  // D04C0DE (4)
    262974949,  // FACADE5 (5)
    195082934,  // BA0BAB6 (6)
    199740158,  // BE7CAFE (7)
    182291627,  // ADD8CAB (8)
    202115593,  // C0C0A09 (9)
    31378142,   // 1DECADE (10)
    233754641,  // DEED011 (11)
    251711506,  // F00D012 (12)
    181452307,  // AD0BE13 (13)
    21683693,   // 14ADDED (14)
    78762005,   // 4B1D015 (15)
    84258901,   // 505B055 (16) // ending
};

// ---

u8              g_teopl_level_exit_activated_last_x_b;
u8              g_teopl_level_exit_activated_last_y_px;

TeoplLevelData  g_teopl_level_data;
char            g_teopl_level_password[TEOPL_LEVEL_PASSWORD_LENGTH + 1];

// ---

static void _teopl_level_load_prepare(
    u8 level_index,
    u8 levels_num,
    u8 x_b, u8 y_px
) {
    ptr->level_index = level_index;
    ptr->levels_num = levels_num;
    ptr->level_x_b = x_b;
    ptr->level_y_px = y_px;
}

static void _teopl_level_init_tiles() {

    ptr->loaded_tiles = 0;

    // ---

    ptr->level_tiles = 0;
    ptr->level_size = 0;
    ptr->levels_num = 0;
    ptr->level_index = TEOPL_LEVEL_INDEX_UNINITIALIZED;
    ptr->level_special = 0;
    ptr->level_special_num_chips_total = 0;
    ptr->level_special_num_chips_destroyed = 0;
    ptr->level_x_b = 0;
    ptr->level_y_px = 0;
    ptr->level_width = 0;
    ptr->level_height = 0;
    ptr->level_frame_right_b = 0;
    ptr->level_frame_down_px = 0;
}

static void _teopl_level_init_texts() {

    ptr->text_welcome_next_index = 0;

    // ---

    ptr->texts_info_loaded = 0;
    ptr->texts_num = 0;
    ptr->texts_num_welcome = 0;
}

static void _teopl_level_init_entities() {

    ptr->loaded_entities = 0;

    // ---

    teopl_pool_init_type(TEOPL_POOL_RINGBUF_TYPE_ENTITIES);

    teopl_ringbuffer_init(&(ptr->entites));
    teopl_ringbuffer_init(&(ptr->tiles_playable));
    teopl_ringbuffer_init(&(ptr->tiles_ai));

    ptr->entity_player = 0;
    ptr->entity_ai = 0;
}

// ---

u8 teopl_level_is_ending() {

    return g_teopl_level_data.level_index == g_teopl_level_data.levels_num - 1;
}

void teopl_level_remove_chip_from_inventory(u8 context) {

    teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_chip_removing_from_inventory, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

    // 1. chip gone
    teopl_level_entity_create(
        TEOPL_ENTITY_DRAW_TYPE_NONMASKED,
        TEOPL_ENTITY_TYPE_INVENTORY_REMOVING,
        TEOPL_DEFINES_LEVEL_INVENTORY2_X_B, 3 * font_HEIGHT_PX + (ptr->level_special_num_chips_destroyed - 1) * (TEOPL_DEFINES_LEVEL_TILE_H_PX + 2),
        TEOPL_DEFINES_LEVEL_TILE_W_B, TEOPL_DEFINES_LEVEL_TILE_H_PX,
        g_blocks + TEOPL_LEVEL_TILE_POOF * blocks_BYTES_PER_SPRITE,
        TEOPL_ENTITY_DIRECTION_TYPE_NONE, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC,
        (TEOPL_MEMORY_POINTER_TYPE)TEOPL_LEVEL_TILE_CHIP_INACTIVE,
        (TEOPL_MEMORY_POINTER_TYPE)context
    );

    ptr->level_special_num_chips_destroyed--;

    if (!context) { // 0 means chip is used, so total should be adjusted also
        g_teopl_level_data.level_special_num_chips_total--;
    }
}

u8 teopl_level_tile_type_is_block(TeoplLevelTileType tile_type) {
    return (tile_type >= TEOPL_LEVEL_TILE_BLOCK_01 && tile_type <= TEOPL_LEVEL_TILE_BLOCK_08);
}

u8 teopl_level_tile_is_block(u8* tile) {
    return teopl_level_tile_type_is_block(*tile);
}

void teopl_level_draw_entities() {

    u8 i, num;
    TEOPL_MEMORY_POINTER_TYPE output;
    TeoplEntity* entity;

    num = g_teopl_level_data.entites.num_elements;
    for (i = 0; i < num; ++i) {

        teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output);
        entity = (TeoplEntity*)output;

        //if (entity->type != TEOPL_ENTITY_TYPE_NONE) {
        teopl_entity_draw(entity);
        //}
    }
}

void teopl_level_restart() {

    u8 level_index = ptr->level_index;

    // ---

    // internet level => try to reload level, else exit to main menu
    if (level_index == 255) {

        teopl_level_init();

        /* FIXXXME: hotfix when killed in internet level

        if (teopl_level_index_get(g_teopl_level_password) == 255) {

            teopl_level_load_all(255, 1);
        }
        else {*/

            g_teopl_main_loop_game_exit = 1;
        //}
    }
    // regular level => just reload
    else {
        teopl_level_load_all(level_index, 0);
    }
}

u8 teopl_level_index_get(char* password) {

    u8 level_index = 0;
    u8 i;
    u8 str_len = 0;
    u32 code_candidate = teopl_util_string_to_u32(password, &str_len, 1);

    u8 num = sizeof(_passwords) / sizeof(u32);

    // ---

    // 2. then try local level
    if (code_candidate == 0) {
        return 0;
    }

    for (i = 0; i < num; ++i) {
        if (code_candidate == _passwords[i]) {
            level_index = i;
            break;
        }
    }

    return level_index;
}

void teopl_level_password_get(u8 index, char* password) {

    u8 len;

    cpct_memset(password, 0, TEOPL_LEVEL_PASSWORD_LENGTH + 1);
    teopl_util_u32_to_string(password, &len, _passwords[index], 1);
}

void teopl_level_init() {

    g_teopl_level_exit_activated_last_x_b = 0;
    g_teopl_level_exit_activated_last_y_px = 0;

    levelset_local_map_fill(); // FIXME: remove once release is ready

    _teopl_level_init_tiles();
    _teopl_level_init_texts();
    _teopl_level_init_entities();

    // REMARK: this is not set for internet levels?
    _teopl_level_load_prepare(
        0, TEOPL_LEVELSET_LOCAL_NUM_LEVELS + (TEOPL_LEVELSET_LOCAL_NUM_LEVELS / (TEOPL_DEFINES_LEVEL_SPECIAL_FREQUENCY - 1)),
        0, 0
    );

    // ---

    ptr->keys_num = 0;
}

void teopl_level_entity_create(
    TeoplEntityDrawType draw_type,
    u8 type,
    u8 x_b, u8 y_px, u8 w_b, u8 h_px, const u8* sprite,
    TeoplEntityDirection move_direction, TeoplEntityAnimationType force_anim_type,
    TEOPL_MEMORY_POINTER_TYPE data1, TEOPL_MEMORY_POINTER_TYPE data2) {

    TeoplEntity* entity = teopl_pool_entity_new();

    // ---

    entity->type = type;

    // draw
    entity->draw_type = draw_type;
    entity->x_b = x_b;
    entity->y_px = y_px;
    entity->x_b_prev = x_b;
    entity->y_px_prev = y_px;
    //entity->w_b = w_b;
    //entity->h_px = h_px;
    entity->sprite = (u8*)sprite;

    // direction and animation
    teopl_entity_sprite_direction_and_animation_set(entity, move_direction, force_anim_type);

    // special
    entity->data1 = data1;
    entity->data2 = data2;

    // ---

    teopl_level_entity_add(entity);
}

u8 teopl_level_coord_aligned_x_b_absolute(u8 x_b) {
    return ((x_b - ptr->level_x_b) % TEOPL_DEFINES_LEVEL_TILE_W_B == 0) ? 1 : 0;
}

u8 teopl_level_coord_aligned_y_px_absolute(u8 y_px) {
    return ((y_px - ptr->level_y_px) % TEOPL_DEFINES_LEVEL_TILE_H_PX == 0) ? 1 : 0;
}

u8 teopl_level_coord_aligned_absolute(u8 x_b, u8 y_px) {
    return (teopl_level_coord_aligned_x_b_absolute(x_b) && teopl_level_coord_aligned_y_px_absolute(y_px)) ? 1 : 0;
}

u8* teopl_level_tile_get(u8 x, u8 y) {
    return &(ptr->level_tiles[ptr->level_width * y + x]);
}

u8* teopl_level_tile_get_from_level(u8* level, u8 x, u8 y, u8 level_width) {
    return (level + level_width * y + x);
}

// coord tile
u8 teopl_level_tile_x_get(u8* tile) {
    return (tile - ptr->level_tiles) % ptr->level_width;
}
u8 teopl_level_tile_y_get(u8* tile) {
    return (tile - ptr->level_tiles) / ptr->level_width;
}

u8* teopl_level_tile_get_from_coord_aligned_absolute(u8 x_b, u8 y_px) {

    // no tile if not aligned
    if (!teopl_level_coord_aligned_absolute(x_b, y_px)) {
        return 0;
    }

    return teopl_level_tile_get((x_b - ptr->level_x_b) / TEOPL_DEFINES_LEVEL_TILE_W_B, (y_px - ptr->level_y_px) / TEOPL_DEFINES_LEVEL_TILE_H_PX);
}

TeoplEntity* teopl_level_entity_get_from_coord_and_direction_absolute(TeoplEntity* entity_skip, u8 x_b, u8 y_px, TeoplEntityDirection move_direction) {

    u8 i, num = ptr->entites.num_elements;
    TEOPL_MEMORY_POINTER_TYPE output;
    TeoplEntity* entity_hit_candidate = 0;

    u8 e2_x_b, e2_y_px;

    // ---

    if (!ptr->loaded_entities) {
        return 0;
    }

    for (i = 0; i < num; ++i) {

        teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output);
        entity_hit_candidate = (TeoplEntity*)output;

        if (entity_hit_candidate == entity_skip) {
            continue;
        }

        e2_x_b = entity_hit_candidate->x_b;
        e2_y_px = entity_hit_candidate->y_px;

        if (teopl_collision_check_entity_vs_entity(x_b, y_px, e2_x_b, e2_y_px)) {

            return entity_hit_candidate;
        }
    }

    return 0;
}

u8* teopl_level_tile_get_from_coord_and_direction_absolute(u8 x_b, u8 y_px, TeoplEntityDirection move_direction) {

    u8* tile = 0;

    // ---

    if (!ptr->loaded_tiles) {
        return 0;
    }

    // ---

    // if aligned
    tile = teopl_level_tile_get_from_coord_aligned_absolute(x_b, y_px);
    if (tile) {
        return tile;
    }

    if (teopl_level_coord_aligned_y_px_absolute(y_px)) {
        if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT) {
            return teopl_level_tile_get((x_b - ptr->level_x_b) / TEOPL_DEFINES_LEVEL_TILE_W_B, (y_px - ptr->level_y_px) / TEOPL_DEFINES_LEVEL_TILE_H_PX);
        }
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
            return teopl_level_tile_get((x_b - ptr->level_x_b) / TEOPL_DEFINES_LEVEL_TILE_W_B + 1, (y_px - ptr->level_y_px) / TEOPL_DEFINES_LEVEL_TILE_H_PX);
        }
    }

    if (teopl_level_coord_aligned_x_b_absolute(x_b)) {
        if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
            return teopl_level_tile_get((x_b - ptr->level_x_b) / TEOPL_DEFINES_LEVEL_TILE_W_B, (y_px - ptr->level_y_px) / TEOPL_DEFINES_LEVEL_TILE_H_PX);
        }
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
            return teopl_level_tile_get((x_b - ptr->level_x_b) / TEOPL_DEFINES_LEVEL_TILE_W_B, (y_px - ptr->level_y_px) / TEOPL_DEFINES_LEVEL_TILE_H_PX + 1);
        }
    }

    return 0;
}

// coord screen
u8 teopl_level_tile_x_b_get(u8* tile) {
    return ptr->level_x_b + teopl_level_tile_x_get(tile) * TEOPL_DEFINES_LEVEL_TILE_W_B;
}
u8 teopl_level_tile_y_px_get(u8* tile) {
    return ptr->level_y_px + teopl_level_tile_y_get(tile) * TEOPL_DEFINES_LEVEL_TILE_H_PX;
}

void teopl_level_entity_add(TeoplEntity* entity) {

    if (entity->type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE) {
        if (ptr->entity_player == 0) {
            ptr->entity_player = entity;
        }
    }

    // REMARK: last enemy entity will be current active enemy
    if (entity->type == TEOPL_ENTITY_TYPE_ENEMY) {
        ptr->entity_ai = entity;
    }

    teopl_ringbuffer_add(&(ptr->entites), (TEOPL_MEMORY_POINTER_TYPE)entity);
}

void teopl_level_entity_remove(TeoplEntity* entity) {

    if (entity->type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE) {
        if (ptr->entity_player == entity) {
            ptr->entity_player = 0;
        }
    }

    if (entity->type == TEOPL_ENTITY_TYPE_ENEMY) {
        if (ptr->entity_ai == entity) {
            ptr->entity_ai = 0;
        }
    }

    teopl_ringbuffer_remove(&(g_teopl_level_data.entites), (TEOPL_MEMORY_POINTER_TYPE)entity);

    teopl_pool_entity_delete(entity);
}

void teopl_level_entities_extract(u8 x, u8 y, u8 width, u8 height) {

    u8* tile;
    u8  tile_value;
    u8  right = x + width;
    u8  bottom = y + height;

    TeoplEntityType entity_type;

    // ---

    if (!ptr->loaded_entities) {

        for (u8 row = x; row < bottom; ++row) {

            for (u8 col = y; col < right; ++col) {

                tile = teopl_level_tile_get(col, row);
                tile_value = *tile;

                if (tile_value == TEOPL_LEVEL_TILE_EMPTY) {
                    continue;
                }

                // special tile actions
                // ---

                // add special tiles (inactive player)
                if (tile_value == TEOPL_LEVEL_TILE_PLAYER_INACTIVE) {
                    teopl_ringbuffer_add(&(ptr->tiles_playable), (TEOPL_MEMORY_POINTER_TYPE)tile);
                    continue;
                }

                // add special tiles (enemy - ai)
                if (tile_value == TEOPL_LEVEL_TILE_ENEMY_DOWN) {
                    teopl_ringbuffer_add(&(ptr->tiles_ai), (TEOPL_MEMORY_POINTER_TYPE)tile);
                    continue;
                }

                // count number of chips to be destroyed
                if (tile_value == TEOPL_LEVEL_TILE_CHIP_INACTIVE) {
                    ptr->level_special_num_chips_total++;
                }

                // add entities
                // ---

                if (tile_value == TEOPL_LEVEL_TILE_PLAYER_ACTIVE_DOWN) {
                    entity_type = TEOPL_ENTITY_TYPE_PLAYER_ACTIVE;
                }
                else {
                    entity_type = TEOPL_ENTITY_TYPE_NONE;
                }

                if (entity_type != TEOPL_ENTITY_TYPE_NONE) {
                    teopl_level_tile_to_entity(TEOPL_ENTITY_DRAW_TYPE_MASKED, tile, entity_type, tile_value, TEOPL_ENTITY_DIRECTION_TYPE_NONE, 0, 0);
                }
            }
        }

        ptr->loaded_entities = 1;
    }
}

static void _generate_tile(TeoplLevelTileType tile_type, u8* level, u16 level_size) {

    u32 r;

    // ---

    r = teopl_util_random32(level_size);
    while (*(level + r) != TEOPL_LEVEL_TILE_EMPTY) {
        r = (r + 1) % level_size;
    }

    *(level + r) = tile_type;
}

void teopl_level_load_tiles(u8 index) {

    u8  w, h;
    u16 size;
    u8* level_internal = TEOPL_DEFINES_POOL_LEVEL;

    // ---

    if (!ptr->loaded_tiles || ptr->level_index != index) {

        u8 dimensions_encoded = 0;

        // ---

        ptr->level_index = index;

        // 0. internet level => data is already loaded/extracted
        if (index != 255) {

            // 1. special level => generates
            if (ptr->level_special) {

                // FIXME: generated parameters are valid only for this amount of special levels (6)
                u8  i;
                u8  w;
                u8  h;
                u16 size;
                u8  num_chips;
                u8  num_enemies;

                // ---

                // generate level size, num chips, num enemies
                i = index / TEOPL_DEFINES_LEVEL_SPECIAL_FREQUENCY;
                w = 13 + i;
                h = 10 + i;
                size = w * h;
                num_chips = 4 * (i + 2) / 2;
                num_enemies = i + 1;

                // generate probability for creating blocks
                ptr->level_special_percent_block = i * 5;
                ptr->level_special_percent_plant = 10 + (i * 5);;
                ptr->level_special_percent_captured = 5 * (i / 2);
                
                cpct_memset(level_internal, TEOPL_LEVEL_TILE_EMPTY, TEOPL_LEVEL_LVL_MAX_NUM_BYTES);

                // encode dimensions
                *level_internal = ((w - 3) << 4) | (h - 3);

                level_internal++; // move from encoded dimansions

                // generate chips               
                for (i = 0; i < num_chips; ++i) {
                    _generate_tile(TEOPL_LEVEL_TILE_CHIP_INACTIVE, level_internal, size);
                }

                // generate player
                _generate_tile(TEOPL_LEVEL_TILE_PLAYER_ACTIVE_DOWN, level_internal, size);

                // generate enemies
                for (i = 0; i < num_enemies; ++i) {
                    _generate_tile(TEOPL_LEVEL_TILE_ENEMY_DOWN, level_internal, size);
                }
            }
            // 2. local level => extract
            else {
                g_teopl_pool_levels_extracted = 0;
                teopl_pool_extract(TEOPL_POOL_TYPE_LEVELS, 0);
            }
        }

        dimensions_encoded = *(TEOPL_DEFINES_POOL_LEVEL);

        // level is encoded without frame and dimensions were decreased by 3 (minimal meanigful dimension) => add 3
        w = (dimensions_encoded >> 4) + 3;
        h = (dimensions_encoded & 0b00001111) + 3;
        size = w * h;

        ptr->level_width = w;
        ptr->level_height = h;
        ptr->level_size = size;
        ptr->level_tiles = (TEOPL_DEFINES_POOL_LEVEL + 1);
    }

    ptr->loaded_tiles = 1;

    ptr->level_x_b = (TEOPL_DEFINES_VIDEO_PARAM_SCREEN_WIDTH_B - (ptr->level_width * TEOPL_DEFINES_LEVEL_TILE_W_B)) / 3;
    ptr->level_x_b -= (ptr->level_x_b % TEOPL_DEFINES_LEVEL_TILE_W_B);

    ptr->level_y_px = (TEOPL_DEFINES_VIDEO_PARAM_SCREEN_HEIGHT_PX - BILLBOARD_HEIGHT_PX - ptr->level_height * TEOPL_DEFINES_LEVEL_TILE_H_PX) / 2;
    ptr->level_y_px -= (ptr->level_y_px % TEOPL_DEFINES_LEVEL_TILE_H_PX);

    ptr->level_frame_right_b = ptr->level_x_b + ptr->level_width * TEOPL_DEFINES_LEVEL_TILE_W_B;
    ptr->level_frame_down_px = ptr->level_y_px + ptr->level_height * TEOPL_DEFINES_LEVEL_TILE_H_PX;
}

static void _decode_text(char* text_dest, char* text_ptr_source, u8 text_size_zip) {

    char*   cptr = text_ptr_source;
    u8      c;

    char*   wptr;
    u8      cw;

    u8      word;
    u8      word_index;

    // ---

    while (cptr - text_ptr_source < text_size_zip) {

        c = *cptr;

        // decode to word or char
        word = 0;
        word_index = c;

        if (c > 95) {
            word = 1;
            word_index -= 50;
        }
        else if (c < 32) {
            word = 1;
        }
        else if (c > 33 && c < 39) {
            word = 1;
            word_index -= 2;
        }
        else if (c == 42 || c == 43) {
            word = 1;
            word_index -= 5;
        }
        else if (c == 47) {
            word = 1;
            word_index = 39;
        }
        else if (c == 59) {
            word = 1;
            word_index = 40;
        }
        else if (c == 61) {
            word = 1;
            word_index = 41;
        }
        else if (c > 90 && c < 95) {
            word = 1;
            word_index -= 49;
        }

        if (word) {
            wptr = frequent_words[word_index];
            cw = *wptr;
            wptr++;
            while (cw) {
                *text_dest = cw;
                text_dest++;
                cw = *wptr;
                wptr++;
            }
        }
        else {
            *text_dest = c;
            text_dest++;
        }

        cptr++;
    }

    *text_dest = 0;
}

// Populates:
// - number of texts
// - number of welcome texts
// - all level texts encoded
void teopl_level_load_texts_all(u8* extracted, u8 level_index) {

    u8* texts_start = 0;
    u8  text_level_index;
    u8  text_size_zip;
    u8  text_type;
    u16 size = 0;
    u8* texts_encoded = TEOPL_DEFINES_POOL_TEXTS_ENCODED;

    // ---

    if (ptr->texts_info_loaded) {
        return;
    }

    ptr->texts_num = 0;
    ptr->texts_num_welcome = 0;

    // ---

    // level index 0 is the marker to stop
    while (*extracted) {

        // read
        text_level_index = *extracted++ - 1;
        text_size_zip = *extracted++;
        text_type = *extracted++;

        if (text_level_index == level_index) {

            if (texts_start == 0) {
                texts_start = extracted - 3;
            }

            ptr->texts_num++;

            if (text_type == 'B' || text_type == 'C') {
                ptr->texts_num_welcome++;
            }
        }
        else if (text_level_index > level_index) {
            extracted -= 3;
            break;
        }

        extracted += text_size_zip;
    };

    if (texts_start) {

        size = extracted - texts_start;

        cpct_memcpy(g_teopl_pool_anything, texts_start, size);
        cpct_memcpy(texts_encoded, g_teopl_pool_anything, size);
    }

    // ---

    g_teopl_level_data.texts_info_loaded = 1;
}

// - decodes text with given index
// - returns picture type
u8 teopl_level_load_text(char* text_dest, u8 text_index) {

    u8* ptr_text;
    u8  text_size_zip;
    u8  text_type;

    TeoplBillboardPictureType picture_type = TEOPL_BILLBOARD_PICTURE_TYPE_NONE;

    // ---

    ptr_text = TEOPL_DEFINES_POOL_TEXTS_ENCODED;

    while (1) {

        // read
        *ptr_text++; // text_level_index = *text_ptr_source++ - 1;
        text_size_zip = *ptr_text++;
        text_type = *ptr_text++;

        // load text and picture type
        if (text_index == 0) {

            if (text_type == 'B' || text_type == 'D') {
                picture_type = TEOPL_BILLBOARD_PICTURE_TYPE_AI;
            }
            else if (text_type == 'C' || text_type == 'E') {
                picture_type = TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC;
            }

            _decode_text(text_dest, ptr_text, text_size_zip);

            break;
        }

        text_index--;

        ptr_text += text_size_zip;
    };

    // ---

    return picture_type;
}

void teopl_level_load_all(u8 index, u8 welcome) {

    teopl_billboard_say_finish(0);

    teopl_video_screen_clear();

    teopl_level_init();

    ptr->level_index = index;

    ptr->level_special = ((index + 1) % TEOPL_DEFINES_LEVEL_SPECIAL_FREQUENCY == 0) ? 1 : 0; // REMARK: special is every 3rd level

    // load password if not internet level
    if (index != 255) {
        teopl_level_password_get(index, ptr->level_password);
    }
    else {
        cpct_memcpy(ptr->level_password, g_teopl_level_password, TEOPL_LEVEL_PASSWORD_LENGTH);
    }

    // ---

    // stat loading level assets
    // ---

    // REMARK: not texts for internet levels and after pill is taken
    if (index != 255 && !g_teopl_main_pill) {
        teopl_pool_extract(TEOPL_POOL_TYPE_TEXTS, index);
    }

    teopl_billboard_init();

    if (welcome && ptr->texts_num_welcome) {
        // draw text
        teopl_logic_state_set(TEOPL_LOGIC_STATE_LEVEL_WELCOME);
        teopl_billboard_prepare_next_welcome_level_text(TEOPL_UTIL_ALGNMENT_CENTER);
    }
    else {
        // continue loading level assets
        // ---

        teopl_level_load_tiles(index);
        teopl_level_entities_extract(0, 0, g_teopl_level_data.level_width, g_teopl_level_data.level_height);

        // draw level
        teopl_video_draw_level(0, 0, g_teopl_level_data.level_width, g_teopl_level_data.level_height);

        // ready to play
        teopl_logic_state_set(TEOPL_LOGIC_STATE_LEVEL_PLAYABLE);
    }
}

void teopl_level_load_next() {

    u8 index_next = ptr->level_index + 1;

    // ---

    // REMARK: wait if needed (for example, after last chip counted)
    teopl_util_wait_freeze();

    // finishing internet level returnes to menu
    if (ptr->level_index == 255) {
        g_teopl_main_loop_game_exit = 1;
        return;
    }

    if (index_next < ptr->levels_num) {
        teopl_level_load_all(index_next, 1);
    }
    else {
        teopl_level_init();
        g_teopl_main_loop_game_exit = 1;
        return;
    }
}

static u8* _player_next_tile_get(TeoplEntityDirection direction) {

    u8 origin_x_b;
    u8 origin_y_b;

    u8 tile_x_b;
    u8 tile_y_b;

    u8 candidate;

    u8* result = 0;
    u8* result_candidate = 0;

    u16 tmp;
    u16 distance_px_min = (u16)-1;
    u16 distance_px_min_candidate = (u16)-1;

    u8  num = ptr->tiles_playable.num_elements;
    u8  i;
    TeoplEntity* player = ptr->entity_player;

    TEOPL_MEMORY_POINTER_TYPE output;

    // ---

    if (num > 0) {

        if (player) {
            origin_y_b = player->y_px / TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;
            origin_x_b = player->x_b;
        }
        else {
            origin_x_b = g_teopl_level_exit_activated_last_x_b;
            origin_y_b = g_teopl_level_exit_activated_last_y_px / TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;
        }

        for (i = 0; i < num; ++i) {

            candidate = 0;

            teopl_ringbuffer_peek(&(ptr->tiles_playable), i, &output);

            result_candidate = (u8*)output;
            tile_x_b = teopl_level_tile_x_b_get(result_candidate);
            tile_y_b = teopl_level_tile_y_px_get(result_candidate) / TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;

            switch (direction) {

            case TEOPL_ENTITY_DIRECTION_TYPE_NONE:
                candidate = 1;
                break;

            case TEOPL_ENTITY_DIRECTION_TYPE_UP:
                if (tile_y_b < origin_y_b) {
                    candidate = 1;
                }
                break;

            case TEOPL_ENTITY_DIRECTION_TYPE_DOWN:
                if (tile_y_b > origin_y_b) {
                    candidate = 1;
                }
                break;

            case TEOPL_ENTITY_DIRECTION_TYPE_LEFT:
                if (tile_x_b < origin_x_b) {
                    candidate = 1;
                }
                break;

            case TEOPL_ENTITY_DIRECTION_TYPE_RIGHT:
                if (tile_x_b > origin_x_b) {
                    candidate = 1;
                }
                break;
            }

            if (candidate) {

                tmp = teopl_util_abs_diff(origin_x_b, tile_x_b);
                distance_px_min_candidate = (tmp * tmp);

                tmp = teopl_util_abs_diff(origin_y_b, tile_y_b);
                distance_px_min_candidate += (tmp * tmp);

                // ---

                if (distance_px_min_candidate < distance_px_min) {

                    result = result_candidate;
                    distance_px_min = distance_px_min_candidate;
                }
            }
        }
    }

    return result;
}

u8 teopl_level_player_next(TeoplEntityDirection direction) {

    u8 found = 0;
    u8* player_next_tile = _player_next_tile_get(direction);

    // ---

    if (player_next_tile) {

        found = 1;

        teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_player_next_activated, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

        // previous player
        if (ptr->entity_player) {
            teopl_level_entity_to_tile(ptr->entity_player, TEOPL_LEVEL_TILE_PLAYER_INACTIVE);
        }

        // new player
        teopl_level_tile_to_entity(TEOPL_ENTITY_DRAW_TYPE_MASKED, player_next_tile, TEOPL_ENTITY_TYPE_PLAYER_ACTIVE_SELECTED_POOF, TEOPL_LEVEL_TILE_POOF, TEOPL_ENTITY_DIRECTION_TYPE_NONE, 0, 0);
    }

    return found;
}

void teopl_level_entity_to_tile(TeoplEntity* entity, TeoplLevelTileType tile_type) {

    u8* tile;
    u8  x_b = entity->x_b;
    u8  y_px = entity->y_px;

    // ---

    tile = teopl_level_tile_get_from_coord_aligned_absolute(x_b, y_px);

    // ---

    if (tile) {

        // special handling

        // ??? -> player inactive (tile)
        if (tile_type == TEOPL_LEVEL_TILE_PLAYER_INACTIVE) {
            teopl_ringbuffer_add(&(ptr->tiles_playable), (TEOPL_MEMORY_POINTER_TYPE)tile);
        }
        // ??? -> enemy ai inactive (tile)
        else if (tile_type == TEOPL_LEVEL_TILE_ENEMY_DOWN) {
            teopl_ringbuffer_add(&(ptr->tiles_ai), (TEOPL_MEMORY_POINTER_TYPE)tile);
        }

        // ---

        *tile = tile_type;

        teopl_video_draw_level_tile(tile); // FIXME: draw in main drawing function

        teopl_level_entity_remove(entity);
    }
}

/*
TeoplEntity* teopl_level_entity_to_entity(TeoplEntity* entity1, u8 entity2_types, u8 entity2_tile_sprite) {
    // FIXME: need this?

    return 0;
}
*/

void teopl_level_tile_to_entity(TeoplEntityDrawType draw_type, u8* tile, u8 entity_type, u8 entity_tile, TeoplEntityDirection direction, TEOPL_MEMORY_POINTER_TYPE data1, TEOPL_MEMORY_POINTER_TYPE data2) {

    u8 col = teopl_level_tile_x_get(tile);
    u8 row = teopl_level_tile_y_get(tile);
    TeoplLevelTileType tile_type = *tile;

    // ---

    teopl_level_entity_create(
        draw_type,
        entity_type,
        col * TEOPL_DEFINES_LEVEL_TILE_W_B + ptr->level_x_b, row * TEOPL_DEFINES_LEVEL_TILE_H_PX + ptr->level_y_px,
        TEOPL_DEFINES_LEVEL_TILE_W_B, TEOPL_DEFINES_LEVEL_TILE_H_PX,
        g_blocks + entity_tile * blocks_BYTES_PER_SPRITE,
        direction,
        TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC,
        data1,
        data2
    );

    // 2. remove tile
    // ---

    // REMARK: special handling when removing tile
    // ---

    // player inactive (tile)
    if (tile_type == TEOPL_LEVEL_TILE_PLAYER_INACTIVE) {
        teopl_ringbuffer_remove(&(ptr->tiles_playable), (TEOPL_MEMORY_POINTER_TYPE)tile);
    }
    // enemy ai inactive (tile)
    else if (tile_type == TEOPL_LEVEL_TILE_ENEMY_DOWN) {
        teopl_ringbuffer_remove(&(ptr->tiles_ai), (TEOPL_MEMORY_POINTER_TYPE)tile);
    }

    *tile = TEOPL_LEVEL_TILE_EMPTY;
}

void teopl_level_tile_to_tile(u8* tile1, TeoplLevelTileType tile2_type) {

    TeoplLevelTileType tile1_type = *tile1;

    // ---

    // special handling

    // player inactive -> ???
    if (tile1_type == TEOPL_LEVEL_TILE_PLAYER_INACTIVE) {
        teopl_ringbuffer_remove(&(ptr->tiles_playable), (TEOPL_MEMORY_POINTER_TYPE)tile1);
    }
    // enemy ai inactive -> ???
    else if (tile1_type == TEOPL_LEVEL_TILE_ENEMY_DOWN) {
        teopl_ringbuffer_remove(&(ptr->tiles_ai), (TEOPL_MEMORY_POINTER_TYPE)tile1);
    }

    // ---

    // ??? -> player inactive
    if (tile2_type == TEOPL_LEVEL_TILE_PLAYER_INACTIVE) {
        teopl_ringbuffer_add(&(ptr->tiles_playable), (TEOPL_MEMORY_POINTER_TYPE)tile1);
    }
    // ??? -> player inactive
    else if (tile2_type == TEOPL_LEVEL_TILE_ENEMY_DOWN) {
        teopl_ringbuffer_add(&(ptr->tiles_ai), (TEOPL_MEMORY_POINTER_TYPE)tile1);
    }

    // ---

    *tile1 = tile2_type;

    teopl_video_draw_level_tile(tile1);
}
