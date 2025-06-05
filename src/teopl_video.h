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

#ifndef _TEOPL_VIDEO_H_
#define _TEOPL_VIDEO_H_

#include "cpctelera_wrap.h"

#include <types.h>

#include "teopl_billboard.h"

#define TEOPL_VIDEO_ADDRESS_SCREEN ((u8*)0xC000)
#define TEOPL_VIDEO_GET_LOCATION(X_B,Y_PX)      cpct_getScreenPtr(TEOPL_VIDEO_ADDRESS_SCREEN, X_B, Y_PX)

// palettes
#ifndef ENTERPRISE

#define COLOR_NORMAL_1_none				0x54
#define COLOR_NORMAL_2_Magenta			0x58
#define COLOR_NORMAL_3_Mauve			0x5D
#define COLOR_NORMAL_4_Bright_Magenta	0x4D

#define COLOR_BLUE_1_none				0x54
#define COLOR_BLUE_2_Magenta			0x40
#define COLOR_BLUE_3_Mauve			    0x47
#define COLOR_BLUE_4_Bright_Magenta	    0x43

#define COLOR_RED_1_none				0x54
#define COLOR_RED_2_Magenta			    0x46
#define COLOR_RED_3_Mauve			    0x59
#define COLOR_RED_4_Bright_Magenta	    0x43

#else

#define COLOR_NORMAL_1_none				0x00
#define COLOR_NORMAL_2_Magenta			0x05
#define COLOR_NORMAL_3_Mauve			0x25
#define COLOR_NORMAL_4_Bright_Magenta	0x6D

#define COLOR_BLUE_1_none				0x00
#define COLOR_BLUE_2_Magenta			0x07
#define COLOR_BLUE_3_Mauve			    0x4F
#define COLOR_BLUE_4_Bright_Magenta	    0xDF

#define COLOR_RED_1_none				0x00
#define COLOR_RED_2_Magenta			    0x06
#define COLOR_RED_3_Mauve			    0x97
#define COLOR_RED_4_Bright_Magenta	    0xDF

// #define COLOR_none				0x00
// #define COLOR_dark				0x01
// #define COLOR_light_low			0xc9
// #define COLOR_light_high		0xdb

#endif

typedef enum {

    TEOPL_VIDEO_MODE_0 = 0,
    TEOPL_VIDEO_MODE_1 = 1,
    TEOPL_VIDEO_MODE_2 = 2

} TeoplVideoMode;

typedef enum {

    TEOPL_VIDEO_TYPE_TITLE_MENU,
    TEOPL_VIDEO_TYPE_GAME

} TeoplVideoType;

typedef struct {

    //u8 is_rich;

    u8 ch1; // , ch2, ch3, ch4;  // color horizontal
    u8 cv1; // , cv2, cv3, cv4;  // color vertical

} TeoplVideoFrameData;

typedef struct {

    u8 color_horizontal;    // color horizontal
    u8 color_vertical;      // color vertical

} TeoplVideoFrameSimpleData;

// ---

extern TeoplVideoMode   g_teopl_video_mode;

// ---

extern TeoplVideoFrameData g_teopl_video_frame_data_talk_ai;
extern TeoplVideoFrameData g_teopl_video_frame_data_talk_ludic;
extern TeoplVideoFrameData g_teopl_video_frame_data_password;
extern TeoplVideoFrameData g_teopl_video_frame_data_level;

cpctm_declareMaskTable(g_teopl_video_masktable);

// ---

// functions defined in main.c
void                    teopl_cpct_setBorder(u8 ink);
void                    teopl_cpct_setPalette(u8* colour_array, u16 size) __z88dk_callee __naked;

// variables defined in main.c
extern u8               g_teopl_video_palette_normal[];
extern u8               g_teopl_video_palette_blue[];
extern u8               g_teopl_video_palette_red[];

void                    teopl_video_screen_clear();

TeoplVideoFrameData*    teopl_video_talk_frame_data_get(TeoplBillboardPictureType picture_type);

void                    teopl_video_init();
void                    teopl_video_set(TeoplVideoType video_type);

void                    teopl_video_matrix(u8 pill);

void                    teopl_video_mode_set(TeoplVideoMode mode);

void                    teopl_video_draw_background(u8 x_b, u8 y_px);
void                    teopl_video_draw_box_pattern(u8 pattern, u8 x_b, u8 y_px, u8 w_b, u8 h_px);
void                    teopl_video_draw_box_color(u8 color, u8 x_b, u8 y_px, u8 w_b, u8 h_px);
void                    teopl_video_draw_box_frame(TeoplVideoFrameData* frame_data, u8 x_b, u8 y_px, u8 w_b, u8 h_px, u8 remove);

void                    teopl_video_draw_sprite_compressed_end(u8* sprite_zip_end, u16 sprite_zip_size, u16 sprite_unzip_size, u8 x_b, u8 y_px, u8 w_b, u8 h_px);

void                    teopl_video_draw_level(u8 x, u8 y, u8 width, u8 height);
void                    teopl_video_draw_level_tile(u8* tile);
void                    teopl_video_draw_letter();
//void                    teopl_video_draw();

void                    teopl_video_game_freeze_animating_frame(u8 frame_type);

#endif
