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

#include "teopl_collision.h"

#include "teopl_defines.h"
#include "teopl_ringbuffer.h"
#include "teopl_entity.h"
#include "teopl_memory.h"
#include "teopl_level.h"
#include "teopl_video.h"
#include "teopl_pool.h"
#include "teopl_sound.h"
#include "teopl_logic.h"

#include "assets/sprites/font.h"

extern u16  g_main_num_chips_total;
extern u8   g_teopl_main_pill;

// ---

// REMARK:
// ---
// Player should exit on these collisions:
// 
// 1. (player active) hits (goal)
// 2. (player active) hits (blue/red pill)
// 3. (player active) hits (enemy)          <= special case: player collided with entity (enemy) and not tile!
// 4. (player active) hits (plant)
// 5. (player active) hits (last chip)
static void _player_exit(TeoplEntity* player, u8* tile_hit) {

    u8* tile_goal_animation = 0;
    u8* tile_player = teopl_level_tile_get_from_coord_aligned_absolute(player->x_b, player->y_px);
    u8  finish_talk = 0;

    // ---

    //g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_INF;

    // 3. (player active) hits (enemy)      <= zero signals special case
    if (!tile_hit) {

        teopl_video_game_freeze_animating_frame(1);

        teopl_level_restart();

        return;
    }
    else {

        TeoplLevelTileType tile_hit_type = (TeoplLevelTileType)(*tile_hit);

        // ---

        // 1. (player active) hits (goal)
        if (tile_hit_type == TEOPL_LEVEL_TILE_GOAL) {
            tile_goal_animation = tile_hit;
        }
        // 5. (player active) hits (last chip)
        else if (tile_hit_type == TEOPL_LEVEL_TILE_CHIP_ACTIVATED) {
            finish_talk = 1;
            tile_goal_animation = tile_hit;
        }
        // 4. (player active) hits (plant)
        else if (tile_hit_type == TEOPL_LEVEL_TILE_PLANT_00) {
            finish_talk = 1;
            tile_goal_animation = 0; // REMARK: no goal animation, instead animate plant digesting!
        }
        // 2. (player active) hits (blue/red pill)
        else  if (tile_hit_type == TEOPL_LEVEL_TILE_PILL_BLUE || tile_hit_type == TEOPL_LEVEL_TILE_PILL_RED) {
            finish_talk = 1;
            tile_goal_animation = tile_player;
        }
    }

    if (finish_talk) {
        teopl_billboard_say_finish(0);
    }

    // ---

    teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_player_current_poofed, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

    // FIXME: use entity_to_entity instead entity->tile->entity?

    // step 1
    teopl_level_entity_to_tile(player, TEOPL_LEVEL_TILE_POOF); // REMARK: action on SUBJECT not allowed... but leave it for now

    // step 2
    teopl_level_tile_to_entity(
        TEOPL_ENTITY_DRAW_TYPE_MASKED,
        tile_player,
        TEOPL_ENTITY_TYPE_PLAYER_FREEING_POOF,
        TEOPL_LEVEL_TILE_POOF,
        TEOPL_ENTITY_DIRECTION_TYPE_NONE,
        (TEOPL_MEMORY_POINTER_TYPE)(tile_hit),
        (TEOPL_MEMORY_POINTER_TYPE)(tile_goal_animation)
    );
}

