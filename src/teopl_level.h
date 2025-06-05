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

#ifndef _TEOPL_LEVEL_H_
#define _TEOPL_LEVEL_H_

#include <types.h>

#include "teopl_billboard.h"
#include "teopl_ringbuffer.h"
#include "teopl_entity.h"
#include "teopl_font.h"
#include "teopl_memory.h"

#include "assets/sprites/blocks.h"
#include "levelset_local.h"

#define TEOPL_LEVEL_PASSWORD_LENGTH 7

// level max dimensions are 18x18 (in level design we have 20x20 with frame)
#define TEOPL_LEVEL_TILES_MAX_WIDTH 18
#define TEOPL_LEVEL_TILES_MAX_HEIGHT 18

#define TEOPL_LEVEL_TILE_EMPTY 255
#define TEOPL_LEVEL_INDEX_UNINITIALIZED 255

// REMARK: if this is changed, wee need to change server too (to send correct number of bytes for level)
#define TEOPL_LEVEL_LVL_MAX_NUM_BYTES (1 + TEOPL_LEVEL_TILES_MAX_WIDTH * TEOPL_LEVEL_TILES_MAX_HEIGHT) // +1 is for encoded dimensions

// max lines of text per level (including welcome and level texts)
#define TEOPL_LEVEL_MAX_NUM_TEXTS 4

// +3 for level index, talk type and text size
#define TEOPL_LEVEL_TEXTS_ENCODED_LINE 59
#define TEOPL_LEVEL_TEXTS_ENCODED_SIZE (TEOPL_LEVEL_MAX_NUM_TEXTS * (TEOPL_LEVEL_TEXTS_ENCODED_LINE + 3))

typedef struct {

    u8      keys_num;

    // prepared data
    // ---

    u8      level_x_b;
    u8      level_y_px;

    // loaded data
    // ---

    u8      loaded_tiles;
    u8      loaded_entities;

    u8*     level_tiles;                                            // pointer to tile array
    u16     level_size;
    u8      levels_num;
    u8      level_index;

    u8      level_special;
    u16     level_special_num_chips_total;
    u16     level_special_num_chips_destroyed;
    u8      level_special_percent_block;
    u8      level_special_percent_plant;
    u8      level_special_percent_captured;

    char    level_password[TEOPL_LEVEL_PASSWORD_LENGTH + 1];
    u8      level_width;
    u8      level_height;
    u8      level_frame_right_b;
    u8      level_frame_down_px;

    u8      texts_info_loaded;
    u8      texts_num;                                              // not more then TEOPL_LEVEL_MAX_NUM_TEXTS
    u8      texts_num_welcome;
    u8      text_welcome_next_index;

    TeoplEntity*        entity_player;
    TeoplEntity*        entity_ai;

    TeoplRingBuffer     entites;

    TeoplRingBuffer     tiles_playable;
    TeoplRingBuffer     tiles_ai;

} TeoplLevelData;

// REMARK: extended set of tiles (used not only in level editor!)
typedef enum {

    TEOPL_LEVEL_TILE_PLAYER_INACTIVE        = 0 /*G_playerInactive00*/,
    TEOPL_LEVEL_TILE_PLAYER_INACTIVE_LEFT   = 1 /*G_playerInactiveLeft00*/,
    TEOPL_LEVEL_TILE_PLAYER_INACTIVE_UP     = 4 /*G_playerInactiveUp00*/,
    TEOPL_LEVEL_TILE_PLAYER_INACTIVE_DOWN   = 7 /*G_playerInactiveDown00*/,

    TEOPL_LEVEL_TILE_PLAYER_ACTIVE_LEFT     = 10 /*G_playerActiveLeft00*/,
    TEOPL_LEVEL_TILE_PLAYER_ACTIVE_UP       = 15 /*G_playerActiveUp00*/,
    TEOPL_LEVEL_TILE_PLAYER_ACTIVE_DOWN     = 20 /*G_playerActiveDown00*/,

    TEOPL_LEVEL_TILE_PLAYER_CAPTURED        = 25 /*G_playerCaptured00*/,

    TEOPL_LEVEL_TILE_GLASS                  = 29 /*G_glass00*/,

    TEOPL_LEVEL_TILE_DOOR                   = 35 /*G_door00*/,

    TEOPL_LEVEL_TILE_GOAL                   = 40 /*G_goal00*/,

    TEOPL_LEVEL_TILE_BLOCK_01               = 48 /*G_block01*/,
    TEOPL_LEVEL_TILE_BLOCK_02               = 49 /*G_block02*/,
    TEOPL_LEVEL_TILE_BLOCK_03               = 50 /*G_block03*/,
    TEOPL_LEVEL_TILE_BLOCK_04               = 51 /*G_block04*/,
    TEOPL_LEVEL_TILE_BLOCK_05               = 52 /*G_block05*/,
    TEOPL_LEVEL_TILE_BLOCK_06               = 53 /*G_block06*/,
    TEOPL_LEVEL_TILE_BLOCK_07               = 54 /*G_block07*/,
    TEOPL_LEVEL_TILE_BLOCK_08               = 55 /*G_block08*/,

    TEOPL_LEVEL_TILE_ENEMY_LEFT             = 56 /*G_enemyLeft00*/,
    TEOPL_LEVEL_TILE_ENEMY_DOWN             = 59 /*G_enemyDown00*/,
    TEOPL_LEVEL_TILE_ENEMY_UP               = 62 /*G_enemyUp00*/,

    TEOPL_LEVEL_TILE_POOF                   = 65 /*G_poof00*/,

    TEOPL_LEVEL_TILE_KEY                    = 73 /*G_key00*/,

    TEOPL_LEVEL_TILE_CHIP_INACTIVE          = 74 /*G_chip00*/,
    TEOPL_LEVEL_TILE_CHIP_ACTIVATED         = 78 /*G_chip01*/,

    TEOPL_LEVEL_TILE_PLANT_00               = 79 /*G_plant00*/,

    TEOPL_LEVEL_TILE_PILL_BLUE              = 84 /*G_pillBlue*/,
    TEOPL_LEVEL_TILE_PILL_RED               = 85 /*G_pillRed*/

} TeoplLevelTileType;

