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

#include "teopl_entity.h"

#include "teopl_video.h"
#include "teopl_collision.h"
#include "teopl_pool.h"
#include "teopl_level.h"
#include "teopl_util.h"
#include "teopl_logic.h"
#include "teopl_ringbuffer.h"
#include "teopl_interrupt.h"
#include "teopl_memory.h"
#include "teopl_sound.h"
#include "teopl_level.h"
#include "teopl_defines.h"
#include "teopl_input.h"

#include "assets/sprites/font.h"

cpctm_declareMaskTable(g_teopl_video_masktable);

extern u8 g_teopl_main_pill;

extern u16 g_main_num_chips_total;

extern u8 g_teopl_main_player_sprites_fliped_left;
extern u8 g_teopl_main_player_inactive_sprites_fliped_left;
extern u8 g_teopl_main_enemy_sprites_fliped_left;

extern u8 g_teopl_main_loop_game_exit;

// ---

static void _before_load_next_level() {

    teopl_billboard_say_finish(0);

    if (g_teopl_level_data.level_special_num_chips_destroyed > 0) {

        // REMARK: 1 = after removal, continue with next until level end (removing when chip is used to stop is diffrent context)
        teopl_level_remove_chip_from_inventory(1);

        // REMARK: don't count chips in internet level (or when pill is active)
        if (g_teopl_level_data.level_index != 255 && !g_teopl_main_pill) {

            u8 _string[5];
            u8 _length;

            // ---

            g_main_num_chips_total++;

            teopl_util_u32_to_string(_string, &_length, g_main_num_chips_total, 0);
            _string[_length] = 0;
            teopl_font_sync_print(
                TEOPL_DEFINES_LEVEL_PASS_X_B + 7 * font_WIDTH_B,
                TEOPL_DEFINES_LEVEL_PASS_Y_PX + 2 * font_HEIGHT_PX,
                32,
                _string
            );
        }
    }
    else {
        teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_level_end, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

        g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_XXL;

        teopl_level_load_next();
    }
}

TeoplEntity* teopl_entity_find_first(TeoplRingBuffer* entities, TeoplEntityType type) {

    u8 i;
    u8 num = entities->num_elements;
    TEOPL_MEMORY_POINTER_TYPE output;
    TeoplEntity* result = 0;

    // ---

    for (i = 0; i < num; ++i) {
        teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output);
        result = (TeoplEntity*)output;
        if (result->type == type) {
            break;
        }
    }

    return result;
}

u8 teopl_entity_is_player(TeoplEntity* entity) {
    return (entity->type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE || entity->type == TEOPL_ENTITY_TYPE_PLAYER_INACTIVE);
}

u8 teopl_entity_is_enemy(TeoplEntity* entity) {
    return (entity->type == TEOPL_ENTITY_TYPE_ENEMY);
}

void teopl_entity_draw(TeoplEntity* entity) {

    if (entity->draw) {

        u8* sprite = (void*)(entity->sprite + 16 * entity->anim_frame);

        u8 x_b = entity->x_b;
        u8 y_px = entity->y_px;
        //u8 w_b = entity->w_b;
        //u8 h_px = entity->h_px;

        void* location = TEOPL_VIDEO_GET_LOCATION(x_b, y_px);

        // ---

        teopl_entity_position_old_clear(entity, 1);

        entity->x_b_prev = x_b;
        entity->y_px_prev = y_px;

        if (entity->draw_type == TEOPL_ENTITY_DRAW_TYPE_MASKED) {

            cpct_drawSpriteMaskedAlignedTable(sprite, location, blocks_WIDTH_B, blocks_HEIGHT_PX, g_teopl_video_masktable);
        }
        else {

            cpct_drawSprite(sprite, location, blocks_WIDTH_B, blocks_HEIGHT_PX);
        }

        // ---

        entity->draw = 0;
    }
}

u8 teopl_entity_is_movable(TeoplEntity* entity) {

    TeoplEntityType type = entity->type;

    // ---

    return type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE || type == TEOPL_ENTITY_TYPE_ENEMY || type == TEOPL_ENTITY_TYPE_PLAYER_INACTIVE;
}

