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

#include "teopl_events.h"

#include <types.h>

#include "teopl_level.h"
#include "teopl_util.h"
#include "teopl_billboard.h"
#include "teopl_input.h"
#include "teopl_entity.h"
#include "teopl_font.h"
#include "teopl_video.h"
#include "teopl_sound.h"
#include "teopl_logic.h"
#include "teopl_interrupt.h"

#ifdef WINCPCTELERA
#define _ACTION_STRING (_special_index_action == ACTION_AWAKE_AND_FOLLOW_PLAYER ? "AWAKE AND FOLLOW" : _special_index_action == ACTION_CREATE_BLOCK ? "CREATE" : "?")
#endif

static char     _time_string[TEOPL_UTIL_TIME_STRING_LEN];

// ---

// general
// ---

typedef enum {

    UPDATE_TYPE_TALK,
    UPDATE_TYPE_SPECIAL,

    UPDATE_TYPE_NUM

} UpdateType;

// talk
// ---

typedef enum {

    STATE_TALK_WAIT_TO_START,       // when time passes, starts next talk           + set the next time wait
    STATE_TALK_WAIT_TO_END          // when time passes, this state removes banner  + set the next time wait

} StateTalkId;

// special
// ---

typedef enum {

    STATE_SPECIAL_WAIT_TO_START,    // when time passes, starts next "special action"                       + set the next time wait
    STATE_SPECIAL_WAIT_TO_END       // when time passes, this state will determine next "special action"    + set the next time wait

} StateSpecialId;

typedef enum {

    ACTION_AWAKE_AND_FOLLOW_PLAYER,
    //ACTION_AWAKE_AND_GO_RANDOM,

    ACTION_CREATE_BLOCK,

    ACTION_NUM

} Action;

// ###

// general
// ---

static u8               _done[UPDATE_TYPE_NUM];

static u32              _time_start[UPDATE_TYPE_NUM];
static u16              _time_to_pass[UPDATE_TYPE_NUM];

// talk
// ---

static StateTalkId      _talk_state_id;
static u16              _talk_time_to_pass_map[TEOPL_LEVEL_MAX_NUM_TEXTS];

static u8               _talk_index_text_start;
static u8               _talk_index_text_end;

// special
// ---

static StateTalkId      _special_state_id;
static u16              _special_time_to_pass_before;
static u16              _special_time_to_pass_after;

static u8               _special_index_action;

static void _set_wait(u16 ticks, UpdateType type) {

    _time_start[type] = g_teopl_interrupt_ticks_total;
    _time_to_pass[type] = ticks;
}

// talk
// ---

static void _talk_time_passed() {

    switch (_talk_state_id) {

    case STATE_TALK_WAIT_TO_START:
    {

        TeoplBillboardPictureType picture_type = TEOPL_BILLBOARD_PICTURE_TYPE_NONE;
        picture_type = teopl_level_load_text((char*)&g_teopl_font_async_text, g_teopl_level_data.texts_num_welcome + _talk_index_text_start);
        teopl_billboard_say(TEOPL_UTIL_ALGNMENT_BOTTOM, picture_type);

        // ---

        _set_wait(TEOPL_UTIL_SECONDS_TO_TICKS(15), UPDATE_TYPE_TALK); // time for banner visibility
        _talk_state_id = STATE_TALK_WAIT_TO_END;

        break;
    }

    case STATE_TALK_WAIT_TO_END:

        teopl_banner_remove();

        if (_talk_index_text_start < _talk_index_text_end - 1) {

            // determine next talk and wait time before that
            _talk_index_text_start++;
            _set_wait(_talk_time_to_pass_map[_talk_index_text_start], UPDATE_TYPE_TALK);
        }
        else {
            _done[UPDATE_TYPE_TALK] = 1;
            _talk_index_text_end = 0;
        }

        // ---

        _talk_state_id = STATE_TALK_WAIT_TO_START;
        break;
    }
}

// special
// ---

static Action _special_action_choose() {

    return teopl_util_random32(ACTION_NUM);
}

static TeoplLevelTileType _special_block_choose() {

    u32 r = teopl_util_random32(100);

    u8 p1 = g_teopl_level_data.level_special_percent_block;
    u8 p2 = p1 + g_teopl_level_data.level_special_percent_plant;
    u8 p3 = p2 + g_teopl_level_data.level_special_percent_captured;

    if (r <  p1) {
        return TEOPL_LEVEL_TILE_BLOCK_04;
    }
    else if (r < p2) {
        return TEOPL_LEVEL_TILE_PLANT_00;
    }
    else if (r < p3) {
        return TEOPL_LEVEL_TILE_PLAYER_CAPTURED;
    }
    else {
        return TEOPL_LEVEL_TILE_GLASS;
    }
}

