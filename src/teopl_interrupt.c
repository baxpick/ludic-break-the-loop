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

#include "teopl_interrupt.h"

#include "cpctelera_wrap.h"
#include <types.h>

#include "teopl_sound.h"
#include "teopl_util.h"
#include "teopl_video.h"
#include "teopl_logic.h"

// ---

u32                 g_teopl_interrupt_ticks_total;

u8                  g_teopl_interrupt_tick_dt_taken;

u8                  g_teopl_interrupt_keyboard_event;

u8                  g_teopl_interrupt_started;

static u8           _tick;
static u8           _tick_dt_prepared;

u8                  g_teopl_interrupt_tick_step;

// ---

#ifdef WINCPCTELERA
u16 cpct_removeInterruptHandler();
#endif

static void teopl_interrupt_handler(void) {

    _tick_dt_prepared += g_teopl_interrupt_tick_step;
    _tick += g_teopl_interrupt_tick_step;

#ifndef ENTERPRISE
    if (_tick == 6) {

        _tick = 0;

        teopl_sound_play();
    }
#else
    _tick = 0;

    teopl_sound_play();
#endif

    cpct_scanKeyboard_if();

    if (!g_teopl_interrupt_keyboard_event) {

        if (cpct_isAnyKeyPressed_f()) {

            g_teopl_interrupt_keyboard_event = 1;
        }
    }
}

static void _reset_time() {

    _tick = 0;
    _tick_dt_prepared = 0;
    
    g_teopl_interrupt_tick_dt_taken = 0;
}

void teopl_interrupt_init() {

    g_teopl_interrupt_ticks_total = 0;

    g_teopl_interrupt_started = 0;

    g_teopl_interrupt_keyboard_event = 0;

#ifdef ENTERPRISE
    g_teopl_interrupt_tick_step = 6;
#else
    g_teopl_interrupt_tick_step = 1;
#endif

    _reset_time();
}

void teopl_interrupt_and_sound(u8 enable) {

    if (enable == 0) {
        if (g_teopl_interrupt_started == 1) {
            
            __asm__("halt");
            teopl_sound_stop();

            cpct_removeInterruptHandler();

            g_teopl_interrupt_started = 0;
        }
    }
    else {
        if (g_teopl_interrupt_started == 0) {

            _reset_time();

            // REMARK: forces song unpack & init and sets play flag
            teopl_sound_song_init(teopl_sound_song_file_index_get(), 1);

            // sync
            cpct_waitVSYNC();
            __asm__("halt");
            __asm__("halt");
            cpct_waitVSYNC();

            // ---

            cpct_setInterruptHandler(teopl_interrupt_handler);

            g_teopl_interrupt_started = 1;
        }
    }
}

void teopl_interrupt_tick_dt_reset() {

    _tick_dt_prepared = 0;
}

u8 teopl_interrupt_tick_dt_get() {

    u8 ticks = _tick_dt_prepared;

    // ---

    teopl_interrupt_tick_dt_reset();

    if (g_teopl_util_freeze) {
        if (ticks <= g_teopl_util_freeze) {
            g_teopl_util_freeze -= ticks;
        }
        else {
            g_teopl_util_freeze = 0;
        }
    }

    // ---

    g_teopl_interrupt_ticks_total += ticks;

    return ticks;
}