void teopl_entity_stop(TeoplEntity* entity) {

    TeoplEntityType type = entity->type;

    // ---

    // skip invalidated entities
    if (type == TEOPL_ENTITY_TYPE_NONE) {
        return;
    }

    // stop movement
    entity->move_ticks = 0;

    teopl_entity_position_old_clear(entity, 0);

    // align
    while (!teopl_level_tile_get_from_coord_aligned_absolute(entity->x_b, entity->y_px)) {
        teopl_entity_position_move(entity, teopl_entity_direction_oposite_get(entity->move_direction));
    }

    // REMARK: special handling
    // ---

    // player inactive is converted to tile on stopping
    if (type == TEOPL_ENTITY_TYPE_PLAYER_INACTIVE) {
        teopl_level_entity_to_tile(entity, TEOPL_LEVEL_TILE_PLAYER_INACTIVE);
    }
    // player inactive is converted to tile on stopping
    else if (type == TEOPL_ENTITY_TYPE_ENEMY) {
        teopl_level_entity_to_tile(entity, TEOPL_LEVEL_TILE_ENEMY_DOWN);
    }

    // ---

    if (entity->type != TEOPL_ENTITY_TYPE_NONE) {
        teopl_entity_sprite_direction_and_animation_set(entity, TEOPL_ENTITY_DIRECTION_TYPE_NONE, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC);
    }
}

TeoplEntityDirection teopl_entity_direction_oposite_get(TeoplEntityDirection move_direction) {

    if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
        return TEOPL_ENTITY_DIRECTION_TYPE_DOWN;
    }
    else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
        return TEOPL_ENTITY_DIRECTION_TYPE_UP;
    }
    else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT) {
        return TEOPL_ENTITY_DIRECTION_TYPE_RIGHT;
    }
    else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
        return TEOPL_ENTITY_DIRECTION_TYPE_LEFT;
    }

    return TEOPL_ENTITY_DIRECTION_TYPE_NONE;
}

// ---

// FIXME: remove not needed cases!
static void _teopl_entity_direction_none_data_set(TeoplEntity* entity, u8** sprite, u8* move_ticks_limit, TeoplEntityAnimationType* anim_type) {

    switch (entity->type) {

        // entities with no (idle) animation when static
        // ---

    case TEOPL_ENTITY_TYPE_PLAYER_INACTIVE:

        *move_ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_NONE;
        *anim_type = TEOPL_ENTITY_ANIMATION_TYPE_NONE;

        // REMARK: *no* direction sprite offset change reverted
        // *sprite = 0;

        break;

        // entities with "idle" animation when static
        // ---

#ifdef DEBUG
    case TEOPL_ENTITY_TYPE_TILE_PLAYERCAPTURED_FREEING:
    case TEOPL_ENTITY_TYPE_TILE_CHIP_ACTIVATING:
    case TEOPL_ENTITY_TYPE_TILE_GLASS_BREAKING:
    case TEOPL_ENTITY_TYPE_TILE_DOOR_GONE:
    case TEOPL_ENTITY_TYPE_PLANT_DIGESTING:
    case TEOPL_ENTITY_TYPE_TILE_GOAL_ACTIVATED:
    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:
    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE_SELECTED_POOF:
    case TEOPL_ENTITY_TYPE_PLAYER_FREEING_POOF:

    case TEOPL_ENTITY_TYPE_LEVEL_REMOVING:
    case TEOPL_ENTITY_TYPE_INVENTORY_ADDING:
    case TEOPL_ENTITY_TYPE_INVENTORY_REMOVING:

    case TEOPL_ENTITY_TYPE_BLOCK_CREATE_POOF:
#else
    default:
#endif

        *move_ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_NONE;
        *anim_type = TEOPL_ENTITY_ANIMATION_TYPE_IDLE;

        // REMARK: *no* direction sprite offset change reverted
        // *sprite = 0;

        break;
    }
}

