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

#include "teopl_variables.h"

#include "teopl_defines.h"
#include "teopl_video.h"
#include "teopl_level.h"
#include "teopl_pool.h"
#include "teopl_input.h"
#include "teopl_logic.h"
#include "teopl_physics.h"
#include "teopl_input.h"
#include "teopl_interrupt.h"
#include "teopl_font.h"
#include "teopl_sound.h"
#include "teopl_util.h"
#include "teopl_billboard.h"
#include "teopl_events.h"
#include "teopl_events.h"

#include "assets/sprites/font.h"
#include "assets_pictures_talk.h"

#include "assets_menu.h"

// ---

// FIXME: optimize by using local variables when accesing pointer values more then once

// FIXME: optimize by adding __z88dk_fastcall
// (used on functions that have exactly one argument of at most 32 bits.This argument will be passed in the lower bytes of dehl.It is now fully supported in sdcc.)

DEF_start(MAINHEADER)

// [0x0300 - 0x03FF] =>   256 bytes for video mask table (g_teopl_video_masktable)
#ifdef WINCPCTELERA
u8 g_teopl_video_masktable[256];
#else
DEF_u8_array_CONST(g_teopl_video_masktable, PASS_PARAMETERS((0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xEE, 0xEE, 0xCC, 0xCC, 0xAA, 0xAA, 0x88, 0x88, 0x66, 0x66, 0x44, 0x44, 0x22, 0x22, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktable2, PASS_PARAMETERS((0xDD, 0xCC, 0xDD, 0xCC, 0x99, 0x88, 0x99, 0x88, 0x55, 0x44, 0x55, 0x44, 0x11, 0x00, 0x11, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x88, 0x88, 0x88, 0x88, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktable4, PASS_PARAMETERS((0xBB, 0xAA, 0x99, 0x88, 0xBB, 0xAA, 0x99, 0x88, 0x33, 0x22, 0x11, 0x00, 0x33, 0x22, 0x11, 0x00, 0xAA, 0xAA, 0x88, 0x88, 0xAA, 0xAA, 0x88, 0x88, 0x22, 0x22, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktable6, PASS_PARAMETERS((0x99, 0x88, 0x99, 0x88, 0x99, 0x88, 0x99, 0x88, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktable8, PASS_PARAMETERS((0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0x66, 0x66, 0x44, 0x44, 0x22, 0x22, 0x00, 0x00, 0x66, 0x66, 0x44, 0x44, 0x22, 0x22, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktable0, PASS_PARAMETERS((0x55, 0x44, 0x55, 0x44, 0x11, 0x00, 0x11, 0x00, 0x55, 0x44, 0x55, 0x44, 0x11, 0x00, 0x11, 0x00, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktableB, PASS_PARAMETERS((0x33, 0x22, 0x11, 0x00, 0x33, 0x22, 0x11, 0x00, 0x33, 0x22, 0x11, 0x00, 0x33, 0x22, 0x11, 0x00, 0x22, 0x22, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00)))
DEF_u8_array_CONST(g_teopl_video_masktableD, PASS_PARAMETERS((0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)))
#endif

DEF_u8_array(g_teopl_pool_anything, TEOPL_POOL_MAX_ANYTHING)

DEF_end

char        g_teopl_font_async_text[TEOPL_FONT_MAX_NUM_BYTES_PER_TEXT];
const char  g_teopl_main_text[] = {
    'S','T','A','R','T','\0',
    'P','A','S','S','W','O','R','D','\0',
    'O',',',' ','P',' ','L','E','V','E','L',' ','N','A','V','I','G','A','T','I','O','N','\0',
    'T','H','E','R','E',' ','I','S',' ','N','O',' ','S','P','O','O','N','\0',
    'B','R','E','A','K',' ','T','H','E',' ','L','O','O','P','\0',
    'Q','U','B','I','T','S',':','\0'
};

u8          _title_drawn;

u16         g_main_num_chips_total;

// REMARK: 1 = blue, 2 = red
u8          g_teopl_main_pill;

// sprite flags
u8          g_teopl_main_player_sprites_fliped_left;
u8          g_teopl_main_player_inactive_sprites_fliped_left;
u8          g_teopl_main_enemy_sprites_fliped_left;

// loop flags
u8          g_teopl_main_loop_title_menu_exit;
u8          g_teopl_main_loop_title_password_exit;
u8          g_teopl_main_loop_game_exit;

u8          g_teopl_main_title_menu_selection;
u8          g_teopl_main_title_password_selection;
u8          g_teopl_main_title_password_string_index;

// ---

static void _update_all() {

    TeoplLogicState state = teopl_logic_state_get();

    // ---

    teopl_interrupt_and_sound(1);

    g_teopl_interrupt_tick_dt_taken = teopl_interrupt_tick_dt_get();

    // 1. update
    // ---

    if (!g_teopl_util_freeze) {
        teopl_input_update(state);  // (update human input)
    }
    // FIXME: remove
#ifdef WINCPCTELERA
    else {
        //printf("freeze: %5ld\n", g_teopl_util_freeze);
    }
#endif   

    teopl_events_update(state);     // (update events)
    teopl_font_async_update();

    teopl_physics_update();         // (move, collide, animate) 
}

static void _draw_all() {

    cpct_waitVSYNC();

    teopl_level_draw_entities();

    teopl_video_draw_letter();
}

// ---

static void _draw_and_loop_title_menu() {

    u8 selection = 255;

    // ---

    // title menu
    // === === ===

    // draw "break the loop" text
    if (g_teopl_main_pill & 2) {
        teopl_font_sync_print(TEOPL_DEFINES_MAIN_MENU_TEXT_X_B - 2, TEOPL_DEFINES_MAIN_MENU_TEXT_Y_PX + MENU_TEXT_H + 2, 32, g_teopl_main_text + 37);
    }
    else {
        teopl_font_sync_print(TEOPL_DEFINES_MAIN_MENU_TEXT_X_B + 1, TEOPL_DEFINES_MAIN_MENU_TEXT_Y_PX + MENU_TEXT_H + 2, 32, g_teopl_main_text + 55);
    }

    // retrodev
    teopl_font_sync_print(TEOPL_DEFINES_MAIN_MENU_TEXT_X_B - 1, 176, 32, "CPCRETRODEV 2019");

    // draw title menu images
    if (!_title_drawn) {
        teopl_video_draw_sprite_compressed_end(
            menu_text_end, menu_text_size_z, menu_text_size,
            TEOPL_DEFINES_MAIN_MENU_TEXT_X_B, TEOPL_DEFINES_MAIN_MENU_TEXT_Y_PX,
            MENU_TEXT_W, MENU_TEXT_H
        );
        _title_drawn = 1;
    }

    // text
    teopl_font_sync_print(TEOPL_DEFINES_MAIN_MENU_X_B, TEOPL_DEFINES_MAIN_MENU_Y_PX, 32, g_teopl_main_text);
    teopl_font_sync_print(TEOPL_DEFINES_MAIN_MENU_X_B, TEOPL_DEFINES_MAIN_MENU_Y_PX + font_HEIGHT_PX, 32, g_teopl_main_text + 6);

    if (g_teopl_main_pill) {
        teopl_font_sync_print(TEOPL_DEFINES_MAIN_MENU_TEXT_X_B - 6, 200 - font_HEIGHT_PX, 48, g_teopl_main_text + 15);
    }

    teopl_util_draw_studio_logo(0, 0);

    // loop
    // ---

    while (!g_teopl_main_loop_title_menu_exit) {

        // 1. update
        // ---

        _update_all();

        // 2. draw
        // ---

        _draw_all();

        // 3. draw special
        // ---

        if (selection != g_teopl_main_title_menu_selection) {

            // selection
            if (selection != 255) {
                teopl_video_draw_box_color(
                    0x00,
                    TEOPL_DEFINES_MAIN_MENU_X_B - font_WIDTH_B, TEOPL_DEFINES_MAIN_MENU_Y_PX + (selection - 1) * font_HEIGHT_PX,
                    font_WIDTH_B, font_HEIGHT_PX
                );

                teopl_billboard_draw_picture_next_for(TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC);
            }

            teopl_font_draw_character(
                '>',
                TEOPL_DEFINES_MAIN_MENU_X_B - font_WIDTH_B, TEOPL_DEFINES_MAIN_MENU_Y_PX + (g_teopl_main_title_menu_selection - 1) * font_HEIGHT_PX
            );

            selection = g_teopl_main_title_menu_selection;
        }
    }

    // on game start wait now and on password selected wait after keyboard is drawn
    if (g_teopl_main_title_menu_selection == 1) {
        teopl_util_wait_freeze();
    }
}

static char _password_selection_to_char(u8 selection) {

    char c;

    if (selection < 10) {
        c = '0' + selection;
    }
    else {
        c = 'A' + selection - 10;
    }

    return c;
}

static void _draw_password_keyboard(char selection, u8 draw_or_delete, u8 x_b, u8 y_px) {

    x_b += (selection % 4 * font_WIDTH_B);
    y_px += 3 * font_HEIGHT_PX - (selection / 4 * font_HEIGHT_PX);

    if (draw_or_delete) {
        teopl_font_draw_character(_password_selection_to_char(selection), x_b, y_px);
    }
    else {
        teopl_video_draw_box_color(0x00, x_b, y_px, font_WIDTH_B, font_HEIGHT_PX);
    }
}

static void _draw_and_loop_title_password() {

    u8 i;
    char c;

    u8 selection = g_teopl_main_title_password_selection;
    u8 string_index;

    u8 draw = 1;
    u8 time_blink = 0;

    // ---

    string_index = 0;
    g_teopl_main_title_password_string_index = 0;

    cpct_memset(g_teopl_level_password, '0', TEOPL_LEVEL_PASSWORD_LENGTH + 1);

    // title password
    // === === ===

    // draw
    // ---

    for (i = 0; i < 0x10; ++i) {
        _draw_password_keyboard(i, 1, TEOPL_DEFINES_MAIN_KEYBOARD_X_B, TEOPL_DEFINES_MAIN_KEYBOARD_Y_PX);
    }

    teopl_video_draw_box_color(
        0x00,
        TEOPL_DEFINES_MAIN_PASSWORD_X_B - 1, TEOPL_DEFINES_MAIN_PASSWORD_Y_PX - TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B,
        font_WIDTH_B * (TEOPL_LEVEL_PASSWORD_LENGTH + 1),
        font_HEIGHT_PX + 2 * TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B
    );

    teopl_video_draw_box_frame(
        &g_teopl_video_frame_data_password,
        TEOPL_DEFINES_MAIN_PASSWORD_X_B - 1, TEOPL_DEFINES_MAIN_PASSWORD_Y_PX - TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B,
        font_WIDTH_B * (TEOPL_LEVEL_PASSWORD_LENGTH + 1),
        font_HEIGHT_PX + 2 * TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B,
        0
    );

    // loop
    // ---

    // REMARK: wait on freeze set before, but after drawing
    teopl_util_wait_freeze();

    g_teopl_interrupt_tick_dt_taken = 0;
    time_blink = 0;

    teopl_interrupt_tick_dt_reset();
    while (!g_teopl_main_loop_title_password_exit) {

        // 1. update
        // ---

        _update_all();

        // 2. draw
        // ---

        _draw_all();

        // 3. draw special
        // ---

        time_blink += g_teopl_interrupt_tick_dt_taken;

        if (time_blink > 30) {
            time_blink = 0;
            draw = !draw;

            _draw_password_keyboard(g_teopl_main_title_password_selection, draw, TEOPL_DEFINES_MAIN_KEYBOARD_X_B, TEOPL_DEFINES_MAIN_KEYBOARD_Y_PX);
        }

        if (selection != g_teopl_main_title_password_selection) {

            _draw_password_keyboard(selection, 1, TEOPL_DEFINES_MAIN_KEYBOARD_X_B, TEOPL_DEFINES_MAIN_KEYBOARD_Y_PX);
            selection = g_teopl_main_title_password_selection;
        }

        if (string_index != g_teopl_main_title_password_string_index) {

            // typing password
            if (g_teopl_main_title_password_string_index != 255) {

                c = _password_selection_to_char(g_teopl_main_title_password_selection);

                teopl_font_draw_character(c, TEOPL_DEFINES_MAIN_PASSWORD_X_B + string_index * font_WIDTH_B, TEOPL_DEFINES_MAIN_PASSWORD_Y_PX);

                g_teopl_level_password[string_index] = c;
                string_index = g_teopl_main_title_password_string_index;

                if (string_index == TEOPL_LEVEL_PASSWORD_LENGTH) {
                    g_teopl_main_title_password_string_index = 255;
                }
            }
            // finished typing password
            else {

                g_teopl_main_title_password_string_index = 0;
                g_teopl_main_loop_title_password_exit = 1;
                g_teopl_main_loop_title_menu_exit = 0;

                g_teopl_level_password[TEOPL_LEVEL_PASSWORD_LENGTH] = 0;
            }
        }
    }

    teopl_video_draw_box_color(0x00, TEOPL_DEFINES_MAIN_KEYBOARD_X_B, TEOPL_DEFINES_MAIN_KEYBOARD_Y_PX, font_WIDTH_B * 4, font_HEIGHT_PX * 4);

    teopl_video_draw_box_frame(
        &g_teopl_video_frame_data_password,
        TEOPL_DEFINES_MAIN_PASSWORD_X_B - 1, TEOPL_DEFINES_MAIN_PASSWORD_Y_PX - TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B,
        font_WIDTH_B * (TEOPL_LEVEL_PASSWORD_LENGTH + 1),
        font_HEIGHT_PX + 2 * TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B,
        1
    );
}

static void _draw_and_loop_game() {

    u8 level_index;

    // ---

    // game
    // === === ===

    // draw
    // ---

    level_index = teopl_level_index_get(g_teopl_level_password);

    // reset password if not internet level
    if (level_index != 255) {
        *g_teopl_level_password = 0; // reset because it will be loaded (once) from the level
    }

    // password correct?
    if (level_index > 0) {

        teopl_video_game_freeze_animating_frame(0);
    }

    teopl_video_screen_clear();
    teopl_video_set(TEOPL_VIDEO_TYPE_GAME);

    teopl_level_load_all(level_index, 1);

    // loop
    // ---

    g_teopl_interrupt_tick_dt_taken = 0;
    teopl_interrupt_tick_dt_reset();

    // sync
    cpct_waitVSYNC();
    __asm__("halt");
    __asm__("halt");
    cpct_waitVSYNC();

    while (!g_teopl_main_loop_game_exit) {

        // 1. draw
        // ---

        _draw_all();

        // 2. update
        // ---

        _update_all();
    }
}

// ---

void main() {

    cpct_disableFirmware(); // not to interfere with code

    // one time init
    // ---

    teopl_interrupt_init();

    g_teopl_main_pill = 0;

    g_teopl_util_freeze = 0;

    g_teopl_sound_initialized = 0;

    // we start from left flipped sprites
    g_teopl_main_player_sprites_fliped_left = 1;
    g_teopl_main_player_inactive_sprites_fliped_left = 1;
    g_teopl_main_enemy_sprites_fliped_left = 1;

    teopl_video_init();

    // ---

    while (1) {

        // init
        // ---

        g_main_num_chips_total = 0;

        teopl_pool_init();
        teopl_font_init();
        teopl_input_init();
        teopl_logic_init();
        teopl_physics_init();
        teopl_sound_init();
        teopl_level_init();
        teopl_events_init();

        // ---

        g_teopl_main_title_menu_selection = 1; // START
        g_teopl_main_title_password_selection = 0; // '0'
        g_teopl_main_title_password_string_index = 0;

        g_teopl_main_loop_title_menu_exit = 0;
        g_teopl_main_loop_title_password_exit = 0;

        g_teopl_main_loop_game_exit = 0;

        teopl_video_screen_clear();
        teopl_video_set(TEOPL_VIDEO_TYPE_TITLE_MENU);

        // REMARK: starts interrupt and song playback
        teopl_logic_state_set(TEOPL_LOGIC_STATE_GAME_TITLE_MENU);

        _title_drawn = 0;

        teopl_billboard_draw_picture_next_for(TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC);

        // loop
        // ---

        teopl_interrupt_tick_dt_reset();
        while (!g_teopl_main_loop_title_menu_exit || !g_teopl_main_loop_title_password_exit) {

            // fill in password from last level loaded
            // (except when back from first level where pass length is 1 & not cleared)
            if (g_teopl_level_data.level_password[0] != 0 && g_teopl_level_data.level_password[1] != '\0' && !g_teopl_main_pill) {

                cpct_memcpy(g_teopl_level_password, g_teopl_level_data.level_password, TEOPL_LEVEL_PASSWORD_LENGTH + 1);
                teopl_font_sync_print(TEOPL_DEFINES_MAIN_PASSWORD_X_B, TEOPL_DEFINES_MAIN_PASSWORD_Y_PX, 32, g_teopl_level_password);

                *(g_teopl_level_data.level_password) = 0; // clear pass
            }

            if (!g_teopl_main_loop_title_menu_exit) {
                teopl_logic_state_set(TEOPL_LOGIC_STATE_GAME_TITLE_MENU);
                _draw_and_loop_title_menu();
            }

            if (!g_teopl_main_loop_title_password_exit) {
                teopl_logic_state_set(TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD);
                _draw_and_loop_title_password();
            }
        }

        _draw_and_loop_game();
    }
}

// ### ### ### ### ###
// # GLOBALS (start) #
// ### ### ### ### ###

DEF_start(MAIN)

DEF_u8_array_CONST(g_teopl_video_palette_normal, PASS_PARAMETERS((COLOR_NORMAL_1_none, COLOR_NORMAL_2_Magenta, COLOR_NORMAL_3_Mauve, COLOR_NORMAL_4_Bright_Magenta)))
DEF_u8_array_CONST(g_teopl_video_palette_blue, PASS_PARAMETERS((COLOR_BLUE_1_none, COLOR_BLUE_2_Magenta, COLOR_BLUE_3_Mauve, COLOR_BLUE_4_Bright_Magenta)))
DEF_u8_array_CONST(g_teopl_video_palette_red, PASS_PARAMETERS((COLOR_RED_1_none, COLOR_RED_2_Magenta, COLOR_RED_3_Mauve, COLOR_RED_4_Bright_Magenta)))

DEF_end

// ##################################################################################################################################

// Put here code which should be close to main
// ### ### ### ### ### ### ### ### ### ### ###

// Some code for EP should be below 0xC000 (if it uses LPT)
// (like setBorder and setPalette or change active screen)

// ---

void teopl_cpct_setPALColour(u8 pen, u8 ink) __z88dk_callee __naked {

#ifdef WINCPCTELERA

    cpct_setPALColour(pen, ink);

#else

    // get params from stack
    __asm__("pop iy");  // ret address first because __z88dk_callee convention
    __asm__("pop hl");
    __asm__("push iy"); // ret address last  because __z88dk_callee convention

#ifdef ENTERPRISE
    __asm__("di");
    __asm__("bit 3, l");
    __asm__("ret nz");
    __asm__("bit 4, l");
    __asm__("jr nz, border");
    __asm__("in a, (#0xb3)");
    __asm__("push af");
    __asm__("ld a, #0xff");
    __asm__("out (#0xb3), a");
    __asm__("push de");
    
    // __asm__("ld a, (#0xc007)");
    //__asm__("ld b, a");
    __asm__("ld b,#0x64");

    __asm__("ld a, h");
    __asm__("set 3, l");
    __asm__("ld h, #0xc0");
    __asm__("ld de, #0x0010");
    __asm__("fillcol1:");
    __asm__("ld (hl), a");
    __asm__("add hl, de");
    __asm__("ld (hl), a");
    __asm__("add hl, de");
    __asm__("djnz fillcol1");
    __asm__("pop de");
    __asm__("pop af");
    __asm__("out (#0xb3), a");
    
    // REMARK: Comment this if you don't want to enable interrupts (this is cpctelera default)
    __asm__("ei");
    
    __asm__("ret");
    __asm__("border:");
    __asm__("ld a, h");
    __asm__("out (#0x81), a");

    // REMARK: Comment this if you don't want to enable interrupts (this is cpctelera default)
    __asm__("ei");

    __asm__("ret");

#else
    __asm__("ld b, #0x7F");
    __asm__("out(c), l");
    __asm__("ld a, #0x40");
    __asm__("or h");
    __asm__("out(c), a");
    __asm__("ret");

#endif // ENTERPRISE

#endif // WINCPCTELERA
}

void teopl_cpct_setBorder(u8 ink) {

    teopl_cpct_setPALColour(16, ink);
}

void teopl_cpct_setPalette(u8* colour_array, u16 size) __z88dk_callee __naked {

#ifdef WINCPCTELERA

    cpct_setPalette(colour_array, size);

#else

    // get params from stack
    __asm__("pop iy");  // ret address first because __z88dk_callee convention
    __asm__("pop hl");
    __asm__("pop de");
    __asm__("push iy"); // ret address last  because __z88dk_callee convention

#ifdef ENTERPRISE
    __asm__("ld a,e");
    __asm__("cp #0x0a");
    __asm__("jr c,palvalok");
    __asm__("ld a,#0x09");
    __asm__("palvalok:");
    __asm__("ld c,a");
    __asm__("di");
    __asm__("in a,(#0xb3)");
    __asm__("push af");
    __asm__("ld a,#0xff");
    __asm__("out (#0xb3),a");
    __asm__("ex de,hl");
    __asm__("ld hl,#0xc008");
    __asm__("fillpal:");
    
    //__asm__("ld a,(#0xc007)");
    //__asm__("ld b,a");
    __asm__("ld b,#0x64");
    
    __asm__("ld a,(de)");
    __asm__("push de");
    __asm__("push hl");
    __asm__("ld de,#0x0010");
    __asm__("fillcol1_new:");
    __asm__("ld (hl),a");
    __asm__("add hl,de");
    __asm__("ld (hl),a");
    __asm__("add hl,de");
    __asm__("djnz fillcol1_new");
    __asm__("pop hl");
    __asm__("pop de");
    __asm__("inc l");
    __asm__("inc de");
    __asm__("bit 4,l");
    __asm__("jr nz,bias");
    __asm__("dec c");
    __asm__("jr nz,fillpal");
    __asm__("jr nobias");
    __asm__("bias:");
    __asm__("dec c");
    __asm__("jr z,nobias");
    __asm__("ld a,(de)");
    __asm__("out (#0x80),a");
    __asm__("nobias:");
    __asm__("pop af");
    __asm__("out (#0xb3),a");

    // REMARK: Comment this if you don't want to enable interrupts (this is cpctelera default)
    __asm__("ei");

    __asm__("ret");

#else
    __asm__("dec e");
    __asm__("add hl, de");
    __asm__("ld b, #0x7F");
    __asm__("svp_setPens_loop:");
    __asm__("out (c), e");
    __asm__("ld a, (hl)");
    __asm__("and #0x1F");
    __asm__("or #0x40");
    __asm__("out (c), a");
    __asm__("dec hl");
    __asm__("dec e");
    __asm__("jp p, svp_setPens_loop");
    __asm__("ret");

#endif // ENTERPRISE

#endif // WINCPCTELERA
}
