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

#include "teopl_logic.h"

#include "teopl_interrupt.h"
#include "teopl_sound.h"

static TeoplLogicState      _state;

// ---

static void _teopl_logic_state_change(TeoplLogicState state_old, TeoplLogicState state_new) {

    u8 song_old;
    u8 song_new;

    if (state_old != state_new) {

        // change song?
        song_old = teopl_sound_song_file_index_get();
        _state = state_new;
        song_new = teopl_sound_song_file_index_get();

        if (song_old != song_new) {
            teopl_interrupt_and_sound(0);
        }

        // reset clock?
        teopl_interrupt_tick_dt_reset();
    }
}

// ---

TeoplLogicState teopl_logic_state_get() {
    return _state;
}

void teopl_logic_state_set(TeoplLogicState state) {

    if (_state != state) {
        _teopl_logic_state_change(_state, state);
    }

}

void teopl_logic_init() {
    _state = TEOPL_LOGIC_STATE_NUM;
}
