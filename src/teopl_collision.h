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

#ifndef _TEOPL_COLLISION_H_
#define _TEOPL_COLLISION_H_

#include <types.h>

#include "teopl_entity.h"

// ---

void    teopl_collision_init();

u8      teopl_collision_check_entity_vs_entity(u8 e1_x_b, u8 e1_y_px/*, u8 e1_w_b, u8 e1_h_px*/, u8 e2_x_b, u8 e2_y_px/*, u8 e2_w_b, u8 e2_h_px*/);
u8      teopl_collision_check_entity_vs_frame(u8 e1_x_b, u8 e1_y_px, TeoplEntityDirection direction);

void    teopl_collision_apply_entity_vs_tile(TeoplEntity* entity_hitting, u8* tile_hit);
void    teopl_collision_apply_entity_vs_entity(TeoplEntity* entity_hitting, TeoplEntity* entity_hit);
void    teopl_collision_apply_entity_vs_frame(TeoplEntity* entity_hitting, TeoplEntityDirection direction);

#endif // _TEOPL_COLLISION_H_
