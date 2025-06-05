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

#include "teopl_physics.h"

#include "teopl_collision.h"
#include "teopl_memory.h"
#include "teopl_level.h"
#include "teopl_logic.h"
#include "teopl_interrupt.h"
#include "teopl_video.h"

// ---

void teopl_physics_init() {

    teopl_collision_init();
}

void teopl_physics_update() {

    u8 i, num;
    TEOPL_MEMORY_POINTER_TYPE output;
    TeoplEntity* entity_updating;

    // ---

    num = g_teopl_level_data.entites.num_elements;

    for (i = 0; i < num; ++i) {

        teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output);
        entity_updating = (TeoplEntity*)output;

        // update move and collision
        if (entity_updating->move_direction != TEOPL_ENTITY_DIRECTION_TYPE_NONE && !g_teopl_util_freeze) {
            entity_updating->move_ticks += g_teopl_interrupt_tick_dt_taken;
            teopl_entity_position_and_collision_update(entity_updating);
        }

        // update animation
        if (entity_updating->anim_type != TEOPL_ENTITY_ANIMATION_TYPE_NONE) {
            entity_updating->anim_ticks += g_teopl_interrupt_tick_dt_taken;
            teopl_entity_animation_update(entity_updating);
        }
    }
}