// REMARK: do not change player (SUBJECT), this is done later, update only tile hit (OBJECT)
static void _teopl_collision_player_hits_and_updates_tile(TeoplEntity* player, u8* tile_hit) {

    TeoplLevelTileType tile_type = *tile_hit;
    TeoplEntityType player_type = player->type;
    TeoplEntityDirection player_direction = player->move_direction;
    u8 player_x_b = player->x_b;
    u8 player_y_px = player->y_px;

    u8 player_is_active = player_type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE;

    // ---

    if (teopl_level_tile_type_is_block(tile_type)) {

        teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_player_hits_block);
    }
    else if (tile_type == TEOPL_LEVEL_TILE_GLASS) {

        teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_player_hits_glass);

        // ---

        teopl_level_tile_to_entity(
            TEOPL_ENTITY_DRAW_TYPE_MASKED,
            tile_hit,
            TEOPL_ENTITY_TYPE_TILE_GLASS_BREAKING,
            TEOPL_LEVEL_TILE_GLASS,
            TEOPL_ENTITY_DIRECTION_TYPE_NONE,
            0,
            0
        );
    }
    else if (tile_type == TEOPL_LEVEL_TILE_PLAYER_INACTIVE) {

        teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_player_hits_inactive);

        // ---

        teopl_level_tile_to_entity(
            TEOPL_ENTITY_DRAW_TYPE_MASKED,
            tile_hit,
            TEOPL_ENTITY_TYPE_PLAYER_INACTIVE,
            TEOPL_LEVEL_TILE_PLAYER_INACTIVE,
            player_direction,
            0,
            0
        );
    }
    else if (tile_type == TEOPL_LEVEL_TILE_PLAYER_CAPTURED) {

        teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_player_hits_captured);

        // ---

        teopl_level_tile_to_entity(
            TEOPL_ENTITY_DRAW_TYPE_NONMASKED,
            tile_hit,
            TEOPL_ENTITY_TYPE_TILE_PLAYERCAPTURED_FREEING,
            TEOPL_LEVEL_TILE_PLAYER_CAPTURED,
            TEOPL_ENTITY_DIRECTION_TYPE_NONE,
            0,
            0
        );
    }
    else if (tile_type == TEOPL_LEVEL_TILE_CHIP_INACTIVE) {

        if (!player_is_active) {

            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_player_hits_block, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 0, 0);
        }
        else {

            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_player_hits_chip_inactive, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 0, 0);

            // ---

            teopl_level_tile_to_entity(
                TEOPL_ENTITY_DRAW_TYPE_MASKED,
                tile_hit,
                TEOPL_ENTITY_TYPE_TILE_CHIP_ACTIVATING,
                TEOPL_LEVEL_TILE_CHIP_INACTIVE,
                TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                0,
                0
            );
        }
    }
    else if (tile_type == TEOPL_LEVEL_TILE_CHIP_ACTIVATED) {

        u8* tile_player = teopl_level_tile_get_from_coord_aligned_absolute(player_x_b, player_y_px);
        u8  y_px = 3 * font_HEIGHT_PX + g_teopl_level_data.level_special_num_chips_destroyed * (TEOPL_DEFINES_LEVEL_TILE_H_PX + 2);

        // ---

        if (!player_is_active) {

            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_player_hits_block, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 0, 0);
        }
        else {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_player_destroys_chip, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

            if (tile_player) {

                g_teopl_level_data.level_special_num_chips_destroyed++;

                // ---

                if (g_teopl_level_data.level_special && g_teopl_level_data.level_special_num_chips_destroyed == g_teopl_level_data.level_special_num_chips_total) {

                    _player_exit(player, tile_hit);
                }
            }

            teopl_level_tile_to_entity(
                TEOPL_ENTITY_DRAW_TYPE_MASKED,
                tile_hit,
                TEOPL_ENTITY_TYPE_LEVEL_REMOVING,
                TEOPL_LEVEL_TILE_POOF,
                TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                (TEOPL_MEMORY_POINTER_TYPE)y_px,
                (TEOPL_MEMORY_POINTER_TYPE)TEOPL_LEVEL_TILE_CHIP_ACTIVATED
            );
        }
    }
    else if (tile_type == TEOPL_LEVEL_TILE_KEY) {

        if (player_is_active) {

            u8  y_px = 3 * font_HEIGHT_PX + g_teopl_level_data.keys_num * (TEOPL_DEFINES_LEVEL_TILE_H_PX + 2);

            // ---

            teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_player_takes_key);

            g_teopl_level_data.keys_num++;

            teopl_level_tile_to_entity(
                TEOPL_ENTITY_DRAW_TYPE_MASKED,
                tile_hit,
                TEOPL_ENTITY_TYPE_LEVEL_REMOVING,
                TEOPL_LEVEL_TILE_POOF,
                TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                (TEOPL_MEMORY_POINTER_TYPE)y_px,
                (TEOPL_MEMORY_POINTER_TYPE)TEOPL_LEVEL_TILE_KEY
            );
        }
    }
    else if (tile_type == TEOPL_LEVEL_TILE_DOOR) {

        if (g_teopl_level_data.keys_num == 0) {

            teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_door_hit_with_no_key);
        }
        else {
            //  only active player (with key) can go through door
            if (player_is_active) {

                teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_door_open);

                // ---

                // 1. key gone
                teopl_level_entity_create(
                    TEOPL_ENTITY_DRAW_TYPE_NONMASKED,
                    TEOPL_ENTITY_TYPE_INVENTORY_REMOVING,
                    TEOPL_DEFINES_LEVEL_INVENTORY1_X_B, 3 * font_HEIGHT_PX + (g_teopl_level_data.keys_num - 1) * (TEOPL_DEFINES_LEVEL_TILE_H_PX + 2),
                    TEOPL_DEFINES_LEVEL_TILE_W_B, TEOPL_DEFINES_LEVEL_TILE_H_PX,
                    g_blocks + TEOPL_LEVEL_TILE_POOF * blocks_BYTES_PER_SPRITE,
                    TEOPL_ENTITY_DIRECTION_TYPE_NONE, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC,
                    (TEOPL_MEMORY_POINTER_TYPE)TEOPL_LEVEL_TILE_KEY,
                    0
                );

                g_teopl_level_data.keys_num--;

                // 2. door gone
                teopl_level_tile_to_entity(
                    TEOPL_ENTITY_DRAW_TYPE_MASKED,
                    tile_hit,
                    TEOPL_ENTITY_TYPE_TILE_DOOR_GONE,
                    TEOPL_LEVEL_TILE_DOOR,
                    TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                    0,
                    0
                );
            }
        }
    }
    else if (tile_type == TEOPL_LEVEL_TILE_PLANT_00) {

        u8* tile_player = teopl_level_tile_get_from_coord_aligned_absolute(player_x_b, player_y_px);

        // ---

        if (tile_player) {

            _player_exit(player, tile_hit);
        }
    }
    else if (tile_type == TEOPL_LEVEL_TILE_ENEMY_DOWN) {

        _player_exit(player, 0); // REMARK: send 0 as tile hit to know that collision came from entity (enemy)
    }
    else if (tile_type == TEOPL_LEVEL_TILE_PILL_BLUE || tile_type == TEOPL_LEVEL_TILE_PILL_RED) {

        // blue pill
        if (tile_type == TEOPL_LEVEL_TILE_PILL_BLUE) {

            if (!g_teopl_main_pill) {
                g_teopl_main_pill = 1;
            }
        }
        // red pill
        else {
            if (!g_teopl_main_pill) {
                g_teopl_main_pill = 2;
            }
        }

        teopl_video_game_freeze_animating_frame(1);

        teopl_video_matrix(g_teopl_main_pill);
        teopl_video_set(TEOPL_VIDEO_TYPE_GAME);

        _player_exit(player, tile_hit);
    }
    else if (tile_type == TEOPL_LEVEL_TILE_GOAL) {

        if (player_is_active) {

            _player_exit(player, tile_hit);
        }
    }
}

