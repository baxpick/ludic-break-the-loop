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

#ifndef _TEOPL_INPUT_H_
#define _TEOPL_INPUT_H_

#include "cpctelera_wrap.h"
#include <types.h>

#include "teopl_logic.h"

typedef enum {

    TEOPL_INPUT_ACTION,

    // REMARK: used to select inactive players
    TEOPL_INPUT_ACTION_UP,
    TEOPL_INPUT_ACTION_DOWN,
    TEOPL_INPUT_ACTION_LEFT,
    TEOPL_INPUT_ACTION_RIGHT,

    TEOPL_INPUT_UP,
    TEOPL_INPUT_DOWN,
    TEOPL_INPUT_LEFT,
    TEOPL_INPUT_RIGHT,
// FIXME: remove in final build
//#ifdef DEBUG
    TEOPL_INPUT_KEYBOARD_O,
    TEOPL_INPUT_KEYBOARD_P,
//#endif
    TEOPL_INPUT_KEYBOARD_R,
    TEOPL_INPUT_KEYBOARD_ESC,

    // ---

    _TEOPL_INPUT_KEYBOARD_NUM

} TeoplInputKeyboard;

// ---

void teopl_input_init();

void teopl_input_update(TeoplLogicState state);

#endif // _TEOPL_INPUT_H_