static u8 _special_action_apply() {

    TeoplEntity*            player = g_teopl_level_data.entity_player;
    TeoplEntity*            ai = g_teopl_level_data.entity_ai;

    TeoplEntityDirection    direction = TEOPL_ENTITY_DIRECTION_TYPE_NONE;

    u8  i, num = g_teopl_level_data.entites.num_elements;
    TEOPL_MEMORY_POINTER_TYPE output;
    TeoplEntity* entity = 0;

    u8 applied = 1;

    u8* tile;

    // ---

    switch (_special_index_action) {

    case ACTION_AWAKE_AND_FOLLOW_PLAYER:
    {
        u8 player_x_b;
        u8 player_y_px;

        u8 ai_x_b;
        u8 ai_y_px;

        u32 follow_steps = teopl_util_random32(4) + 1;
        u8  follow_horizontal = follow_steps % 2;

        // ---

        if (!player) {

            applied = 0;
        }
        else {

            player_x_b = player->x_b;
            player_y_px = player->y_px;

            num = g_teopl_level_data.tiles_ai.num_elements;

            if (!ai && num && teopl_ringbuffer_peek(&(g_teopl_level_data.tiles_ai), 0, &output)) {

                tile = (u8*)output;

                ai_x_b = teopl_level_tile_x_b_get(tile);
                ai_y_px = teopl_level_tile_y_px_get(tile);

                // horizontal
                if (follow_horizontal) {

                    if (ai_x_b > player_x_b) {
                        direction = TEOPL_ENTITY_DIRECTION_TYPE_LEFT;
                    }
                    else if (ai_x_b < player_x_b) {
                        direction = TEOPL_ENTITY_DIRECTION_TYPE_RIGHT;
                    }
                }
                // vertical
                else {

                    if (ai_y_px > player_y_px) {
                        direction = TEOPL_ENTITY_DIRECTION_TYPE_UP;
                    }
                    else if (ai_y_px < player_y_px) {
                        direction = TEOPL_ENTITY_DIRECTION_TYPE_DOWN;
                    }
                }

                // try random non blocked direction when horizontal or vertical not possible
                if (direction == TEOPL_ENTITY_DIRECTION_TYPE_NONE) {
                    direction = teopl_entity_sprite_direction_and_animation_set_random_non_blocked(ai_x_b, ai_y_px, 0);
                }

                if (direction) {
                    applied = 1;
                }

                if (applied) {

                    teopl_level_tile_to_entity(
                        TEOPL_ENTITY_DRAW_TYPE_MASKED,
                        tile,
                        TEOPL_ENTITY_TYPE_ENEMY,
                        TEOPL_LEVEL_TILE_ENEMY_DOWN,
                        direction,
                        0, 0
                    );

                    teopl_entity_sprite_direction_and_animation_set(g_teopl_level_data.entity_ai, direction, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC);

                    // REMARK: length to go before stopping
                    g_teopl_level_data.entity_ai->data1 = follow_steps;
                }
            }
            else {
                applied = 0;
            }
        }
    }
    break;

    case ACTION_CREATE_BLOCK:
    {
        u8* tile = (u8*)(g_teopl_level_data.level_tiles) + (teopl_util_random32(g_teopl_level_data.level_size - 1));
        u8  entity_occupied_tile_space = 0;
        u8  tile_x_b;
        u8  tile_y_px;
        u8  near_blocks;
        u8  d_x_b_min;
        u8  d_y_px_min;
        u8  dx_b, dy_px;

        //u8 entities_still = player->move_direction == TEOPL_ENTITY_DIRECTION_TYPE_NONE && ai->move_direction == TEOPL_ENTITY_DIRECTION_TYPE_NONE;

        // ---

        if (*tile != TEOPL_LEVEL_TILE_EMPTY) {

            applied = 0;
        }
        else {

            tile_x_b = teopl_level_tile_x_b_get(tile);
            tile_y_px = teopl_level_tile_y_px_get(tile);

            for (i = 0; i < num; ++i) {

                teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output);
                entity = (TeoplEntity*)output;

                // allow 1 tile close to non movable entity and 3 tiles close to movable
                near_blocks = teopl_entity_is_movable(entity) ? 3 : 1;

                d_x_b_min = TEOPL_DEFINES_LEVEL_TILE_W_B * near_blocks;
                d_y_px_min = TEOPL_DEFINES_LEVEL_TILE_H_PX * near_blocks;

                dx_b = (u8)teopl_util_abs_diff(entity->x_b, tile_x_b);
                dy_px = (u8)teopl_util_abs_diff(entity->y_px, tile_y_px);

                if (dx_b <= d_x_b_min && dy_px <= d_y_px_min) {

                    entity_occupied_tile_space = 1;
                    
                    break;
                }
            }

            if (!entity_occupied_tile_space /* && entities_still */) {

                u8 block = _special_block_choose();

                teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_block_create_poofed, TEOPL_SOUND_SFX_PRIORITY_LOW, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 0, 0);

                // ---

                teopl_level_tile_to_entity(
                    TEOPL_ENTITY_DRAW_TYPE_MASKED,
                    tile,
                    TEOPL_ENTITY_TYPE_BLOCK_CREATE_POOF,
                    TEOPL_LEVEL_TILE_POOF,
                    TEOPL_ENTITY_DIRECTION_TYPE_NONE,
                    (TEOPL_MEMORY_POINTER_TYPE)block,
                    0
                );
            }
            else {
                applied = 0;
            }
        }
    }
    break;
}