static void _teopl_entity_direction_move_data_set(TeoplEntity* entity, u8** sprite, u8* move_ticks_limit, TeoplEntityAnimationType* anim_type) {

    TeoplEntityDirection move_direction = entity->move_direction;

    u8 i = 0;

    // ---

    // all moving entities have the same animation type
    *anim_type = TEOPL_ENTITY_ANIMATION_TYPE_MOVE;

    // ---

    switch (entity->type) {

    case TEOPL_ENTITY_TYPE_PLAYER_INACTIVE:

        *move_ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_player_inactive;

        // REMARK: *yes* direction sprite offset change (must be reverted when going to direction none)

        if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_PLAYER_INACTIVE_UP * blocks_BYTES_PER_SPRITE;
        }
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_PLAYER_INACTIVE_DOWN * blocks_BYTES_PER_SPRITE;
        }
        // REMARK: in case of left/right offeset is changed, but...
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT || move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_PLAYER_INACTIVE_LEFT * blocks_BYTES_PER_SPRITE;

            // ...but sprites must be flipped (if needed)
            if ((move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT && !g_teopl_main_player_inactive_sprites_fliped_left) ||
                (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT && g_teopl_main_player_inactive_sprites_fliped_left)) {

                // flip 3 move frames
                for (i = 0; i < 4; ++i) {
                    cpct_hflipSpriteM1(blocks_WIDTH_B, blocks_HEIGHT_PX, (*sprite + i * blocks_BYTES_PER_SPRITE));
                }

                g_teopl_main_player_inactive_sprites_fliped_left = !g_teopl_main_player_inactive_sprites_fliped_left;
            }
        }
        break;

    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:

        *move_ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_player_active;

        // REMARK: *yes* direction sprite offset change (must be reverted when going to direction none)
        if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_PLAYER_ACTIVE_UP * blocks_BYTES_PER_SPRITE;
        }
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_PLAYER_ACTIVE_DOWN * blocks_BYTES_PER_SPRITE;
        }
        // REMARK: in case of left/right offeset is changed, but...
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT || move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_PLAYER_ACTIVE_LEFT * blocks_BYTES_PER_SPRITE;

            // ...but sprites must be flipped (if needed)
            if ((move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT && !g_teopl_main_player_sprites_fliped_left) ||
                (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT && g_teopl_main_player_sprites_fliped_left)) {

                // flip 2 idle and 3 move frames
                for (i = 0; i < 5; ++i) {
                    cpct_hflipSpriteM1(blocks_WIDTH_B, blocks_HEIGHT_PX, (*sprite + i * blocks_BYTES_PER_SPRITE));
                }

                g_teopl_main_player_sprites_fliped_left = !g_teopl_main_player_sprites_fliped_left;
            }
        }
        break;

    case TEOPL_ENTITY_TYPE_ENEMY:

        *move_ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_enemy;

        // REMARK: *yes* direction sprite offset change (must be reverted when going to direction none)
        if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_ENEMY_UP * blocks_BYTES_PER_SPRITE;
        }
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_ENEMY_DOWN * blocks_BYTES_PER_SPRITE;
        }
        // REMARK: in case of left/right offeset is changed, but...
        else if (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT || move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
            *sprite = g_blocks + TEOPL_LEVEL_TILE_ENEMY_LEFT * blocks_BYTES_PER_SPRITE;

            // ...but sprites must be flipped (if needed)
            if ((move_direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT && !g_teopl_main_enemy_sprites_fliped_left) ||
                (move_direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT && g_teopl_main_enemy_sprites_fliped_left)) {

                // flip 3 move frames
                for (i = 0; i < 3; ++i) {
                    cpct_hflipSpriteM1(blocks_WIDTH_B, blocks_HEIGHT_PX, (*sprite + i * blocks_BYTES_PER_SPRITE));
                }

                g_teopl_main_enemy_sprites_fliped_left = !g_teopl_main_enemy_sprites_fliped_left;
            }
        }
        break;
    }
}

// one setter for:
//
// 1. starting sprite sometimes depending on direction
// 2. direction
// 3. animation parameters (depends on direction, but can be forced)
// ---
void teopl_entity_sprite_direction_and_animation_set(TeoplEntity* entity, TeoplEntityDirection move_direction, TeoplEntityAnimationType force_anim_type) {

    u8*                         sprite = 0;

    u8                          move_ticks_limit;
    TeoplEntityAnimationType    anim_type;

    // ---

    // REMARK: since sprite offset and animation parameters depend on direction, set it first
    entity->move_direction = move_direction;

    // ---

    switch (move_direction) {

    case TEOPL_ENTITY_DIRECTION_TYPE_NONE:

        _teopl_entity_direction_none_data_set(entity, &sprite, &move_ticks_limit, &anim_type);
        break;

    case TEOPL_ENTITY_DIRECTION_TYPE_UP:
    case TEOPL_ENTITY_DIRECTION_TYPE_DOWN:
    case TEOPL_ENTITY_DIRECTION_TYPE_LEFT:
    case TEOPL_ENTITY_DIRECTION_TYPE_RIGHT:

        _teopl_entity_direction_move_data_set(entity, &sprite, &move_ticks_limit, &anim_type);
        break;
    }

    // ---

    entity->draw = 1;

    // 1. sprite (offset - when depends on direction)
    // ---

    if (sprite) {
        entity->sprite = sprite;
    }

    // 2. direction
    // ---

    entity->move_ticks_limit = move_ticks_limit;
    entity->move_ticks = move_ticks_limit; // REMARK: move asap!

    // 3. animation
    // ---

    if (force_anim_type == TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC) {
        teopl_entity_animation_set(entity, anim_type);
    }
    else {
        teopl_entity_animation_set(entity, force_anim_type);
    }
}