// ---

extern u8           g_teopl_level_exit_activated_last_x_b;
extern u8           g_teopl_level_exit_activated_last_y_px;

extern TeoplLevelData   g_teopl_level_data;

extern char         g_teopl_level_password[];

// ---

u8                  teopl_level_is_ending();

void                teopl_level_remove_chip_from_inventory(u8 continue_flow);

u8                  teopl_level_tile_type_is_block(TeoplLevelTileType tile_type); 
u8                  teopl_level_tile_is_block(u8* tile);

u8                  teopl_level_index_get(char* password);
void                teopl_level_password_get(u8 index, char* password);

void                teopl_level_init();

void teopl_level_entity_create(
    TeoplEntityDrawType draw_type,
    u8 type,
    u8 x_b, u8 y_px, u8 w_b, u8 h_px, const u8* sprite,
    TeoplEntityDirection move_direction, TeoplEntityAnimationType force_anim_type,
    TEOPL_MEMORY_POINTER_TYPE data1, TEOPL_MEMORY_POINTER_TYPE data2
);

void                teopl_level_draw_entities();

void                teopl_level_restart();

u8                  teopl_level_coord_aligned_x_b_absolute(u8 x_b);
u8                  teopl_level_coord_aligned_y_px_absolute(u8 y_px);
u8                  teopl_level_coord_aligned_absolute(u8 x_b, u8 y_px);

u8*                 teopl_level_tile_get(u8 x, u8 y);
u8*                 teopl_level_tile_get_from_level(u8* level, u8 x, u8 y, u8 level_width);

u8                  teopl_level_tile_x_get(u8* tile);
u8                  teopl_level_tile_y_get(u8* tile);

u8*                 teopl_level_tile_get_from_coord_aligned_absolute(u8 x_b, u8 y_px);

TeoplEntity*        teopl_level_entity_get_from_coord_and_direction_absolute(TeoplEntity* entity_skip, u8 x_b, u8 y_px, TeoplEntityDirection move_direction);
u8*                 teopl_level_tile_get_from_coord_and_direction_absolute(u8 x_b, u8 y_px, TeoplEntityDirection move_direction);
u8                  teopl_level_tile_x_b_get(u8* tile);
u8                  teopl_level_tile_y_px_get(u8* tile);

void                teopl_level_entity_add(TeoplEntity* entity);
void                teopl_level_entity_remove(TeoplEntity* entity);

void                teopl_level_load_tiles(u8 index);
void                teopl_level_load_texts_all(u8* extracted, u8 level_index);
u8                  teopl_level_load_text(char* text_dest, u8 text_index);

void                teopl_level_load_all(u8 index, u8 welcome);

void                teopl_level_entities_extract(u8 x, u8 y, u8 width, u8 height);

void                teopl_level_load_next();

u8                  teopl_level_player_next(TeoplEntityDirection direction);

void                teopl_level_entity_to_tile(TeoplEntity* entity, TeoplLevelTileType tile_type);
//TeoplEntity*    teopl_level_entity_to_entity(TeoplEntity* entity1, u8 entity2_types, u8 entity2_tile_sprite);

void                teopl_level_tile_to_entity(TeoplEntityDrawType draw_type, u8* tile, u8 entity_type, u8 entity_tile_sprite, TeoplEntityDirection direction, TEOPL_MEMORY_POINTER_TYPE data1, TEOPL_MEMORY_POINTER_TYPE data2);
void                teopl_level_tile_to_tile(u8* tile1, TeoplLevelTileType tile2_type);

#endif
