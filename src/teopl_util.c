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

#include "teopl_util.h"

#include "cpctelera_wrap.h"
#include <types.h>

#include "teopl_interrupt.h"
#include "teopl_video.h"
#include "teopl_defines.h"

#ifdef WINCPCTELERA
#include "windows.h"
#include <math.h>

extern void ZX7Decrunch(const u8* source, u8* target) __z88dk_callee;

#endif

static u32      _random;

u32             g_teopl_util_freeze;

// ---

void teopl_util_draw_studio_logo(u8 x_b, u8 y_px) {
    teopl_font_sync_print(x_b, y_px, 48, "()SMOBIT_<@AMES");
}

u32 teopl_util_random32(u32 max_plus_one) {

    _random += cpct_nextRandom_mxor_u32(g_teopl_interrupt_ticks_total + cpct_rand8()); // REMARK: rand8 added to have better seed
    
    return _random % max_plus_one;
}

/*
void teopl_util_sec_to_time_string(u32 sec, char* time_string) {

    // "00:00:00"
    char    str_hrs[3], str_min[3], str_sec[3];
    u8      len_hrs, len_min, len_sec;
    u8      hrs, min;

    // ---

    hrs = (u8)(sec / 3600);
    min = (u8)((sec - hrs * 3600) / 60);
    sec = (sec - hrs * 3600 - min * 60);

    cpct_memcpy(time_string, "00:00:00", TEOPL_UTIL_TIME_STRING_LEN);

    teopl_util_u32_to_string(str_hrs, &len_hrs, hrs, 0);
    teopl_util_u32_to_string(str_min, &len_min, min, 0);
    teopl_util_u32_to_string(str_sec, &len_sec, sec, 0);

    cpct_memcpy(time_string + (2 - len_hrs), str_hrs, len_hrs);
    cpct_memcpy(time_string + 3 + (2 - len_min), str_min, len_min);
    cpct_memcpy(time_string + 6 + (2 - len_sec), str_sec, len_sec);
}
*/
void teopl_util_u32_to_string(char* string, u8* string_len, u32 number, u8 hex) {

    char    _string[TEOPL_UTIL_U32_STRING_LEN];     // needed for 0x FF FF FF FF + '\0'
    u8      i = 9;                                  // last digit place for max number
    u8      len;
    u8      system = hex ? 16 : 10;
    char    digit_start;
    u8      digit_offset;

    // ---

    do {
        digit_offset = (number % system);
        if (digit_offset > 9) {
            digit_offset -= 10;
            digit_start = 'A';
        }
        else {
            digit_start = '0';
        }
        _string[i--] = digit_start + digit_offset;
        number /= system;

    } while (number);

    len = 9 - i;

    cpct_memcpy(string, _string + i + 1, len);
    *(string + len) = 0;

    if (string_len) {
        *string_len = len;
    }
}

// REMARK : string must be null terminated
u32 teopl_util_string_to_u32(u8* str, u8* str_len, u8 hex) {

    u32 result = 0;
    u32 position = 1;
    u8 c;
    u32 digit;
    u32 system = hex ? 16 : 10;
    char* s = str;

    // find string end (or first non digit)
    while (*(++s)) {
        c = *s;
        digit = ((c >= '0' && c <= '9') || (hex && (c >= 'A' && c <= 'F')));
        if (!digit) {
            break;
        }
    }

    *str_len = (s - str);

    while (s >= str) {

        c = *(--s);

        if ((digit = c - '0') < system) {
            result += digit * position;
            position *= system;
        }
        else if (hex && ((digit = c - 'A') < system)) {
            result += (10 + digit) * position;
            position *= system;
        }
        else {
            break;
        }
    }

    return result;
}

/*
u8 teopl_util_strings_equal(char* str1, char* str2) {

    u8 equal = 0;
    u8 i = 0;
    char c1;
    char c2;

    do {
        c1 = *str1++;
        c2 = *str2++;

        equal = (c1 == c2);

        if (!c1 || !c2 || !equal) {
            break;
        }
    } while (1);

    return equal;
}
*/

void teopl_util_wait_freeze() {

    teopl_interrupt_and_sound(1);

    while (g_teopl_util_freeze) {

        teopl_interrupt_tick_dt_get();
            
        // FIXME: remove
#ifdef WINCPCTELERA
        //printf("freeze: %5ld\n", g_teopl_util_freeze);
#endif
    }
}

u16 teopl_util_abs_diff(u16 x, u16 y) {
    if (x < y) {
        return y - x;
    }
    else {
        return x - y;
    }
}

/*
u8 teopl_util_strcmp(char *str1, char* str2) {

    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return *(u8*)str1 - *(u8*)str2;
}
*/

u8 teopl_util_character_is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '_';
}

#ifdef WINCPCTELERA
void teopl_util_decompress_zx7(u8* zip_end, u16 zip_size, u8* unzip_end, u16 unzip_size) {
#else
void teopl_util_decompress_zx7(u8* zip_end, u8* unzip_end) {
#endif

#ifdef WINCPCTELERA
    ZX7Decrunch(zip_end - zip_size + 1, unzip_end - unzip_size + 1);
#else
    cpct_zx7b_decrunch_s(unzip_end, zip_end);
#endif

}
