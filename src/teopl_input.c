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

#include "teopl_input.h"

#include "teopl_defines.h"
#include "teopl_level.h"
#include "teopl_entity.h"
#include "teopl_video.h"
#include "teopl_logic.h"
#include "teopl_interrupt.h"
#include "teopl_sound.h"
#include "teopl_billboard.h"
#include "teopl_collision.h"

extern u8   g_teopl_main_pill;

// ---

extern u8   g_teopl_main_loop_title_menu_exit;
extern u8   g_teopl_main_loop_title_password_exit;
extern u8   g_teopl_main_loop_game_exit;

extern u8   g_teopl_main_title_menu_selection;
extern u8   g_teopl_main_title_password_selection;
extern u8   g_teopl_main_title_password_string_index;

// ---

static TeoplInputKeyboard   _input_keyboard;
static u8                   _action_button;

static void _update_action_button_state() {

    _action_button = cpct_isKeyPressed(Key_Space) || cpct_isKeyPressed(Joy0_Fire1);
}

void teopl_input_init() {

    _input_keyboard = _TEOPL_INPUT_KEYBOARD_NUM;
}

static void _teopl_input_update_for_title_menu() {

    switch (_input_keyboard) {

    case TEOPL_INPUT_UP:
        if (g_teopl_main_title_menu_selection == 2) {
            g_teopl_main_title_menu_selection = 1;
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 1);
        }
        break;

    case TEOPL_INPUT_DOWN:
        if (g_teopl_main_title_menu_selection == 1) {
            g_teopl_main_title_menu_selection = 2;
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 1);
        }
        break;

    case TEOPL_INPUT_ACTION:

        if (g_teopl_main_title_menu_selection == 1) {

            teopl_logic_state_set(TEOPL_LOGIC_STATE_INVALID);

            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_select_start, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

            g_teopl_main_loop_title_menu_exit = 1;
            g_teopl_main_loop_title_password_exit = 1;

            g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_XXL;
        }
        else if (g_teopl_main_title_menu_selection == 2) {

            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_select_password, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

            teopl_logic_state_set(TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD);

            g_teopl_main_loop_title_menu_exit = 1;
            g_teopl_main_loop_title_password_exit = 0;

            g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;
        }
        break;
    };
}

static void _teopl_input_update_for_title_password() {

    switch (_input_keyboard) {

        // game events
        // ---

    case TEOPL_INPUT_UP:
        if (g_teopl_main_title_password_selection / 4 < 3) {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 1);
            g_teopl_main_title_password_selection += 4;

            g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;
        }
        break;

    case TEOPL_INPUT_DOWN:
        if (g_teopl_main_title_password_selection / 4 > 0) {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 1);
            g_teopl_main_title_password_selection -= 4;

            g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;
        }
        break;

    case TEOPL_INPUT_LEFT:
        if (g_teopl_main_title_password_selection % 4 > 0) {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 1);
            g_teopl_main_title_password_selection--;

            g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;
        }
        break;

    case TEOPL_INPUT_RIGHT:
        if (g_teopl_main_title_password_selection % 4 < 3) {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 1);
            g_teopl_main_title_password_selection++;

            g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;
        }
        break;

    case TEOPL_INPUT_KEYBOARD_ESC:
        teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_menu_navigation, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

        g_teopl_main_loop_title_password_exit = 1;
        g_teopl_main_loop_title_menu_exit = 0;
        break;

    case TEOPL_INPUT_ACTION:

        teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_letter_typed, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

        // digit
        if (g_teopl_main_title_password_selection < 0x10) {
            if (g_teopl_main_title_password_string_index < TEOPL_LEVEL_PASSWORD_LENGTH) {
                g_teopl_main_title_password_string_index++;
            }
            else {
                g_teopl_main_title_password_string_index = 255;
            }
        }

        g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;

        break;
    };
}

static void _welcome_done() {

    teopl_util_wait_freeze();

    teopl_video_screen_clear();

    // continue loading level assets
    // ---

    // 1. load tiles
    teopl_level_load_tiles(g_teopl_level_data.level_index);

    // 2. extract entities
    teopl_level_entities_extract(0, 0, g_teopl_level_data.level_width, g_teopl_level_data.level_height);

    // draw level
    teopl_video_draw_level(0, 0, g_teopl_level_data.level_width, g_teopl_level_data.level_height);

    // ready to play
    teopl_logic_state_set(TEOPL_LOGIC_STATE_LEVEL_PLAYABLE);
}

static void _teopl_input_update_for_level_welcome() {

    switch (_input_keyboard) {

        // level events
        // ---

    case TEOPL_INPUT_ACTION:

        g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;

        // finish text and wait
        if (g_teopl_font_async_character) {

            teopl_billboard_say_finish(1);
        }
        // no more welcome texts ?
        else if (!teopl_billboard_prepare_next_welcome_level_text(TEOPL_UTIL_ALGNMENT_CENTER)) {
            _welcome_done();
        }

        break;

    case TEOPL_INPUT_KEYBOARD_ESC:

        g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_XXL;

        teopl_billboard_say_finish(1);

        _welcome_done();

        break;
    };
}