// return first random direction where entity can go - else return 0
TeoplEntityDirection teopl_entity_sprite_direction_and_animation_set_random_non_blocked(u8 x_b, u8 y_px, u8 blocked_by_block_tiles) {

    u8 r = (u8)teopl_util_random32(4);
    u8 i;
    TeoplEntityDirection direction;

    for (i = 0; i < 4; ++i) {

        direction = (TEOPL_ENTITY_DIRECTION_TYPE_UP + r) % 4;
        r++;

        if (!teopl_entity_direction_blocked_or_static(0, x_b, y_px, direction, blocked_by_block_tiles)) {
            return direction;
        }
    }

    return 0;
}

void teopl_entity_position_and_collision_update(TeoplEntity* entity_hitting) {

    u8 i, num;

    TEOPL_MEMORY_POINTER_TYPE   output_hit;
    TeoplEntity*                entity_hit;

    u8* tile;
    u8  collided;

    u8  move_ticks_limit = entity_hitting->move_ticks_limit;

    TeoplEntityDirection  direction = entity_hitting->move_direction;

    // ---

    collided = 0;

    // FIXME: strange sync problems when allowing more then one position move... use if and not while
    while (entity_hitting->move_ticks >= move_ticks_limit && !collided) {
        //if (entity_hitting->move_ticks >= move_ticks_limit && !collided) {

        // FIXME: sync problems when allowing more then one position move? (setting tick to zero ensures that only one move is made)
        //entity_hitting->move_ticks = 0;
        entity_hitting->move_ticks -= move_ticks_limit;

        teopl_entity_position_move(entity_hitting, direction);

        // collision with frame
        // ---

        if (teopl_collision_check_entity_vs_frame(entity_hitting->x_b, entity_hitting->y_px, direction)) {

            // go back only once (on first collision)
            if (collided == 0) {
                teopl_entity_position_move(entity_hitting, teopl_entity_direction_oposite_get(direction));
            }

            collided = 1;
            teopl_collision_apply_entity_vs_frame(entity_hitting, direction);
        }
        else {

            // collision with tiles (check only if there is no frame collision)
            // ---

            tile = teopl_level_tile_get_from_coord_and_direction_absolute(entity_hitting->x_b, entity_hitting->y_px, direction);

            if (tile && *tile != TEOPL_LEVEL_TILE_EMPTY) {

                // go back only once (on first collision)
                if (collided == 0) {
                    teopl_entity_position_move(entity_hitting, teopl_entity_direction_oposite_get(direction));
                }

                collided = 1;
                teopl_collision_apply_entity_vs_tile(entity_hitting, tile);
            }
        }

        // collision with entities
        // ---

        num = g_teopl_level_data.entites.num_elements;

        for (i = 0; i < num; ++i) {

            teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output_hit);
            entity_hit = (TeoplEntity*)output_hit;

            // REMARK: ignore collision with self or when any entity is invalidated
            if (entity_hit == entity_hitting || entity_hitting->type == TEOPL_ENTITY_TYPE_NONE || entity_hit->type == TEOPL_ENTITY_TYPE_NONE) {
                continue;
            }

            if (teopl_collision_check_entity_vs_entity(
                entity_hitting->x_b, entity_hitting->y_px,
                entity_hit->x_b, entity_hit->y_px)) {

                // go back only once (on first collision)
                if (collided == 0) {
                    teopl_entity_position_move(entity_hitting, teopl_entity_direction_oposite_get(direction));
                }

                collided = 1;
                teopl_collision_apply_entity_vs_entity(entity_hitting, entity_hit);
            }
        }

        // move
        if (!collided) {

            entity_hitting->draw = 1;

            // ---

            if (entity_hitting->type == TEOPL_ENTITY_TYPE_ENEMY) {

                if (teopl_level_coord_aligned_absolute(entity_hitting->x_b, entity_hitting->y_px)) {

                    // decrease max number of moves needed
                    if (entity_hitting->data1 > 0) {
                        entity_hitting->data1--;
                    }
                    else {
                        teopl_entity_stop(entity_hitting);
                    }
                }
            }
        }
    }
}

