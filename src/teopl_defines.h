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

#ifndef _TEOPL_DEFINES_H_
#define _TEOPL_DEFINES_H_

#include <types.h>

#include "assets_menu.h"
#include "teopl_pool.h"
#include "teopl_sound.h"

// FIXME: optimize! (set constants instad of calculations)

// pool
// ###
#define TEOPL_SOUND_LARGEST_SONG 720
#define TEOPL_DEFINES_POOL_LEVEL                                        ((u8*)g_teopl_pool_anything + TEOPL_SOUND_LARGEST_SONG)
#define TEOPL_DEFINES_POOL_TEXTS_ENCODED                                ((u8*)g_teopl_pool_anything + TEOPL_SOUND_LARGEST_SONG + 325)

// main
// ###

#define TEOPL_DEFINES_MAIN_MENU_X_B                                     27
#define TEOPL_DEFINES_MAIN_MENU_Y_PX                                    100
#define TEOPL_DEFINES_MAIN_MENU_TEXT_X_B                                (TEOPL_DEFINES_MAIN_MENU_X_B - 2)
#define TEOPL_DEFINES_MAIN_MENU_TEXT_Y_PX                               (TEOPL_DEFINES_MAIN_MENU_Y_PX - MENU_TEXT_H - 16)
#define TEOPL_DEFINES_MAIN_KEYBOARD_X_B                                 (TEOPL_DEFINES_MAIN_MENU_X_B + 18)
#define TEOPL_DEFINES_MAIN_KEYBOARD_Y_PX                                (TEOPL_DEFINES_MAIN_MENU_Y_PX + 7)
#define TEOPL_DEFINES_MAIN_PASSWORD_X_B                                 (TEOPL_DEFINES_MAIN_MENU_X_B + 1)
#define TEOPL_DEFINES_MAIN_PASSWORD_Y_PX                                (TEOPL_DEFINES_MAIN_MENU_Y_PX + 24)

// sound
// ###

// TABLE 1
#define TEOPL_DEFINES_SOUND_SFX_player_destroys_chip                    TEOPL_SOUND_FILE_SFX_MAIN_01
#define TEOPL_DEFINES_SOUND_SFX_player_takes_key                        TEOPL_SOUND_FILE_SFX_MAIN_02
#define TEOPL_DEFINES_SOUND_SFX_player_stop_on_action                   TEOPL_SOUND_FILE_SFX_MAIN_04
#define TEOPL_DEFINES_SOUND_SFX_chip_removing_from_inventory            TEOPL_SOUND_FILE_SFX_MAIN_02
#define TEOPL_DEFINES_SOUND_SFX_player_next_activated                   TEOPL_SOUND_FILE_SFX_MAIN_02
#define TEOPL_DEFINES_SOUND_SFX_player_current_poofed                   TEOPL_SOUND_FILE_SFX_MAIN_03
#define TEOPL_DEFINES_SOUND_SFX_block_create_poofed                     TEOPL_SOUND_FILE_SFX_MAIN_03
#define TEOPL_DEFINES_SOUND_SFX_door_open                               TEOPL_SOUND_FILE_SFX_MAIN_04
#define TEOPL_DEFINES_SOUND_SFX_enemy_breaks_tile                       TEOPL_SOUND_FILE_SFX_MAIN_05
#define TEOPL_DEFINES_SOUND_SFX_letter_typed                            TEOPL_SOUND_FILE_SFX_MAIN_05
#define TEOPL_DEFINES_SOUND_SFX_door_hit_with_no_key                    TEOPL_SOUND_FILE_SFX_MAIN_06
#define TEOPL_DEFINES_SOUND_SFX_entity_hits_frame                       TEOPL_SOUND_FILE_SFX_MAIN_07
#define TEOPL_DEFINES_SOUND_SFX_skip_text_with_esc                      TEOPL_SOUND_FILE_SFX_MAIN_07
#define TEOPL_DEFINES_SOUND_SFX_player_hits_captured                    TEOPL_SOUND_FILE_SFX_MAIN_08
#define TEOPL_DEFINES_SOUND_SFX_player_hits_chip_inactive               TEOPL_SOUND_FILE_SFX_MAIN_08
#define TEOPL_DEFINES_SOUND_SFX_menu_select_password                    TEOPL_SOUND_FILE_SFX_MAIN_08
#define TEOPL_DEFINES_SOUND_SFX_skip_text_with_space                    TEOPL_SOUND_FILE_SFX_MAIN_08
#define TEOPL_DEFINES_SOUND_SFX_plant_digesting                         TEOPL_SOUND_FILE_SFX_MAIN_09
#define TEOPL_DEFINES_SOUND_SFX_frame_animate_pass_enemy_pill           TEOPL_SOUND_FILE_SFX_MAIN_0A
#define TEOPL_DEFINES_SOUND_SFX_menu_navigation                         TEOPL_SOUND_FILE_SFX_MAIN_0B
#define TEOPL_DEFINES_SOUND_SFX_player_hits_block                       TEOPL_SOUND_FILE_SFX_MAIN_0B
#define TEOPL_DEFINES_SOUND_SFX_player_hits_inactive                    TEOPL_SOUND_FILE_SFX_MAIN_0B
#define TEOPL_DEFINES_SOUND_SFX_player_hits_glass                       TEOPL_SOUND_FILE_SFX_MAIN_0B
#define TEOPL_DEFINES_SOUND_SFX_level_end                               TEOPL_SOUND_FILE_SFX_MAIN_0C
#define TEOPL_DEFINES_SOUND_SFX_goal_animating                          TEOPL_SOUND_FILE_SFX_MAIN_0C
#define TEOPL_DEFINES_SOUND_SFX_menu_select_start                       TEOPL_SOUND_FILE_SFX_MAIN_0C

