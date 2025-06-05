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

#ifndef _TEOPL_LOGIC_H_
#define _TEOPL_LOGIC_H_

#include <types.h>

typedef enum {

    // ---

    TEOPL_LOGIC_STATE_GAME_TITLE_MENU,
    TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD,

    TEOPL_LOGIC_STATE_LEVEL_WELCOME,
    TEOPL_LOGIC_STATE_LEVEL_PLAYABLE,

    TEOPL_LOGIC_STATE_NUM,

    TEOPL_LOGIC_STATE_INVALID = TEOPL_LOGIC_STATE_NUM

} TeoplLogicState;

// ---

void                    teopl_logic_init();

TeoplLogicState         teopl_logic_state_get();
void                    teopl_logic_state_set(TeoplLogicState state);

#endif