void teopl_entity_position_move(TeoplEntity* entity, TeoplEntityDirection move_direction) {

    // FIXME: switch to 1px vertical movement?
    switch (move_direction) {
    case TEOPL_ENTITY_DIRECTION_TYPE_UP:
        entity->y_px -= TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;
        break;
    case TEOPL_ENTITY_DIRECTION_TYPE_DOWN:
        entity->y_px += TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;
        break;
    case TEOPL_ENTITY_DIRECTION_TYPE_LEFT:
        entity->x_b -= 1;
        break;
    case TEOPL_ENTITY_DIRECTION_TYPE_RIGHT:
        entity->x_b += 1;
        break;
    }
}

void teopl_entity_position_old_clear(TeoplEntity* entity, u8 force) {

    u8 x_b_old = entity->x_b_prev;
    u8 y_px_old = entity->y_px_prev;
    u8 x_b = entity->x_b;
    u8 y_px = entity->y_px;

    // ---

    // FIXME: check if this and force argument is needed
    if (x_b != x_b_old || y_px != y_px_old || force) {

        if (entity->draw_type == TEOPL_ENTITY_DRAW_TYPE_MASKED) {
            teopl_video_draw_background(x_b_old, y_px_old);
        }
        else {
            teopl_video_draw_box_color(0x00, x_b_old, y_px_old, blocks_WIDTH_B, blocks_HEIGHT_PX);
        }
    }
}

// ---

static void _teopl_entity_animation_none_data_set(TeoplEntity* entity, u8* frame_first, u8* frame_last_plus_one, u8* frame_loop, u8* ticks_limit) {

    *frame_first = 0;
    *frame_last_plus_one = 0;
    *frame_loop = 0;
    *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_NONE;
}

static void _teopl_entity_animation_idle_data_set(TeoplEntity* entity, u8* frame_first, u8* frame_last_plus_one, u8* frame_loop, u8* ticks_limit) {

    // common
    *frame_first = 0;
    *frame_loop = 0;

    switch (entity->type) {

        //  entities *with* idle animations
        // ---

    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:

        //*frame_first = 0;
        *frame_last_plus_one = 2;
        *frame_loop = TEOPL_DEFINES_ENTITY_ANIMATION_LOOP_FOREVER;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_player;

        break;

    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE_SELECTED_POOF:
    case TEOPL_ENTITY_TYPE_PLAYER_FREEING_POOF:
    case TEOPL_ENTITY_TYPE_BLOCK_CREATE_POOF:
    case TEOPL_ENTITY_TYPE_LEVEL_REMOVING:
    case TEOPL_ENTITY_TYPE_INVENTORY_ADDING:
    case TEOPL_ENTITY_TYPE_INVENTORY_REMOVING:

        //*frame_first = 0;
        *frame_last_plus_one = 8;
        //*frame_loop = 0;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_poof;

        break;

    case TEOPL_ENTITY_TYPE_TILE_PLAYERCAPTURED_FREEING:

        //*frame_first = 0;
        *frame_last_plus_one = 4;
        //*frame_loop = 0;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_captured_freeing;

        break;

    case TEOPL_ENTITY_TYPE_TILE_CHIP_ACTIVATING:

        //*frame_first = 0;
        *frame_last_plus_one = 4;
        //*frame_loop = 0;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_chip_activating;

        break;

    case TEOPL_ENTITY_TYPE_TILE_GLASS_BREAKING:

        //*frame_first = 0;
        *frame_last_plus_one = 6;
        //*frame_loop = 0;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_glass_breaking;

        break;

    case TEOPL_ENTITY_TYPE_TILE_DOOR_GONE:

        //*frame_first = 0;
        *frame_last_plus_one = 5;
        //*frame_loop = 0;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_door_gone;

        break;

    case TEOPL_ENTITY_TYPE_PLANT_DIGESTING:

        //*frame_first = 0;
        *frame_last_plus_one = 5;
        //*frame_loop = 0;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_plant_digesting;

        break;

    case TEOPL_ENTITY_TYPE_TILE_GOAL_ACTIVATED:

        //*frame_first = 0;
        *frame_last_plus_one = 8;
        *frame_loop = 2;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_goal_activated;

        break;
    }
}