// video
// ###

#ifdef ENTERPRISE
#define TEOPL_DEFINES_VIDEO_PARAM_BORDER_DEFAULT                        0x00
#else
#define TEOPL_DEFINES_VIDEO_PARAM_BORDER_DEFAULT                        0x54
#endif

#define TEOPL_DEFINES_VIDEO_PARAM_COLORS_NUM                            4
#define TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B                              4
#define TEOPL_DEFINES_VIDEO_PARAM_SCREEN_WIDTH_B                        80
#define TEOPL_DEFINES_VIDEO_PARAM_SCREEN_HEIGHT_PX                      200

// level
// ###

#define TEOPL_DEFINES_LEVEL_SPECIAL_FREQUENCY                           3
#define TEOPL_DEFINES_LEVEL_TILE_W_B                                    2
#define TEOPL_DEFINES_LEVEL_TILE_H_PX                                   8
#define TEOPL_DEFINES_LEVEL_LOGO_X_B                                    52
#define TEOPL_DEFINES_LEVEL_LOGO_Y_PX                                   0
#define TEOPL_DEFINES_LEVEL_PASS_X_B                                    52
#define TEOPL_DEFINES_LEVEL_PASS_Y_PX                                   32
#define TEOPL_DEFINES_LEVEL_INVENTORY1_X_B                              3
#define TEOPL_DEFINES_LEVEL_INVENTORY2_X_B                              0

// input
// ###

#define TEOPL_DEFINES_INPUT_FREEZE_S                                    30
#define TEOPL_DEFINES_INPUT_FREEZE_M                                    70
#define TEOPL_DEFINES_INPUT_FREEZE_L                                    120
#define TEOPL_DEFINES_INPUT_FREEZE_XL                                   180
#define TEOPL_DEFINES_INPUT_FREEZE_XXL                                  320

#define TEOPL_DEFINES_INPUT_FREEZE_INF                                  (u32)(-1)

// entity
// ###

// speeds (per direction type)
// ---

// none (TEOPL_ENTITY_DIRECTION_TYPE_NONE)
#define TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_NONE                       255

// move (TEOPL_DEFINES_ENTITY_DIRECTION_UP/DOWN/LEFT/RIGHT)
#define TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_player_active         12
#define TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_player_inactive       8
//#define TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_inventory_removing    24
#define TEOPL_DEFINES_ENTITY_TICKS_DIRECTION_MOVE_enemy                 60

// speeds (per animation type)
// ---

// none (TEOPL_ENTITY_ANIMATION_TYPE_NONE)
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_NONE                       255

// idle (TEOPL_ENTITY_ANIMATION_TYPE_IDLE)
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_player                180
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_captured_freeing      20
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_chip_activating       24
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_glass_breaking        18
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_door_gone             16
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_plant_digesting       180
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_goal_activated        16
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_IDLE_poof                  16

// move (TEOPL_ENTITY_ANIMATION_TYPE_MOVE)
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_MOVE_player_active         14
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_MOVE_player_inactive       10
#define TEOPL_DEFINES_ENTITY_TICKS_ANIMATION_MOVE_enemy                 48

// ---

#define TEOPL_DEFINES_ENTITY_ANIMATION_LOOP_FOREVER                     254

#endif