// ---

#ifdef WINCPCTELERA
if (applied) {

    printf("ACTION '%s' applied!\n", _ACTION_STRING);
}
else {

    printf("ACTION '%s' skipped...\n", _ACTION_STRING);
}
#endif

return applied;
}

static void _special_time_passed() {

    switch (_special_state_id) {

    case STATE_SPECIAL_WAIT_TO_START:
    {
        if (_special_index_action == ACTION_NUM) {
            _special_index_action = _special_action_choose();

#ifdef WINCPCTELERA
            printf("ACTION '%s' selected.\n", _ACTION_STRING);
#endif
        }

        if (_special_action_apply()) {

            _set_wait(_special_time_to_pass_after, UPDATE_TYPE_SPECIAL); // time for action duration
            _special_state_id = STATE_SPECIAL_WAIT_TO_END;

#ifdef WINCPCTELERA
            printf("ACTION '%s' applied!\n", _ACTION_STRING);
#endif
        }

        _special_index_action = ACTION_NUM;
    }

    case STATE_SPECIAL_WAIT_TO_END:
    {

        // ---

        _set_wait(_special_time_to_pass_before, UPDATE_TYPE_SPECIAL);
        _special_state_id = STATE_SPECIAL_WAIT_TO_START;

        break;
    }
    }
}

static void _update(UpdateType type) {

    u8 time_passed;

    if (_done[type]) {
        return;
    }

    // ---

    time_passed = g_teopl_interrupt_ticks_total - _time_start[type] >= _time_to_pass[type];

    if (time_passed) {

        if (type == UPDATE_TYPE_SPECIAL) {

            _special_time_passed();
        }
        else if (type == UPDATE_TYPE_TALK) {

            _talk_time_passed();
        }
    }
}

// ---

void teopl_events_init() {

    u8 i;

    // ---

    // general
    // ---

    for (i = 0; i < UPDATE_TYPE_NUM; ++i) {
        _done[i] = 0;
    }

    // talk
    // ---

    _talk_time_to_pass_map[0] = TEOPL_UTIL_SECONDS_TO_TICKS(5);
    _talk_time_to_pass_map[1] = TEOPL_UTIL_SECONDS_TO_TICKS(12);
    _talk_time_to_pass_map[2] = TEOPL_UTIL_SECONDS_TO_TICKS(19);
    _talk_time_to_pass_map[3] = TEOPL_UTIL_SECONDS_TO_TICKS(26);

    _talk_index_text_end = g_teopl_level_data.texts_num - g_teopl_level_data.texts_num_welcome;

    _talk_state_id = STATE_TALK_WAIT_TO_START;
    _set_wait(_talk_time_to_pass_map[0], UPDATE_TYPE_TALK);

    _talk_index_text_start = 0;

    // special
    // ---

    _special_index_action = ACTION_NUM;

    _special_time_to_pass_before = 500;
    _special_time_to_pass_after = 500;

    _set_wait(_special_time_to_pass_before, UPDATE_TYPE_SPECIAL);

    _special_state_id = STATE_SPECIAL_WAIT_TO_START;

    _special_index_action = _special_action_choose();
}

// ---

void teopl_events_time_start_set(u32 time) {

    u8 type;

    for (type = 0; type < UPDATE_TYPE_NUM; ++type) {

        _time_start[type] = time;
    }
}

void teopl_events_update(TeoplLogicState state) {

    // REMARK: no events here
    if (state == TEOPL_LOGIC_STATE_LEVEL_WELCOME) {
        return;
    }

    // talk
    // ---

    if (state == TEOPL_LOGIC_STATE_LEVEL_PLAYABLE) {

        if (_talk_index_text_end > 0) {
            _update(UPDATE_TYPE_TALK);
        }
    }

    // special
    // ---

    if (g_teopl_level_data.level_special) {

        _update(UPDATE_TYPE_SPECIAL);
    }
}