static void _teopl_entity_animation_move_data_set(TeoplEntity* entity, u8* frame_first, u8* frame_last_plus_one, u8* frame_loop, u8* ticks_limit) {

    // common
    *frame_first = 0;
    *frame_loop = TEOPL_DEFINES_ENTITY_ANIMATION_LOOP_FOREVER;

    //  entities *with* moving animations
    // ---

    switch (entity->type) {

    case TEOPL_ENTITY_TYPE_PLAYER_INACTIVE:

        //*frame_first = 0;
        *frame_last_plus_one = 3;
        //*frame_loop = TEOPL_ENTITY_ANIMATION_LOOP_FOREVER;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_MOVE_player_inactive;
        break;

    case TEOPL_ENTITY_TYPE_PLAYER_ACTIVE:

        *frame_first = 2;
        *frame_last_plus_one = 5;
        //*frame_loop = TEOPL_ENTITY_ANIMATION_LOOP_FOREVER;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_MOVE_player_active;
        break;

    case TEOPL_ENTITY_TYPE_ENEMY:

        //*frame_first = 0;
        *frame_last_plus_one = 3;
        //*frame_loop = TEOPL_ENTITY_ANIMATION_LOOP_FOREVER;
        *ticks_limit = TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_MOVE_enemy;
        break;
    }
}

void teopl_entity_animation_set(TeoplEntity* entity, TeoplEntityAnimationType type) {

    u8 frame_first;
    u8 frame_last_plus_one;
    u8 frame_loop;
    u8 ticks_limit;

    // ---

    switch (type) {

    case TEOPL_ENTITY_ANIMATION_TYPE_NONE:
        _teopl_entity_animation_none_data_set(entity, &frame_first, &frame_last_plus_one, &frame_loop, &ticks_limit);
        break;

    case TEOPL_ENTITY_ANIMATION_TYPE_IDLE:
        _teopl_entity_animation_idle_data_set(entity, &frame_first, &frame_last_plus_one, &frame_loop, &ticks_limit);
        break;

    case TEOPL_ENTITY_ANIMATION_TYPE_MOVE:
        _teopl_entity_animation_move_data_set(entity, &frame_first, &frame_last_plus_one, &frame_loop, &ticks_limit);
        break;
    }

    // ---

    entity->draw = 1;

    // ---

    // animation
    // ---

    entity->anim_type = type;

    entity->anim_frame = frame_first;
    entity->anim_frame_first = frame_first;
    entity->anim_frame_last_plus_one = frame_last_plus_one;
    entity->anim_loops = frame_loop;

    entity->anim_ticks = ticks_limit; // animate asap!
    entity->anim_ticks_limit = ticks_limit;
}

// ---

static void _player_after_freeing() {

    if (g_teopl_level_data.tiles_playable.num_elements == 0 || g_teopl_level_data.level_special) {

        _before_load_next_level();
    }
    else {
        teopl_level_player_next(TEOPL_ENTITY_DIRECTION_TYPE_NONE);
    }
}