static void _teopl_input_update_for_level_playable() {

    u8 level_index = g_teopl_level_data.level_index;
    u8 levels_num = g_teopl_level_data.levels_num;
    TeoplEntity* player = g_teopl_level_data.entity_player;
    u8 special = g_teopl_level_data.level_special;

    u8 x_b;
    u8 y_px;

    TeoplEntityDirection direction = TEOPL_ENTITY_DIRECTION_TYPE_NONE;

    u8 aligned;
    u8 can_stop;
    u8 is_moving;

    // ---

    // forbid input while player is moving or not available
    if (!player) {
        return;
    }

    is_moving = player->move_direction != TEOPL_ENTITY_DIRECTION_TYPE_NONE;

    x_b = player->x_b;
    y_px = player->y_px;

    aligned = teopl_level_coord_aligned_absolute(x_b, y_px);

    can_stop = special && is_moving && aligned && g_teopl_level_data.level_special_num_chips_destroyed > 0;

    // REMARK: ignore events while moving (except on special level for action event + aligned + chips available)
    if (is_moving) {

        if (!special) {
            return;
        }
        else if (_input_keyboard != TEOPL_INPUT_ACTION || !can_stop) {
            return;
        }
    }

    switch (_input_keyboard) {

        // game events
        // ---

    case TEOPL_INPUT_ACTION:

        if (can_stop) {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_skip_text_with_space, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);

            teopl_entity_stop(player);

            teopl_level_remove_chip_from_inventory(0); // REMARK: 0 = after removal, *don't* continue with next
        }
        break;

        // FIXME: add pill in final build
    case TEOPL_INPUT_KEYBOARD_O:

        teopl_level_load_all(level_index == 0 ? levels_num - 1 : level_index - 1, 1);
        break;

    case TEOPL_INPUT_KEYBOARD_P:

        teopl_level_load_next();
        break;

    case TEOPL_INPUT_KEYBOARD_R:

#ifdef DEBUG
        teopl_video_debug_palette_set(g_teopl_video_debug_palette_c1, g_teopl_video_debug_palette_c2, g_teopl_video_debug_palette_c3, g_teopl_video_debug_palette_c4, 0);
#endif

        teopl_level_restart();
        break;

    case TEOPL_INPUT_KEYBOARD_ESC:

        g_teopl_main_loop_game_exit = 1;
        break;

        // entity events
        // ---

    case TEOPL_INPUT_UP:

        direction = TEOPL_ENTITY_DIRECTION_TYPE_UP;
        break;

    case TEOPL_INPUT_DOWN:

        direction = TEOPL_ENTITY_DIRECTION_TYPE_DOWN;
        break;

    case TEOPL_INPUT_LEFT:

        direction = TEOPL_ENTITY_DIRECTION_TYPE_LEFT;
        break;

    case TEOPL_INPUT_RIGHT:

        direction = TEOPL_ENTITY_DIRECTION_TYPE_RIGHT;
        break;

        // level events
        // ---

    case TEOPL_INPUT_ACTION_UP:
    case TEOPL_INPUT_ACTION_DOWN:
    case TEOPL_INPUT_ACTION_LEFT:
    case TEOPL_INPUT_ACTION_RIGHT:
    {
        TeoplEntityDirection direction = TEOPL_ENTITY_DIRECTION_TYPE_NONE;

        // ---

        if (_input_keyboard == TEOPL_INPUT_ACTION_UP) {
            direction = TEOPL_ENTITY_DIRECTION_TYPE_UP;
        }
        else if (_input_keyboard == TEOPL_INPUT_ACTION_DOWN) {
            direction = TEOPL_ENTITY_DIRECTION_TYPE_DOWN;
        }
        else if (_input_keyboard == TEOPL_INPUT_ACTION_LEFT) {
            direction = TEOPL_ENTITY_DIRECTION_TYPE_LEFT;
        }
        else if (_input_keyboard == TEOPL_INPUT_ACTION_RIGHT) {
            direction = TEOPL_ENTITY_DIRECTION_TYPE_RIGHT;
        }

        teopl_level_player_next(direction);
    }
    break;
    };

    if (!teopl_entity_direction_blocked_or_static(player, x_b, y_px, direction, 1)) {

        teopl_entity_sprite_direction_and_animation_set(player, direction, TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC);
    }
}

