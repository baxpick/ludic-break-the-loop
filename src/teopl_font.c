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

#include "teopl_font.h"

#include "cpctelera_wrap.h"

#include "teopl_video.h"
#include "teopl_sound.h"
#include "teopl_util.h"
#include "teopl_interrupt.h"
#include "teopl_pool.h"
#include "teopl_billboard.h"
#include "teopl_logic.h"

#include "assets/sprites/font.h"

// ---

TeoplFontCharacterDraw      g_teopl_font_async_character_draw;
char                        g_teopl_font_async_character;

u8                          g_teopl_font_async_text_offset;

u8                          g_teopl_font_async_x_b;
u8                          g_teopl_font_async_x_b_offset;

u8                          g_teopl_font_async_y_px;
u8                          g_teopl_font_async_y_px_offset;

u8                          g_teopl_font_async_w_b;

u16                         g_teopl_font_async_ticks;
u16                         g_teopl_font_async_ticks_initial_delay;
u16                         g_teopl_font_async_ticks_limit;

u8*                         g_teopl_font_async_set_flag_on_start;
u8*                         g_teopl_font_async_set_flag_on_finish;

// ---

void teopl_font_async_print_prepare(u8 x_b, u8 y_px, u8 w_b, u16 ticks_initial_delay, u8 ticks_limit, u8* set_flag_on_start, u8* set_flag_on_finish) {

    g_teopl_font_async_x_b = x_b;
    g_teopl_font_async_y_px = y_px;
    g_teopl_font_async_w_b = w_b;

    g_teopl_font_async_ticks = 0;
    g_teopl_font_async_ticks_initial_delay = ticks_initial_delay;
    g_teopl_font_async_ticks_limit = ticks_limit;

    g_teopl_font_async_character = *g_teopl_font_async_text;

    g_teopl_font_async_set_flag_on_start = set_flag_on_start;
    g_teopl_font_async_set_flag_on_finish = set_flag_on_finish;
}

void teopl_font_async_init_parameters() {

    g_teopl_font_async_ticks = 0;
    g_teopl_font_async_ticks_initial_delay = 0;
    g_teopl_font_async_ticks_limit = 0;

    g_teopl_font_async_character = 0;

    g_teopl_font_async_text_offset = 0;
    g_teopl_font_async_x_b_offset = 0;
    g_teopl_font_async_y_px_offset = 0;

    g_teopl_font_async_character_draw = TEOPL_FONT_CHARACTER_DRAW_CHARACTER_NO;
}

void teopl_font_init() {

    teopl_font_async_init_parameters();
}

// convert ascii character to font sprite index
u8 teopl_font_char_to_index(char c) {

    u8 i = c;

    if (c < 33) {
        i = 0;
    }
    else if (c < 39) {
        i -= 33;
    }
    else if (c < 44) {
        i -= 38;
    }
    else if (c < 48) {
        i -= 40;
    }
    else if (c < 60) {
        i -= 41;
    }
    else if (c < 62) {
        i -= 42;
    }
    else if (c < 91) {
        i -= 43;
    }
    else {
        i = 0;
    }

    return i;
}

void teopl_font_draw_character(char c, u8 x_b, u8 y_px) {

    c = teopl_font_char_to_index(c);

    // ---

    cpct_drawSprite(
        (void*)(g_font + c * font_BYTES_PER_SPRITE),
        TEOPL_VIDEO_GET_LOCATION(x_b, y_px),
        font_WIDTH_B, font_HEIGHT_PX
    );
}

void teopl_font_sync_print(u8 x_b, u8 y_px, u8 w_b, char* text) {

    char    c = 0;
    u8      x_b_offset = 0;
    u8      y_px_offset = 0;

    // ---

    while (c = *text) {

        u8 is_whitespace = teopl_util_character_is_whitespace(c);
        u8 is_whitespace_prev = teopl_util_character_is_whitespace(*(text - 1));

        // ---

        // hypernation
        if (x_b_offset > w_b) {

            if (!is_whitespace) {
                if (!is_whitespace_prev) {
                    teopl_font_draw_character('-', x_b + x_b_offset, y_px + y_px_offset);
                }

                text--;
            }

            y_px += font_HEIGHT_PX;
            x_b_offset = 0;
        }
        else {

            if (!is_whitespace) {

                // draw letter
                teopl_font_draw_character(c, x_b + x_b_offset, y_px + y_px_offset);

                x_b_offset += font_WIDTH_B;
            }
            else if (c == '\n' || c == '_') {
                y_px += font_HEIGHT_PX;
                x_b_offset = 0;
            }
            else {
                x_b_offset += font_WIDTH_B;
            }
        }

        text++;
    }
}

void teopl_font_async_update() {

    u16 limit = g_teopl_font_async_ticks_limit + g_teopl_font_async_ticks_initial_delay;

    // ---

    if (g_teopl_font_async_character != 0) {

        g_teopl_font_async_ticks += g_teopl_interrupt_tick_dt_taken;

        if (g_teopl_font_async_ticks >= limit) {

            u8 is_whitespace = teopl_util_character_is_whitespace(g_teopl_font_async_character);

            // ---

            g_teopl_font_async_ticks -= limit;

            g_teopl_font_async_ticks_initial_delay = 0;

            // play sound
            if (!is_whitespace) {
            }

            // ---

            g_teopl_font_async_character_draw = TEOPL_FONT_CHARACTER_DRAW_CHARACTER_YES;
        }
    }
}

void teopl_font_async_letter_next() {

    g_teopl_font_async_text_offset++;
    g_teopl_font_async_character = *(g_teopl_font_async_text + g_teopl_font_async_text_offset);
}

void teopl_font_async_letter_previous() {

    g_teopl_font_async_text_offset--;
    g_teopl_font_async_character = *(g_teopl_font_async_text + g_teopl_font_async_text_offset);
}