// REMARK: do not change enemy (SUBJECT), this is done later, update only tile hit (OBJECT)
static void _teopl_collision_enemy_hits_and_updates_tile(TeoplEntity* enemy, u8* tile_hit) {

    TeoplLevelTileType tile_type = *tile_hit;

    // ---

    // REMARK: ignore on non special levels
    if (!g_teopl_level_data.level_special) {
        return;
    }

    // ---

    if (teopl_level_tile_type_is_block(tile_type) ||
        tile_type == TEOPL_LEVEL_TILE_GLASS ||
        tile_type == TEOPL_LEVEL_TILE_PLAYER_CAPTURED ||
        tile_type == TEOPL_LEVEL_TILE_PLAYER_INACTIVE ||
        tile_type == TEOPL_LEVEL_TILE_PLANT_00) {

        teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_enemy_breaks_tile);

        // ---

        teopl_level_tile_to_entity(
            TEOPL_ENTITY_DRAW_TYPE_MASKED,
            tile_hit,
            TEOPL_ENTITY_TYPE_TILE_GLASS_BREAKING,
            TEOPL_LEVEL_TILE_GLASS,
            TEOPL_ENTITY_DIRECTION_TYPE_NONE,
            0,
            0
        );
    }
}

// REMARK: do not change player (SUBJECT), this is done later, update only entity hit (OBJECT)
static void _teopl_collision_player_hits_and_updates_entity(TeoplEntity* player, TeoplEntity* entity_hit) {

    switch (entity_hit->type) {

    case TEOPL_ENTITY_TYPE_ENEMY:

        // REMARK: always "bounce back" when hit by entity (less bugs?)
        teopl_entity_sprite_direction_and_animation_set(entity_hit, teopl_entity_direction_oposite_get(entity_hit->move_direction), TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC);

        break;
    }
}

