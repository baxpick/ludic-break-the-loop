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

#include "teopl_billboard.h"

#include "teopl_font.h"
#include "teopl_video.h"
#include "teopl_pool.h"
#include "teopl_logic.h"
#include "teopl_interrupt.h"
#include "teopl_events.h"
#include "teopl_defines.h"

#include "assets/sprites/font.h"

// ---

u8                          g_teopl_billboard_x_b;
u8                          g_teopl_billboard_y_px;

u8                          g_teopl_billboard_talking_started;
u8                          g_teopl_billboard_talking_finished;

// ---

static TeoplUtilAlignment   _billboard_alignment;

static u8                   _billboard_banner_has_frame;
//static u8                   _billboard_banner_has_picture;

static TeoplBillboardPictureType   _picture_type;
static u8                   _picture_index;
static u8*                  _picture_sprite;

static u8                   _picture_x_b;
static u8                   _picture_y_px;
static u8                   _picture_w_b;
static u8                   _picture_h_px;

// ---

void teopl_billboard_init() {

    g_teopl_billboard_talking_started = 0;
    g_teopl_billboard_talking_finished = 0;

    _billboard_banner_has_frame = 1;
    //_billboard_banner_has_picture = 1;

    // ---

    teopl_events_init();

    _picture_type = TEOPL_BILLBOARD_PICTURE_TYPE_NONE;
    _picture_index = 0;
    _picture_sprite = 0;
}

u8 teopl_billboard_is_talking() {

    return (g_teopl_billboard_talking_started == 1 && g_teopl_billboard_talking_finished == 0);
}

static void _picture_parameters_set() {

    TeoplLogicState state = teopl_logic_state_get();

    // ---

    switch (_picture_type) {

    case TEOPL_BILLBOARD_PICTURE_TYPE_LUDIC:
        if (_picture_index == 1) {
            _picture_w_b = LUDIC_TALK_01_W;
            _picture_h_px = LUDIC_TALK_01_H;
            _picture_sprite = g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - ludic_talk_01_size;
        }
        else if (_picture_index == 0) {
            _picture_w_b = LUDIC_TALK_00_W;
            _picture_h_px = LUDIC_TALK_00_H;
            _picture_sprite = g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - ludic_talk_01_size - ludic_talk_00_size;
        }

        if (state == TEOPL_LOGIC_STATE_GAME_TITLE_MENU || state == TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD) {
            _picture_x_b = TEOPL_DEFINES_MAIN_MENU_TEXT_X_B + MENU_TEXT_W;
        }
        else {
            _picture_x_b = g_teopl_billboard_x_b + BILLBOARD_WIDTH_B - _picture_w_b - 1;
        }
        break;

    case TEOPL_BILLBOARD_PICTURE_TYPE_AI:
        if (_picture_index == 1) {
            _picture_w_b = AI_TALK_00_W;
            _picture_h_px = AI_TALK_00_H;
            _picture_sprite = g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - ai_talk_01_size;
        }
        else if (_picture_index == 0) {
            _picture_w_b = AI_TALK_00_W;
            _picture_h_px = AI_TALK_00_H;
            _picture_sprite = g_teopl_pool_anything + TEOPL_POOL_MAX_ANYTHING - ai_talk_01_size - ai_talk_00_size;
        }

        if (state == TEOPL_LOGIC_STATE_GAME_TITLE_MENU || state == TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD) {
            _picture_x_b = TEOPL_DEFINES_MAIN_MENU_TEXT_X_B + MENU_TEXT_W;
        }
        else {
            _picture_x_b = g_teopl_billboard_x_b + 1;
        }
        break;
    }

    if (state == TEOPL_LOGIC_STATE_GAME_TITLE_MENU || state == TEOPL_LOGIC_STATE_GAME_TITLE_PASSWORD) {
        _picture_y_px = TEOPL_DEFINES_MAIN_MENU_TEXT_Y_PX + MENU_TEXT_H - LUDIC_TALK_00_H + 2;
    }
    else {
        _picture_y_px = g_teopl_billboard_y_px - _picture_h_px - 2; // (teopl_video_talk_frame_data_get(_picture_type)->is_rich ? TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B : 2);
    }
}

static void _picture_remove() {

    if (_picture_type != TEOPL_BILLBOARD_PICTURE_TYPE_NONE) {

        _picture_parameters_set();
        teopl_video_draw_box_color(0x00, _picture_x_b, _picture_y_px, _picture_w_b, _picture_h_px);
    }
}

void teopl_billboard_draw_picture_next() {

    //if (_billboard_banner_has_picture) {

        //_picture_remove();

        // extract needed graphics
        teopl_pool_extract(TEOPL_POOL_TYPE_PICTURES_TALK, _picture_type);

        _picture_index = (_picture_index + 1) % 2;

        _picture_parameters_set();

        cpct_drawSprite((void*)_picture_sprite, TEOPL_VIDEO_GET_LOCATION(_picture_x_b, _picture_y_px), _picture_w_b, _picture_h_px);
    //}
}