void teopl_entity_animation_update(TeoplEntity* entity_animating) {

    u16 anim_ticks_limit = entity_animating->anim_ticks_limit;
    TeoplEntityType type = entity_animating->type;

    // ---

    //while (entity_animating->anim_ticks >= anim_ticks_limit) {
    if (entity_animating->anim_ticks >= anim_ticks_limit) {

        // FIXME: to sync with movement, don't allow more then one frame update per main loop, so set tick to zero disables re-entering while
        entity_animating->anim_ticks = 0;
        //entity_animating->anim_ticks -= anim_ticks_limit;

        if (entity_animating->anim_type != TEOPL_ENTITY_ANIMATION_TYPE_NONE) {

            entity_animating->anim_frame++;
            entity_animating->draw = 1;

            // last frame...
            if (entity_animating->anim_frame == entity_animating->anim_frame_last_plus_one) {

                // ...of forever looping animation
                if (entity_animating->anim_loops == TEOPL_DEFINES_ENTITY_ANIMATION_LOOP_FOREVER) {
                    entity_animating->anim_frame = entity_animating->anim_frame_first;
                }
                // of animation looping finite number of times
                else if (entity_animating->anim_loops > 0) {
                    entity_animating->anim_frame = entity_animating->anim_frame_first;
                    entity_animating->anim_loops--;
                }
                // ...of animation finished looping (or never looping)
                else if (entity_animating->anim_loops == 0) {
                    // FIXME: move somewhere?
                    if (type == TEOPL_ENTITY_TYPE_TILE_GLASS_BREAKING) {
                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_EMPTY);
                    }
                    else if (type == TEOPL_ENTITY_TYPE_PLAYER_ACTIVE_SELECTED_POOF) {

                        u8* tile = teopl_level_tile_get_from_coord_aligned_absolute(entity_animating->x_b, entity_animating->y_px);

                        if (tile) {

                            teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_EMPTY);
                            teopl_level_tile_to_entity(TEOPL_ENTITY_DRAW_TYPE_MASKED, tile, TEOPL_ENTITY_TYPE_PLAYER_ACTIVE, TEOPL_LEVEL_TILE_PLAYER_ACTIVE_DOWN, TEOPL_ENTITY_DIRECTION_TYPE_NONE, 0, 0);
                        }
                    }
                    else if (type == TEOPL_ENTITY_TYPE_TILE_CHIP_ACTIVATING) {
                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_CHIP_ACTIVATED);
                    }
                    else if (type == TEOPL_ENTITY_TYPE_LEVEL_REMOVING) {

                        u8  x_b;
                        TeoplLevelTileType tile_removing;
                        TeoplLevelTileType tile_adding;

                        // ---

                        tile_removing = (TeoplLevelTileType)(entity_animating->data2);

                        if (tile_removing == TEOPL_LEVEL_TILE_KEY) {
                            x_b = TEOPL_DEFINES_LEVEL_INVENTORY1_X_B;
                            tile_adding = TEOPL_LEVEL_TILE_KEY;
                        }
                        else if (tile_removing == TEOPL_LEVEL_TILE_CHIP_ACTIVATED) {
                            x_b = TEOPL_DEFINES_LEVEL_INVENTORY2_X_B;
                            tile_adding = TEOPL_LEVEL_TILE_CHIP_INACTIVE;
                        }

                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_EMPTY);

                        teopl_level_entity_create(
                            TEOPL_ENTITY_DRAW_TYPE_NONMASKED,
                            TEOPL_ENTITY_TYPE_INVENTORY_ADDING,
                            x_b, (u8)(entity_animating->data1),
                            TEOPL_DEFINES_LEVEL_TILE_W_B, TEOPL_DEFINES_LEVEL_TILE_H_PX, g_blocks + TEOPL_LEVEL_TILE_POOF * blocks_BYTES_PER_SPRITE,
                            TEOPL_ENTITY_DIRECTION_TYPE_NONE, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC,
                            (TEOPL_MEMORY_POINTER_TYPE)tile_adding,
                            0
                        );
                    }
                    else if (type == TEOPL_ENTITY_TYPE_INVENTORY_ADDING) {

                        entity_animating->draw = 1;
                        entity_animating->sprite = g_blocks + (TeoplLevelTileType)(entity_animating->data1) * blocks_BYTES_PER_SPRITE;
                        teopl_entity_animation_set(entity_animating, TEOPL_ENTITY_ANIMATION_TYPE_NONE);
                        teopl_entity_draw(entity_animating);

                        teopl_level_entity_remove(entity_animating);
                    }
                    else if (type == TEOPL_ENTITY_TYPE_INVENTORY_REMOVING) {

                        u8 type_removed = (TeoplLevelTileType)(entity_animating->data1);
                        u8 continue_flow = (TeoplLevelTileType)(entity_animating->data2);

                        // ---

                        teopl_entity_position_old_clear(entity_animating, 1);

                        teopl_level_entity_remove(entity_animating);

                        // REMARK: increase counter for chips/qubits and continue until last one and then progress level
                        if (type_removed == TEOPL_LEVEL_TILE_CHIP_INACTIVE && continue_flow) {

                            _before_load_next_level();
                        }
                    }
                    else if (type == TEOPL_ENTITY_TYPE_TILE_DOOR_GONE) {
                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_EMPTY);
                    }
                    else if (type == TEOPL_ENTITY_TYPE_TILE_GOAL_ACTIVATED) {

                        g_teopl_level_exit_activated_last_x_b = entity_animating->x_b;
                        g_teopl_level_exit_activated_last_y_px = entity_animating->y_px;

                        // 1. remove goal tile
                        if (entity_animating->data1) {
                            teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_EMPTY);
                        }
                        // 2. reset goal tile
                        else {
                            teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_GOAL);
                        }

                        _player_after_freeing();
                    }
                    else if (type == TEOPL_ENTITY_TYPE_TILE_PLAYERCAPTURED_FREEING) {
                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_PLAYER_INACTIVE);
                    }
                    else if (type == TEOPL_ENTITY_TYPE_PLAYER_FREEING_POOF) {

                        u8* tile_hit = (u8*)(entity_animating->data1);
                        u8* tile_goal_animation = (u8*)(entity_animating->data2);

                        u8 remove_goal_after_animating = 1;

                        // ---

                        if (tile_hit && *tile_hit == TEOPL_LEVEL_TILE_GOAL) {
                            remove_goal_after_animating = 0;
                        }

                        // 1. (player active) hits (goal)
                        // 2. (player active) hits (blue/red pill)
                        if (tile_goal_animation) {

                            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_goal_animating, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

                            teopl_level_tile_to_entity(
                                TEOPL_ENTITY_DRAW_TYPE_MASKED,
                                tile_goal_animation,
                                TEOPL_ENTITY_TYPE_TILE_GOAL_ACTIVATED,
                                TEOPL_LEVEL_TILE_GOAL,
                                TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                                remove_goal_after_animating,
                                0
                            );
                        }
                        // 4. (player active) hits (plant)
                        else if (tile_hit && *tile_hit == TEOPL_LEVEL_TILE_PLANT_00) {

                            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_plant_digesting, TEOPL_SOUND_SFX_PRIORITY_HIGH, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

                            teopl_level_tile_to_entity(
                                TEOPL_ENTITY_DRAW_TYPE_MASKED,
                                tile_hit, // plant tile to animate
                                TEOPL_ENTITY_TYPE_PLANT_DIGESTING,
                                TEOPL_LEVEL_TILE_PLANT_00,
                                TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                                0,
                                0
                            );
                        }

                        // remove player freeing entity
                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_EMPTY);
                    }
                    else if (type == TEOPL_ENTITY_TYPE_BLOCK_CREATE_POOF) {

                        teopl_level_entity_to_tile(entity_animating, (TeoplLevelTileType)(entity_animating->data1));
                    }
                    else if (type == TEOPL_ENTITY_TYPE_PLANT_DIGESTING) {

                        g_teopl_level_exit_activated_last_x_b = entity_animating->x_b;
                        g_teopl_level_exit_activated_last_y_px = entity_animating->y_px;

                        // ---

                        teopl_level_entity_to_tile(entity_animating, TEOPL_LEVEL_TILE_PLANT_00);

                        g_teopl_util_freeze = 0;

                        teopl_level_restart();
                        //_player_after_freeing();
                    }
                }
            }
        }
    }
}

