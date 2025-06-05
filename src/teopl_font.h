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

#ifndef _TEOPL_FONT_H_
#define _TEOPL_FONT_H_

#include <types.h>

// +107 (max text length), +1 (terminating char)
#define TEOPL_FONT_MAX_NUM_BYTES_PER_TEXT (107 + 1)

typedef enum {

    TEOPL_FONT_CHARACTER_DRAW_CHARACTER_YES,
    TEOPL_FONT_CHARACTER_DRAW_CHARACTER_NO

} TeoplFontCharacterDraw;

extern TeoplFontCharacterDraw   g_teopl_font_async_character_draw;
extern char                     g_teopl_font_async_character;

extern char                     g_teopl_font_async_text[TEOPL_FONT_MAX_NUM_BYTES_PER_TEXT];
extern u8                       g_teopl_font_async_text_offset;

extern u8                       g_teopl_font_async_x_b;
extern u8                       g_teopl_font_async_x_b_offset;

extern u8                       g_teopl_font_async_y_px;
extern u8                       g_teopl_font_async_y_px_offset;

extern u8                       g_teopl_font_async_w_b;

extern u16                      g_teopl_font_async_ticks;
extern u16                      g_teopl_font_async_ticks_initial_delay;
extern u16                      g_teopl_font_async_ticks_limit;

extern u8*                      g_teopl_font_async_set_flag_on_start;
extern u8*                      g_teopl_font_async_set_flag_on_finish;

// ---

void    teopl_font_init();

u8      teopl_font_char_to_index(char c);

void    teopl_font_draw_character(char c, u8 x_b, u8 y_px);

void    teopl_font_sync_print(u8 x_b, u8 y_px, u8 w_b, char* text);

// ---

void    teopl_font_async_init_parameters();
void    teopl_font_async_print_prepare(u8 x_b, u8 y_px, u8 w_b, u16 ticks_initial_delay, u8 ticks_limit, u8* set_flag_on_start, u8* set_flag_on_finish);
void    teopl_font_async_update();

void    teopl_font_async_letter_next();
void    teopl_font_async_letter_previous();

#endif // _TEOPL_FONT_H_
