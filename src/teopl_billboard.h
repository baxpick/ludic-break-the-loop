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

#ifndef _TEOPL_BILLBOARD_H_
#define _TEOPL_BILLBOARD_H_

#include <types.h>

#include "teopl_util.h"
#include "teopl_level.h"
#include "teopl_defines.h"

#define BILLBOARD_WIDTH_B                       72
#define BILLBOARD_HEIGHT_PX                     32

#define TEOPL_BILLBOARD_LEVEL_TOP_Y_PX          TEOPL_DEFINES_LEVEL_TILE_H_PX

#define TEOPL_BILLBOARD_LEVEL_CENTER_X_B        ((TEOPL_DEFINES_VIDEO_PARAM_SCREEN_WIDTH_B - BILLBOARD_WIDTH_B) / 2)
#define TEOPL_BILLBOARD_LEVEL_CENTER_Y_PX       ((TEOPL_DEFINES_VIDEO_PARAM_SCREEN_HEIGHT_PX - BILLBOARD_HEIGHT_PX) / 2)

#define TEOPL_BILLBOARD_LEVEL_BOTTOM_Y_PX       (TEOPL_DEFINES_VIDEO_PARAM_SCREEN_HEIGHT_PX - BILLBOARD_HEIGHT_PX - TEOPL_DEFINES_LEVEL_TILE_H_PX)

#define TEOPL_BILLBOARD_PADDING_X_B             1
#define TEOPL_BILLBOARD_PADDING_Y_PX            TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B

#define TEOPL_BILLBOARD_TICKS_LETTER            25

// REMARK: do not change values! used as a trick to know what is extracted (see TEOPL_POOL_TYPE_PICTURES_TALK)
typedef enum {

    TEOPL_BILLBOARD_PICTURE_TYPE_NONE = 0,

    TEOPL_BILLBOARD_PICTURE_TYPE_AI = 1,
    TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC = 2

} TeoplBillboardPictureType;

// ---

extern u8               g_teopl_billboard_x_b;
extern u8               g_teopl_billboard_y_px;

extern u8               g_teopl_billboard_talking_started;
extern u8               g_teopl_billboard_talking_finished;

// ---

void    teopl_billboard_init();

u8      teopl_billboard_is_talking();

void    teopl_billboard_parameters_set(TeoplUtilAlignment alignment);

void    teopl_billboard_draw_picture_next_for(TeoplBillboardPictureType picture_type);
void    teopl_billboard_draw_picture_next();
void    teopl_billboard_say(TeoplUtilAlignment alignment, TeoplBillboardPictureType picture_type);
u8      teopl_billboard_prepare_next_welcome_level_text(TeoplUtilAlignment alignment);
void    teopl_billboard_say_finish(u8 play_sound);

void    teopl_banner_remove();

#endif
