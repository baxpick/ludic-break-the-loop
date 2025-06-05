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

#include "teopl_ringbuffer.h"

void teopl_ringbuffer_init(TeoplRingBuffer* ptr) {

    ptr->index_start = 0;
    ptr->index_end = 0;
    ptr->num_elements = 0;
}

u8 teopl_ringbuffer_index_get(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE input) {

    u8 result = TEOPL_RINGBUFFER_LENGTH_MAX;

    u8 i, num = ptr->num_elements;
    TEOPL_MEMORY_POINTER_TYPE output;

    for (i = 0; i < num; ++i) {
        if (teopl_ringbuffer_peek(ptr, i, &output) && input == output) {
            result = i;
            break;
        }
    }

    return result;
}

u8 teopl_ringbuffer_add(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE input) {

    u8 index_end;

    if (teopl_ringbuffer_is_full(ptr)) {
        return 0;
    }

    index_end = (ptr->index_end + (u8)(teopl_ringbuffer_is_empty(ptr) ? 0 : 1)) % TEOPL_RINGBUFFER_LENGTH_MAX;

    ptr->buffer[index_end] = input;

    ptr->index_end = index_end;

    ptr->num_elements++;

    return 1;
}

// FIXME: better implementation
u8 teopl_ringbuffer_remove(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE input) {

    u8 i, num, found = 0;
    TEOPL_MEMORY_POINTER_TYPE output;

    TeoplRingBuffer tmp;
    teopl_ringbuffer_init(&tmp);

    num = ptr->num_elements;
    for (i = 0; i < num; ++i) {
        if (teopl_ringbuffer_peek(ptr, i, &output) && input != output) {
            teopl_ringbuffer_add(&tmp, output);
        }
        else {
            found = 1;
        }
    }

    teopl_ringbuffer_init(ptr);

    num--;
    for (i = 0; i < num; ++i) {
        if (teopl_ringbuffer_peek(&tmp, i, &output)) {
            teopl_ringbuffer_add(ptr, output);
        }
    }

    return found;
}

u8 teopl_ringbuffer_peek(TeoplRingBuffer* ptr, u8 index, TEOPL_MEMORY_POINTER_TYPE* output) {

    if (!teopl_ringbuffer_is_empty(ptr) && index <= ptr->num_elements - 1) {

        *output = ptr->buffer[((ptr->index_start + index) % TEOPL_RINGBUFFER_LENGTH_MAX)];

        return 1;
    }

    return 0;
}

u8 teopl_ringbuffer_pull(TeoplRingBuffer* ptr, TEOPL_MEMORY_POINTER_TYPE* output) {

    if (!teopl_ringbuffer_is_empty(ptr)) {

        *output = ptr->buffer[ptr->index_start];

        ptr->num_elements--;

        if (!teopl_ringbuffer_is_empty(ptr)) {
            ptr->index_start = (ptr->index_start + 1) % TEOPL_RINGBUFFER_LENGTH_MAX;
        }

        return 1;
    }

    return 0;
}

u8 teopl_ringbuffer_is_full(TeoplRingBuffer* ptr) {

    return ptr->num_elements == TEOPL_RINGBUFFER_LENGTH_MAX;
}

u8 teopl_ringbuffer_is_empty(TeoplRingBuffer* ptr) {

    return !ptr->num_elements;
}