void teopl_billboard_draw_picture_next_for(TeoplBillboardPictureType picture_type) {

    _picture_type = picture_type;

    teopl_billboard_draw_picture_next();
}

void teopl_banner_remove() {

    _picture_remove();

    teopl_video_draw_box_frame(
        teopl_video_talk_frame_data_get(_picture_type),
        g_teopl_billboard_x_b, g_teopl_billboard_y_px,
        BILLBOARD_WIDTH_B, BILLBOARD_HEIGHT_PX, 1
    );
    
    teopl_video_draw_box_color(
        0x00,
        g_teopl_billboard_x_b - 1, g_teopl_billboard_y_px - TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B,
        BILLBOARD_WIDTH_B + 2, BILLBOARD_HEIGHT_PX + 2 * TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B
    );
}

void teopl_billboard_parameters_set(TeoplUtilAlignment alignment) {

    _billboard_alignment = alignment;

    g_teopl_billboard_x_b = TEOPL_BILLBOARD_LEVEL_CENTER_X_B;

    switch (alignment) {
    case TEOPL_UTIL_ALGNMENT_TOP:
        g_teopl_billboard_y_px = TEOPL_BILLBOARD_LEVEL_TOP_Y_PX;
        break;

    case TEOPL_UTIL_ALGNMENT_CENTER:
        g_teopl_billboard_y_px = TEOPL_BILLBOARD_LEVEL_CENTER_Y_PX;
        break;

    case TEOPL_UTIL_ALGNMENT_BOTTOM:
        g_teopl_billboard_y_px = TEOPL_BILLBOARD_LEVEL_BOTTOM_Y_PX;
        break;
    }
}

void teopl_billboard_say(TeoplUtilAlignment alignment, TeoplBillboardPictureType picture_type) {

    _picture_type = picture_type;

    g_teopl_billboard_talking_started = 1;
    g_teopl_billboard_talking_finished = 0;

    teopl_billboard_parameters_set(alignment);

    // draw frame
    if (_billboard_banner_has_frame) {
        teopl_video_draw_box_frame(
            teopl_video_talk_frame_data_get(_picture_type),
            g_teopl_billboard_x_b, g_teopl_billboard_y_px,
            BILLBOARD_WIDTH_B, BILLBOARD_HEIGHT_PX, 0
        );
    }

    // draw picture
    teopl_billboard_draw_picture_next();

    // prepare text
    teopl_font_async_print_prepare(
        g_teopl_billboard_x_b + TEOPL_BILLBOARD_PADDING_X_B, g_teopl_billboard_y_px + TEOPL_BILLBOARD_PADDING_Y_PX,
        BILLBOARD_WIDTH_B - TEOPL_BILLBOARD_PADDING_X_B * 2,
        0,
        TEOPL_BILLBOARD_TICKS_LETTER,
        &g_teopl_billboard_talking_started,
        &g_teopl_billboard_talking_finished
    );
}

u8 teopl_billboard_prepare_next_welcome_level_text(TeoplUtilAlignment alignment) {

    TeoplLevelData*         level_data = &g_teopl_level_data;
    u8                      next_index = level_data->text_welcome_next_index;

    TeoplBillboardPictureType      picture_type = TEOPL_BILLBOARD_PICTURE_TYPE_NONE;

    u8                      num_welcome = level_data->texts_num_welcome;

    // ---

    // there are more "welcome" texts to show
    if (next_index == 0 || (next_index < num_welcome)) {

        teopl_billboard_parameters_set(alignment);

        teopl_banner_remove();

        // ---

        picture_type = teopl_level_load_text((u8*)&g_teopl_font_async_text, level_data->text_welcome_next_index);

        teopl_billboard_say(alignment, picture_type);

        level_data->text_welcome_next_index++;

        return 1;
    }

    return 0;
}

void teopl_billboard_say_finish(u8 play_sound) {

    if (*g_teopl_font_async_text && g_teopl_font_async_character) {

        if (play_sound) {
            teopl_sound_sfx_set_with_priority(TEOPL_DEFINES_SOUND_SFX_skip_text_with_space, TEOPL_SOUND_SFX_PRIORITY_DEFAULT, TEOPL_SOUND_SFX_VOLUME_DEFAULT, 1, 0);
        }

        // stop async talk
        teopl_font_async_init_parameters();

        // say all in sync
        teopl_font_sync_print(g_teopl_font_async_x_b, g_teopl_font_async_y_px, g_teopl_font_async_w_b, g_teopl_font_async_text);

        // reset flags
        if (g_teopl_font_async_set_flag_on_start) {
            *g_teopl_font_async_set_flag_on_start = 0;
        }
        if (g_teopl_font_async_set_flag_on_finish) {
            *g_teopl_font_async_set_flag_on_finish = 0;
        }

        g_teopl_font_async_character = 0;
    }
}
