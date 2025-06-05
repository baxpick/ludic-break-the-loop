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

#ifndef _TEOPL_UTIL_H_
#define _TEOPL_UTIL_H_

#include "cpctelera_wrap.h"

#include <types.h>

#define TEOPL_UTIL_MAX(X,Y) ((X) > (Y) ? (X) : (Y)) 

#ifdef WINCPCTELERA
#define TEOPL_UTIL_PTR(PTR, TYPE) ((TYPE*)(WinCpcTelera_GetMemPtr((i16)PTR)))
#else
#define TEOPL_UTIL_PTR(PTR, TYPE) ((TYPE*)PTR)
#endif

#define TEOPL_UTIL_SECONDS_TO_TICKS(X) (300*X)

#define TEOPL_UTIL_U32_STRING_LEN   11

#define TEOPL_UTIL_TIME_STRING_LEN  9

#define TEOPL_UTIL_BREAKPOINT       __asm__(".db 0xed, 0xff")

// ---

typedef enum {
    TEOPL_UTIL_ALGNMENT_TOP,
    TEOPL_UTIL_ALGNMENT_CENTER,
    TEOPL_UTIL_ALGNMENT_BOTTOM
} TeoplUtilAlignment;

typedef enum {
    TEOPL_UTIL_SYNC,
    TEOPL_UTIL_ASYNC
} TeoplUtilSynchronized;

// ---

extern u32  g_teopl_util_freeze;

// ---

void    teopl_util_draw_studio_logo(u8 x_b, u8 y_px);
u32     teopl_util_random32(u32 max_plus_one);
//void    teopl_util_sec_to_time_string(u32 sec, char* time_string);
void    teopl_util_u32_to_string(char* string, u8* string_len, u32 number, u8 hex);
u32     teopl_util_string_to_u32(u8* str, u8* str_len, u8 hex);
//u8      teopl_util_strings_equal(char* str1, char* str2);
void    teopl_util_wait_freeze();
//void    teopl_util_sleep_ticks_sync(u16 ticks);
u16     teopl_util_abs_diff(u16 x, u16 y);
//u8      teopl_util_strcmp(char *str1, char* str2);
u8      teopl_util_character_is_whitespace(char c);
#ifdef WINCPCTELERA
void    teopl_util_decompress_zx7(u8* zip_end, u16 zip_size, u8* unzip_end, u16 unzip_size);
#else
void    teopl_util_decompress_zx7(u8* zip_end, u8* unzip_end);
#endif

#endif