// REMARK: do not change player (SUBJECT), this is done later, update only entity hit (OBJECT)
static void _teopl_collision_enemy_hits_and_updates_entity(TeoplEntity* enemy, TeoplEntity* entity_hit) {

    switch (entity_hit->type) {

    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:
    {
        // (enemy -> player)
        // ---

        // stop player (and move back if not aligned!)
        teopl_entity_stop(entity_hit);

        // continue flow
        _player_exit(entity_hit, 0); // REMARK: send 0 as tile hit to know that collision came from entity (enemy)
    }
    break;

    case TEOPL_ENTITY_TYPE_PLAYER_INACTIVE:
    {
        //teopl_level_load_all(g_teopl_level_data.level_index, 0);
    }
    break;
    }
}

// ---

void teopl_collision_init() {
}

u8 teopl_collision_check_entity_vs_frame(u8 e1_x_b, u8  e1_y_px, TeoplEntityDirection direction) {

    if (direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
        return (e1_y_px < g_teopl_level_data.level_y_px);
    }
    else if (direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
        return (e1_y_px + TEOPL_DEFINES_LEVEL_TILE_H_PX > g_teopl_level_data.level_frame_down_px);
    }
    else if (direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT) {
        return (e1_x_b < g_teopl_level_data.level_x_b);
    }
    else if (direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
        return (e1_x_b + TEOPL_DEFINES_LEVEL_TILE_W_B > g_teopl_level_data.level_frame_right_b);
    }

    return 0;
}

// FIXME: optimize - inline instead of function, ...
u8 teopl_collision_check_entity_vs_entity(
    u8 e1_x_b, u8 e1_y_px, /*u8 e1_w_b, u8 e1_h_px,*/
    u8 e2_x_b, u8 e2_y_px /*u8 e2_w_b, u8 e2_h_px*/
) {

    if (e1_x_b < e2_x_b + blocks_WIDTH_B &&
        e1_x_b + blocks_WIDTH_B > e2_x_b &&
        e1_y_px < e2_y_px + blocks_HEIGHT_PX &&
        e1_y_px + blocks_HEIGHT_PX > e2_y_px) {

        return 1;
    }

    return 0;
}

// ENTITY -> TILE
// ---

