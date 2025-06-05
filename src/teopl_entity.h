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

#ifndef _TEOPL_ENTITY_H_
#define _TEOPL_ENTITY_H_

#include <types.h>

#include "teopl_ringbuffer.h"

#include "assets/sprites/blocks.h"

// REMARK: don not change this enum!
// direction types
typedef enum {

    TEOPL_ENTITY_DIRECTION_TYPE_NONE,

    TEOPL_ENTITY_DIRECTION_TYPE_UP,
    TEOPL_ENTITY_DIRECTION_TYPE_DOWN,
    TEOPL_ENTITY_DIRECTION_TYPE_LEFT,
    TEOPL_ENTITY_DIRECTION_TYPE_RIGHT,

    TEOPL_ENTITY_DIRECTION_NUM

} TeoplEntityDirection;

// animation types
typedef enum {

    TEOPL_ENTITY_ANIMATION_TYPE_AUTOMATIC, // used to indicate not forcing type (decide based od direction)

    // ---

    TEOPL_ENTITY_ANIMATION_TYPE_NONE,

    TEOPL_ENTITY_ANIMATION_TYPE_IDLE,
    TEOPL_ENTITY_ANIMATION_TYPE_MOVE

} TeoplEntityAnimationType;

// draw types
typedef enum {

    TEOPL_ENTITY_DRAW_TYPE_MASKED,
    TEOPL_ENTITY_DRAW_TYPE_NONMASKED

} TeoplEntityDrawType;

typedef enum {

    TEOPL_ENTITY_TYPE_PLAYER_INACTIVE,
    TEOPL_ENTITY_TYPE_PLAYER_ACTIVE,
    TEOPL_ENTITY_TYPE_PLAYER_ACTIVE_SELECTED_POOF,
    TEOPL_ENTITY_TYPE_PLAYER_FREEING_POOF,    

    TEOPL_ENTITY_TYPE_TILE_PLAYERCAPTURED_FREEING,

    TEOPL_ENTITY_TYPE_TILE_CHIP_ACTIVATING,

    TEOPL_ENTITY_TYPE_TILE_GLASS_BREAKING,

    TEOPL_ENTITY_TYPE_TILE_DOOR_GONE,

    TEOPL_ENTITY_TYPE_TILE_GOAL_ACTIVATED,

    TEOPL_ENTITY_TYPE_ENEMY,
    
    TEOPL_ENTITY_TYPE_BLOCK_CREATE_POOF,

    TEOPL_ENTITY_TYPE_LEVEL_REMOVING,
    TEOPL_ENTITY_TYPE_INVENTORY_ADDING,
    TEOPL_ENTITY_TYPE_INVENTORY_REMOVING,

    TEOPL_ENTITY_TYPE_PLANT_DIGESTING,

    TEOPL_ENTITY_TYPE_NONE = 255

} TeoplEntityType;

typedef struct {

    TeoplEntityType         type;

    // draw
    u8                      draw;
    u8                      draw_type;

    u8                      x_b;
    u8                      y_px;

    u8                      x_b_prev;
    u8                      y_px_prev;

    //u8                      w_b;
    //u8                      h_px;

    u8*                     sprite;

    // animation
    TeoplEntityAnimationType anim_type;
    u8                      anim_loops;
    u8                      anim_frame;
    u8                      anim_frame_first;
    u8                      anim_frame_last_plus_one;
    u8                      anim_ticks;
    u8                      anim_ticks_limit;           // when reached, go to next frame

    // move
    TeoplEntityDirection    move_direction;

    u8                      move_ticks;
    u8                      move_ticks_limit;           // when reached, move ppb pixels (4px in mode 1)

    // special

    TEOPL_MEMORY_POINTER_TYPE data1;                    // some data to pass on
    TEOPL_MEMORY_POINTER_TYPE data2;                    // some data to pass on

} TeoplEntity;

// util
// ---

TeoplEntity*            teopl_entity_find_first(TeoplRingBuffer* entities, TeoplEntityType type);

u8                      teopl_entity_is_player(TeoplEntity* entity);
u8                      teopl_entity_is_enemy(TeoplEntity* entity);

// drawing
// ---

void                    teopl_entity_draw(TeoplEntity* entity);

// moving
// ---

u8                      teopl_entity_is_movable(TeoplEntity* entity);

void                    teopl_entity_stop(TeoplEntity* entity);

TeoplEntityDirection    teopl_entity_direction_oposite_get(TeoplEntityDirection move_direction);

void                    teopl_entity_position_and_collision_update(TeoplEntity* entity_hitting);
void                    teopl_entity_position_move(TeoplEntity* entity, TeoplEntityDirection move_direction);
void                    teopl_entity_position_old_clear(TeoplEntity* entity, u8 force);

// animating
// ---

void                    teopl_entity_animation_set(TeoplEntity* entity, TeoplEntityAnimationType type);
void                    teopl_entity_animation_update(TeoplEntity* entity_animating);

// moving & animating

void                    teopl_entity_sprite_direction_and_animation_set(TeoplEntity* entity, TeoplEntityDirection move_direction, TeoplEntityAnimationType force_anim_type);
TeoplEntityDirection    teopl_entity_sprite_direction_and_animation_set_random_non_blocked(u8 x_b, u8 y_px, u8 blocked_by_block_tiles);

// ---

u8                      teopl_entity_direction_blocked_or_static(TeoplEntity* entity_skip, u8 x_b, u8 y_px, TeoplEntityDirection direction, u8 blocked_by_block_tiles);

#endif
