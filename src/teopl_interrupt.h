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

#ifndef _TEOPL_INTERRUPT_H_
#define _TEOPL_INTERRUPT_H_

#include "cpctelera_wrap.h"
#include <types.h>

extern u32  g_teopl_interrupt_ticks_total;

extern u8   g_teopl_interrupt_tick_dt_taken;     // accumulated ticks taken for physics

extern u8   g_teopl_interrupt_keyboard_event;

extern u8   g_teopl_interrupt_started;

extern u8   g_teopl_interrupt_tick_step;

// ---

void    teopl_interrupt_init();

void    teopl_interrupt_and_sound(u8 enable);

void    teopl_interrupt_tick_dt_reset();

u8      teopl_interrupt_tick_dt_get();

#endif // _TEOPL_INTERRUPT_H_
