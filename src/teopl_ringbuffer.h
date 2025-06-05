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

#ifndef _TEOPL_RINGBUFFER_H_
#define _TEOPL_RINGBUFFER_H_

#include "teopl_memory.h"

#include <types.h>

// REMARK: do not use large values (but not less then 32)
#define TEOPL_RINGBUFFER_LENGTH_MAX 32

typedef struct {

    u8 index_start;
    u8 index_end;
    u8 num_elements;

    TEOPL_MEMORY_POINTER_TYPE buffer[TEOPL_RINGBUFFER_LENGTH_MAX];

} TeoplRingBuffer;

// ---

void    teopl_ringbuffer_init(TeoplRingBuffer* ptr);

u8      teopl_ringbuffer_index_get(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE input);          // returns buffer size if not found

u8      teopl_ringbuffer_add(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE input);                // returns 0 if out of bounds
u8      teopl_ringbuffer_remove(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE input);             // returns 0 if not found
u8      teopl_ringbuffer_peek(TeoplRingBuffer* ptr, u8 index, TEOPL_MEMORY_POINTER_TYPE* output);   // returns 0 if out of bounds
u8      teopl_ringbuffer_pull(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE* output);             // returns 0 if empty

u8      teopl_ringbuffer_is_full(TeoplRingBuffer* ptr);
u8      teopl_ringbuffer_is_empty(TeoplRingBuffer* ptr);

#endif