u8 teopl_entity_direction_blocked_or_static(TeoplEntity* entity_skip, u8 x_b, u8 y_px, TeoplEntityDirection direction, u8 blocked_by_block_tiles) {

    // REMARK: check if something blocks the movement (non-enemy entities, blocks or frame)
    if (direction != TEOPL_ENTITY_DIRECTION_TYPE_NONE) {

        TeoplEntity*    entity_hit;
        u8*             tile_hit;

        // simulate movement
        if (direction == TEOPL_ENTITY_DIRECTION_TYPE_LEFT) {
            x_b -= 1;
        }
        else if (direction == TEOPL_ENTITY_DIRECTION_TYPE_RIGHT) {
            x_b += 1;
        }
        else if (direction == TEOPL_ENTITY_DIRECTION_TYPE_UP) {
            y_px -= TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;
        }
        else if (direction == TEOPL_ENTITY_DIRECTION_TYPE_DOWN) {
            y_px += TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;
        }

        entity_hit = teopl_level_entity_get_from_coord_and_direction_absolute(entity_skip, x_b, y_px, direction);
        tile_hit = teopl_level_tile_get_from_coord_and_direction_absolute(x_b, y_px, direction);

        // REMARK: when next to *any* entity except enemy => do not try to move (or collide)
        if (entity_hit && entity_hit->type != TEOPL_ENTITY_TYPE_ENEMY) {
            return 1; // blocked by entity
        }

        // REMARK: when next to *any* block tile => do not try to move (or collide)
        if (blocked_by_block_tiles) {
            if (tile_hit && teopl_level_tile_is_block(tile_hit)) {
                return 1; // blocked by tile
            }
        }

        // REMARK: when next to frame => do not try to move (or collide)
        if (teopl_collision_check_entity_vs_frame(x_b, y_px, direction)) {
            return 1; // blocked by frame
        }

        return 0; // not blocked
    }
    else {
        return 1; // static
    }
}