void teopl_collision_apply_entity_vs_tile(TeoplEntity* entity_hitting, u8* tile_hit) {

    TeoplEntityDirection direction_old = entity_hitting->move_direction;
    TeoplEntityDirection direction_new = TEOPL_ENTITY_DIRECTION_TYPE_NONE;

    u8 tile = *tile_hit;

    TeoplEntityType type_hitting = entity_hitting->type;

    // ---

    switch (type_hitting) {

    case TEOPL_ENTITY_TYPE_PLAYER_INACTIVE:
    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:

        // 1. action on OBJECT
        // ---

        _teopl_collision_player_hits_and_updates_tile(entity_hitting, tile_hit);

        // 2. action on SUBJECT
        // ---

        if (tile == TEOPL_LEVEL_TILE_CHIP_INACTIVE && type_hitting == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE) {

            direction_new = teopl_entity_direction_oposite_get(direction_old);

            teopl_entity_sprite_direction_and_animation_set(entity_hitting, direction_new, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC);
        }
        else {
            teopl_entity_stop(entity_hitting);
        }

        break;

    case TEOPL_ENTITY_TYPE_ENEMY:

        // 1. action on OBJECT
        // ---

        _teopl_collision_enemy_hits_and_updates_tile(entity_hitting, tile_hit);

        // 2. action on SUBJECT
        // ---

        //if (tile_hit == TEOPL_LEVEL_TILE_CHIP_INACTIVE || tile_hit == TEOPL_LEVEL_TILE_CHIP_ACTIVATED) {
            // FIXME: do something?
        //}

        teopl_entity_stop(entity_hitting);

        break;
    }
}

// ENTITY -> ENTITY
// ---

// REMARK: entity1 *is* moving, entity2 *maybe*
void teopl_collision_apply_entity_vs_entity(TeoplEntity* entity_hitting, TeoplEntity* entity_hit) {

    switch (entity_hitting->type) {

    case TEOPL_ENTITY_TYPE_PLAYER_INACTIVE:
    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:

        // 1. action on OBJECT
        // ---

        _teopl_collision_player_hits_and_updates_entity(entity_hitting, entity_hit);

        // 2. action on SUBJECT
        // ---

        if (entity_hit->type == TEOPL_ENTITY_TYPE_ENEMY) {

            // (player -> enemy)
            // ---

            // stop player (and move back if not aligned!)
            teopl_entity_stop(entity_hitting);

            // active player will exit
            if (entity_hitting->type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE) {
                _player_exit(entity_hitting, 0); // REMARK: send 0 as tile hit to know that collision came from entity (enemy)
            }
        }

        teopl_entity_stop(entity_hitting);

        break;

    case TEOPL_ENTITY_TYPE_ENEMY:

        // 1. action on OBJECT
        // ---

        _teopl_collision_enemy_hits_and_updates_entity(entity_hitting, entity_hit);

        // 2. action on SUBJECT
        // ---

        // REMARK: always "bounce back" when hitting entity (less bugs?)
        teopl_entity_sprite_direction_and_animation_set(entity_hitting, teopl_entity_direction_oposite_get(entity_hitting->move_direction), TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC);

        break;
    }
}

// ENTITY -> FRAME
// ---

void teopl_collision_apply_entity_vs_frame(TeoplEntity* entity_hitting, TeoplEntityDirection direction) {

    // 1. action on OBJECT
    // ---

    // ---

    if (entity_hitting->type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE) {
        teopl_sound_sfx_set_with_priority_default(TEOPL_DEFINES_SOUND_SFX_entity_hits_frame);
    }

    if (direction == TEOPL_ENTITY_DIRECTION_TYPE_UP || direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
        g_teopl_video_frame_data_level.ch1++;
        if (g_teopl_video_frame_data_level.ch1 == TEOPL_DEFINES_VIDEO_PARAM_COLORS_NUM) {
            g_teopl_video_frame_data_level.ch1 = 1;
        }
    }
    else {
        g_teopl_video_frame_data_level.cv1++;
        if (g_teopl_video_frame_data_level.cv1 == TEOPL_DEFINES_VIDEO_PARAM_COLORS_NUM) {
            g_teopl_video_frame_data_level.cv1 = 1;
        }
    }

    teopl_video_draw_box_frame(
        &g_teopl_video_frame_data_level,
        g_teopl_level_data.level_x_b, g_teopl_level_data.level_y_px,
        g_teopl_level_data.level_width * TEOPL_DEFINES_LEVEL_TILE_W_B, g_teopl_level_data.level_height * TEOPL_DEFINES_LEVEL_TILE_H_PX,
        0
    );

    // 2. action on SUBJECT
    // ---

    teopl_entity_stop(entity_hitting);
}