static u8 _teopl_input_keyboard_update(TeoplLogicState state) {

    u8                  is_valid_input = 255; // REMARK: guess that common event happened

    // ---

    // events common to all states
    // ---

    if (cpct_isKeyPressed(Key_M)) {

        teopl_sound_mute_song_toggle();
        g_teopl_util_freeze = TEOPL_DEFINES_INPUT_FREEZE_M;
    }
    else {
        is_valid_input = 1; // REMARK: guess that non common event happened
    }

    if (is_valid_input == 255) {
        return is_valid_input;
    }

    // ---

    switch (state) {

    case TEOPL_LOGIC_STATE_LEVEL_PLAYABLE:

        if (cpct_isKeyPressed(Key_CursorUp) || cpct_isKeyPressed(Joy0_Up)) {
            if (!_action_button) {
                _input_keyboard = TEOPL_INPUT_UP;
            }
            else {
                _input_keyboard = TEOPL_INPUT_ACTION_UP;
            }
        }
        else if (cpct_isKeyPressed(Key_CursorDown) || cpct_isKeyPressed(Joy0_Down)) {
            if (!_action_button) {
                _input_keyboard = TEOPL_INPUT_DOWN;
            }
            else {
                _input_keyboard = TEOPL_INPUT_ACTION_DOWN;
            }
        }
        else if (cpct_isKeyPressed(Key_CursorLeft) || cpct_isKeyPressed(Joy0_Left)) {
            if (!_action_button) {
                _input_keyboard = TEOPL_INPUT_LEFT;
            }
            else {
                _input_keyboard = TEOPL_INPUT_ACTION_LEFT;
            }
        }
        else if (cpct_isKeyPressed(Key_CursorRight) || cpct_isKeyPressed(Joy0_Right)) {
            if (!_action_button) {
                _input_keyboard = TEOPL_INPUT_RIGHT;
            }
            else {
                _input_keyboard = TEOPL_INPUT_ACTION_RIGHT;
            }
        }
        else if (_action_button) {
            _input_keyboard = TEOPL_INPUT_ACTION;
        }
        else if (cpct_isKeyPressed(Key_O) && g_teopl_level_data.level_index != 0 && g_teopl_main_pill) {
            _input_keyboard = TEOPL_INPUT_KEYBOARD_O;
        }
        else if (cpct_isKeyPressed(Key_P) && !teopl_level_is_ending() && g_teopl_main_pill) {
            _input_keyboard = TEOPL_INPUT_KEYBOARD_P;
        }
        else if (cpct_isKeyPressed(Key_R)) {
            _input_keyboard = TEOPL_INPUT_KEYBOARD_R;
        }
        else if (cpct_isKeyPressed(Key_Esc)) {
            _input_keyboard = TEOPL_INPUT_KEYBOARD_ESC;
        }
        else {
            is_valid_input = 0; // REMARK: no event happened
        }

        // ---

        if (is_valid_input) {
            _teopl_input_update_for_level_playable();
        }

        break;

    case TEOPL_LOGIC_STATE_GAME_TITLE_MENU:

        if (cpct_isKeyPressed(Key_CursorUp) || cpct_isKeyPressed(Joy0_Up)) {
            _input_keyboard = TEOPL_INPUT_UP;
        }
        else if (cpct_isKeyPressed(Key_CursorDown) || cpct_isKeyPressed(Joy0_Down)) {
            _input_keyboard = TEOPL_INPUT_DOWN;
        }
        else if (_action_button) {
            _input_keyboard = TEOPL_INPUT_ACTION;
        }
        else {
            is_valid_input = 0;
        }

        // ---

        if (is_valid_input) {
            _teopl_input_update_for_title_menu();
        }

        break;

    case TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD:

        if (cpct_isKeyPressed(Key_CursorUp) || cpct_isKeyPressed(Joy0_Up)) {
            _input_keyboard = TEOPL_INPUT_UP;
        }
        else if (cpct_isKeyPressed(Key_CursorDown) || cpct_isKeyPressed(Joy0_Down)) {
            _input_keyboard = TEOPL_INPUT_DOWN;
        }
        else if (cpct_isKeyPressed(Key_CursorLeft) || cpct_isKeyPressed(Joy0_Left)) {
            _input_keyboard = TEOPL_INPUT_LEFT;
        }
        else if (cpct_isKeyPressed(Key_CursorRight) || cpct_isKeyPressed(Joy0_Right)) {
            _input_keyboard = TEOPL_INPUT_RIGHT;
        }
        else if (_action_button) {
            _input_keyboard = TEOPL_INPUT_ACTION;
        }
        else if (cpct_isKeyPressed(Key_Esc)) {
            _input_keyboard = TEOPL_INPUT_KEYBOARD_ESC;
        }
        else {
            is_valid_input = 0;
        }

        // ---

        if (is_valid_input) {
            _teopl_input_update_for_title_password();
        }

        break;

    case TEOPL_LOGIC_STATE_LEVEL_WELCOME:

        if (_action_button) {
            _input_keyboard = TEOPL_INPUT_ACTION;
        }
        else if (cpct_isKeyPressed(Key_Esc)) {
            _input_keyboard = TEOPL_INPUT_KEYBOARD_ESC;
        }
        else {
            is_valid_input = 0;
        }

        // ---

        if (is_valid_input) {
            _teopl_input_update_for_level_welcome();
        }

        break;
    }

    return is_valid_input;
}

// ---

void teopl_input_update(TeoplLogicState state) {

    if (g_teopl_interrupt_keyboard_event) {

        _input_keyboard = _TEOPL_INPUT_KEYBOARD_NUM;

        _update_action_button_state();

        _teopl_input_keyboard_update(state); // REMARK: returns if event is valid and processed

        g_teopl_interrupt_keyboard_event = 0;
    }
}
