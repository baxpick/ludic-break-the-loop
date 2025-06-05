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

#include "teopl_video.h"

#include "cpctelera_wrap.h"

#include <types.h>

#include "teopl_level.h"
#include "teopl_entity.h"
#include "teopl_logic.h"
#include "teopl_interrupt.h"
#include "teopl_util.h"
#include "teopl_pool.h"
#include "teopl_billboard.h"
#include "teopl_defines.h"
#include "teopl_events.h"

#include "assets/sprites/font.h"
#include "assets_menu.h"



//cpctm_createTransparentMaskTable(g_teopl_video_masktable, 0x0100, M1, 0);

extern char         g_teopl_main_text[];

extern u16          g_main_num_chips_total;
extern u8           g_teopl_main_pill;

// FIXME: remove palette from sprites file (and from convert script generator)
static const u8 _palette_game[4];
// static const u8     _palette_game[16] = { 0x00, 0x04, 0x05, 0x08, 0x0f, 0x09, 0x13, 0x19, 0x10, 0x17, 0x14, 0x03, 0x06, 0x02, 0x07, 0x01 };
// static const u8     _palette_game_blue[16] = { 0x00, 0x0d, 0x10, 0x19, 0x0f, 0x09, 0x13, 0x19, 0x10, 0x17, 0x14, 0x03, 0x06, 0x02, 0x07, 0x01 };
// static const u8     _palette_game_red[16] = { 0x00, 0x0a, 0x16, 0x19, 0x0f, 0x09, 0x13, 0x19, 0x10, 0x17, 0x14, 0x03, 0x06, 0x02, 0x07, 0x01 };

// ---

TeoplVideoMode  g_teopl_video_mode;

// ---

TeoplVideoFrameData g_teopl_video_frame_data_talk_ai;
TeoplVideoFrameData g_teopl_video_frame_data_talk_ludic;
TeoplVideoFrameData g_teopl_video_frame_data_password;
TeoplVideoFrameData g_teopl_video_frame_data_level;

// ---

void teopl_video_screen_clear() {

    cpct_clearScreen(0);
}

TeoplVideoFrameData* teopl_video_talk_frame_data_get(TeoplBillboardPictureType picture_type) {

    if (picture_type == TEOPL_BILLBOARD_PICTURE_TYPE_AI) {
        return &g_teopl_video_frame_data_talk_ai;
    }
    else {
        return &g_teopl_video_frame_data_talk_ludic;
    }
}

void teopl_video_init() {

    /*
    g_teopl_video_frame_data_talk.is_rich = 1;

    g_teopl_video_frame_data_talk.ch1 = 1;
    g_teopl_video_frame_data_talk.ch2 = 3;
    g_teopl_video_frame_data_talk.ch3 = 2;
    g_teopl_video_frame_data_talk.ch4 = 1;

    g_teopl_video_frame_data_talk.cv1 = 2;
    g_teopl_video_frame_data_talk.cv2 = 3;
    g_teopl_video_frame_data_talk.cv3 = 1;
    g_teopl_video_frame_data_talk.cv4 = 2;
    */

    // frame for talk (ai)
    // ---

    //g_teopl_video_frame_data_talk_ai.is_rich = 0;

    g_teopl_video_frame_data_talk_ai.ch1 = 3;
    //g_teopl_video_frame_data_talk_ai.ch2 = 0;
    //g_teopl_video_frame_data_talk_ai.ch3 = 0;
    //g_teopl_video_frame_data_talk_ai.ch4 = 0;

    g_teopl_video_frame_data_talk_ai.cv1 = 3;
    //g_teopl_video_frame_data_talk_ai.cv2 = 0;
    //g_teopl_video_frame_data_talk_ai.cv3 = 0;
    //g_teopl_video_frame_data_talk_ai.cv4 = 0;

    // frame for talk (ludic)
    // ---

    //g_teopl_video_frame_data_talk_ludic.is_rich = 0;

    g_teopl_video_frame_data_talk_ludic.ch1 = 2;
    //g_teopl_video_frame_data_talk_ludic.ch2 = 0;
    //g_teopl_video_frame_data_talk_ludic.ch3 = 0;
    //g_teopl_video_frame_data_talk_ludic.ch4 = 0;

    g_teopl_video_frame_data_talk_ludic.cv1 = 2;
    //g_teopl_video_frame_data_talk_ludic.cv2 = 0;
    //g_teopl_video_frame_data_talk_ludic.cv3 = 0;
    //g_teopl_video_frame_data_talk_ludic.cv4 = 0;

    // frame for password
    // ---

    //g_teopl_video_frame_data_password.is_rich = 0;

    g_teopl_video_frame_data_password.ch1 = 3;
    //g_teopl_video_frame_data_password.ch2 = 0;
    //g_teopl_video_frame_data_password.ch3 = 0;
    //g_teopl_video_frame_data_password.ch4 = 0;

    g_teopl_video_frame_data_password.cv1 = 1;
    //g_teopl_video_frame_data_password.cv2 = 0;
    //g_teopl_video_frame_data_password.cv3 = 0;
    //g_teopl_video_frame_data_password.cv4 = 0;

    // frame for level
    // ---

    //g_teopl_video_frame_data_level.is_rich = 0;

    g_teopl_video_frame_data_level.ch1 = 2;
    //g_teopl_video_frame_data_level.ch2 = 0;
    //g_teopl_video_frame_data_level.ch3 = 0;
    //g_teopl_video_frame_data_level.ch4 = 0;

    g_teopl_video_frame_data_level.cv1 = 2;
    //g_teopl_video_frame_data_level.cv2 = 0;
    //g_teopl_video_frame_data_level.cv3 = 0;
    //g_teopl_video_frame_data_level.cv4 = 0;

    cpct_memcpy(_palette_game, g_teopl_video_palette_normal, 4);
}

void teopl_video_matrix(u8 pill) {

    if (pill == 1) {
        cpct_memcpy(_palette_game, g_teopl_video_palette_blue, 4);
    }
    else if (pill == 2) {
        cpct_memcpy(_palette_game, g_teopl_video_palette_red, 4);
    }
}

void teopl_video_set(TeoplVideoType video_type) {

    switch (video_type) {

    case TEOPL_VIDEO_TYPE_TITLE_MENU:
    case TEOPL_VIDEO_TYPE_GAME:
        teopl_video_mode_set(TEOPL_VIDEO_MODE_1);
        teopl_cpct_setBorder(TEOPL_DEFINES_VIDEO_PARAM_BORDER_DEFAULT);
        teopl_cpct_setPalette(_palette_game, 4);
        break;
    }
}

void teopl_video_mode_set(TeoplVideoMode mode) {

    g_teopl_video_mode = mode;

    cpct_setVideoMode(mode);
}

void teopl_video_draw_background(u8 x_b, u8 y_px) {

    /*
    u8 aligned_x = teopl_level_coord_aligned_x_b_absolute(x_b);
    u8 aligned_y = teopl_level_coord_aligned_y_px_absolute(y_px);

    TeoplLevelTileType tile;

    u8* sprite;

    // ---

    if (aligned_x && aligned_y) {
        tile = TEOPL_LEVEL_TILE_BACKGROUND_00_A;
    }
    else if (aligned_y) {
        tile = TEOPL_LEVEL_TILE_BACKGROUND_00_B;
    }
    else if (aligned_x) {
        tile = TEOPL_LEVEL_TILE_BACKGROUND_00_C;
    }
    else {
        tile = TEOPL_LEVEL_TILE_BACKGROUND_00_D;
    }

    sprite = (void*)(g_blocks + tile * blocks_BYTES_PER_SPRITE);

    cpct_drawSprite(
        sprite,
        TEOPL_VIDEO_GET_LOCATION(x_b, y_px), blocks_WIDTH_B, blocks_HEIGHT_PX
    );
    */

    if (g_teopl_main_pill == 1) {
        
        cpct_drawSprite(
            (void*)(g_blocks + TEOPL_LEVEL_TILE_BLOCK_01 * blocks_BYTES_PER_SPRITE),
            TEOPL_VIDEO_GET_LOCATION(x_b, y_px), blocks_WIDTH_B, blocks_HEIGHT_PX
        );
    }
    else {

        teopl_video_draw_box_color(0x00, x_b, y_px, blocks_WIDTH_B, blocks_HEIGHT_PX);
    }
}

void teopl_video_draw_box_pattern(u8 pattern, u8 x_b, u8 y_px, u8 w_b, u8 h_px) {

    u8* location = TEOPL_VIDEO_GET_LOCATION(x_b, y_px);
    u8  w_b_max = 64;

    // ---

    if (w_b > w_b_max) {
        cpct_drawSolidBox(location, pattern, 64, h_px);
        teopl_video_draw_box_pattern(pattern, x_b + w_b_max, y_px, w_b - w_b_max, h_px);
    }
    else {
        cpct_drawSolidBox(location, pattern, w_b, h_px);
    }
}

void teopl_video_draw_box_color(u8 color, u8 x_b, u8 y_px, u8 w_b, u8 h_px) {

    u8  pattern = cpct_px2byteM1(color, color, color, color);
    teopl_video_draw_box_pattern(pattern, x_b, y_px, w_b, h_px);
}

void teopl_video_draw_box_frame(TeoplVideoFrameData* frame_data, u8 x_b, u8 y_px, u8 w_b, u8 h_px, u8 remove) {

    //u8 is_rich = frame_data->is_rich;

    u8 ch1 = remove ? 0 : frame_data->ch1;
    //u8 ch2 = remove ? 0 : frame_data->ch2;
    //u8 ch3 = remove ? 0 : frame_data->ch3;
    //u8 ch4 = remove ? 0 : frame_data->ch4;

    u8 cv1 = remove ? 0 : frame_data->cv1;
    //u8 cv2 = remove ? 0 : frame_data->cv2;
    //u8 cv3 = remove ? 0 : frame_data->cv3;
    //u8 cv4 = remove ? 0 : frame_data->cv4;

    //u8* corner_sprite = g_blocks + TEOPL_LEVEL_TILE_PLANT_00 * blocks_BYTES_PER_SPRITE;
    //u8 cv_left = cpct_px2byteM1(0, 0, cv1, cv2);
    //u8 cv_right = cpct_px2byteM1(cv3, cv4, 0, 0);

    // ---

    // horizontal up
    /*
    if (is_rich) {
        teopl_video_draw_box_color(ch1, x_b, y_px - 2, w_b, 1);
        teopl_video_draw_box_color(ch2, x_b, y_px - 1, w_b, 1);
        teopl_video_draw_box_color(ch3, x_b, y_px, w_b, 1);
        teopl_video_draw_box_color(ch4, x_b, y_px + 1, w_b, 1);
    }
    else {*/
        teopl_video_draw_box_color(ch1, x_b, y_px - 1, w_b, 1);
    //}

    // horizontal down
    /*if (is_rich) {
        teopl_video_draw_box_color(ch1, x_b, y_px + h_px - 2, w_b, 1);
        teopl_video_draw_box_color(ch2, x_b, y_px + h_px - 1, w_b, 1);
        teopl_video_draw_box_color(ch3, x_b, y_px + h_px, w_b, 1);
        teopl_video_draw_box_color(ch4, x_b, y_px + h_px + 1, w_b, 1);
    }
    else {*/
        teopl_video_draw_box_color(ch1, x_b, y_px + h_px, w_b, 1);
    //}

    // vertical left
    /*if (is_rich) {
        teopl_video_draw_box_pattern(cv_left, x_b - 1, y_px + 2, 1, h_px - 4);
        teopl_video_draw_box_pattern(cv_right, x_b, y_px + 2, 1, h_px - 4);
    }
    else {*/
        teopl_video_draw_box_pattern(cpct_px2byteM1(0, 0, 0, cv1), x_b - 1, y_px, 1, h_px);
    //}

    // vertical right
    /*if (is_rich) {
        teopl_video_draw_box_pattern(cv_left, x_b + w_b - 1, y_px + 2, 1, h_px - 4);
        teopl_video_draw_box_pattern(cv_right, x_b + w_b, y_px + 2, 1, h_px - 4);
    }
    else {*/
        teopl_video_draw_box_pattern(cpct_px2byteM1(cv1, 0, 0, 0), x_b + w_b, y_px, 1, h_px);
    //}
}

void teopl_video_draw_sprite_compressed_end(u8* sprite_zip_end, u16 sprite_zip_size, u16 sprite_unzip_size, u8 x_b, u8 y_px, u8 w_b, u8 h_px) {

    teopl_interrupt_and_sound(0);

#ifdef WINCPCTELERA
    teopl_util_decompress_zx7(sprite_zip_end, sprite_zip_size, g_teopl_pool_anything + sprite_unzip_size - 1, sprite_unzip_size);
#else
    teopl_util_decompress_zx7(sprite_zip_end, g_teopl_pool_anything + sprite_unzip_size - 1);
#endif

    cpct_drawSprite((void*)g_teopl_pool_anything, TEOPL_VIDEO_GET_LOCATION(x_b, y_px), w_b, h_px);

    teopl_interrupt_and_sound(1);
}

void teopl_video_draw_level_tile(u8* tile) {

    u8 col = teopl_level_tile_x_get(tile);
    u8 row = teopl_level_tile_y_get(tile);
    TeoplLevelTileType tile_type = (*tile);

    u8 x_b = col * TEOPL_DEFINES_LEVEL_TILE_W_B + g_teopl_level_data.level_x_b;
    u8 y_px = row * TEOPL_DEFINES_LEVEL_TILE_H_PX + g_teopl_level_data.level_y_px;

    u8* sprite = (void*)(g_blocks + tile_type * blocks_BYTES_PER_SPRITE);
    void* location = TEOPL_VIDEO_GET_LOCATION(x_b, y_px);

    u8 draw_with_masking =
        tile_type == TEOPL_LEVEL_TILE_GLASS ||
        tile_type == TEOPL_LEVEL_TILE_GOAL ||
        tile_type == TEOPL_LEVEL_TILE_CHIP_INACTIVE ||
        tile_type == TEOPL_LEVEL_TILE_PILL_BLUE ||
        tile_type == TEOPL_LEVEL_TILE_PILL_RED;

    teopl_video_draw_background(x_b, y_px);

    if (tile_type != TEOPL_LEVEL_TILE_EMPTY) {

        if (draw_with_masking) {

            cpct_drawSpriteMaskedAlignedTable(
                sprite,
                location,
                TEOPL_DEFINES_LEVEL_TILE_W_B, TEOPL_DEFINES_LEVEL_TILE_H_PX,
                g_teopl_video_masktable
            );
        }
        else {
            cpct_drawSprite(
                sprite,
                location,
                TEOPL_DEFINES_LEVEL_TILE_W_B, TEOPL_DEFINES_LEVEL_TILE_H_PX
            );
        }
    }
}

void teopl_video_draw_level(u8 x, u8 y, u8 width, u8 height) {

    u8*     text_final = g_teopl_font_async_text;

    u8*     tile = 0;
    u8      right = x + width;
    u8      bottom = y + height;

    u8      level_x_b = g_teopl_level_data.level_x_b;
    u8      level_y_px = g_teopl_level_data.level_y_px;

    u8      level_h_px = g_teopl_level_data.level_height * TEOPL_DEFINES_LEVEL_TILE_H_PX;

    u8      level_num = g_teopl_level_data.level_index;
    char    str[4]; // "123" + '\0' // REMARK: must be enough for level and chips/qubits numbers
    u8      str_len;

    // ---

    // studio logo
    teopl_util_draw_studio_logo(0, 0);

    // draw frames
    // ---

    teopl_video_draw_box_frame(
        &g_teopl_video_frame_data_level,
        level_x_b, level_y_px,
        width * TEOPL_DEFINES_LEVEL_TILE_W_B, height * TEOPL_DEFINES_LEVEL_TILE_H_PX,
        0
    );

    // draw tiles
    // ---

    for (u8 row = y; row < bottom; ++row) {
        for (u8 col = x; col < right; ++col) {

            tile = teopl_level_tile_get(col, row);

            teopl_video_draw_level_tile(tile);
        }
    }

    // draw logo
    // ---

    teopl_video_draw_sprite_compressed_end(
        menu_text_end, menu_text_size_z, menu_text_size,
        TEOPL_DEFINES_LEVEL_LOGO_X_B, TEOPL_DEFINES_LEVEL_LOGO_Y_PX,
        MENU_TEXT_W, MENU_TEXT_H
    );

    // draw text (level number, password, chips)
    // ---

    // internet level
    if (level_num == 255) {
        cpct_memcpy(str, "NET", 3);
        str_len = 3;
    }
    else {
        teopl_util_u32_to_string(str, &str_len, level_num + 1, 0);
    }

    cpct_memset(text_final, 0, TEOPL_FONT_MAX_NUM_BYTES_PER_TEXT);

    cpct_memcpy(text_final, "LEVEL ", 6);
    text_final += 6;

    cpct_memcpy(text_final, str, str_len);
    text_final += str_len;

    *text_final = '_';
    text_final++;

    // skip writing pass for first level (only pass with length of 1)
    if (g_teopl_level_data.level_password[0] != 0 && g_teopl_level_data.level_password[1] != '\0') {

        cpct_memcpy(text_final, "PASSWD:", 7);
        text_final += 7;

        cpct_memcpy(text_final, g_teopl_level_data.level_password, TEOPL_LEVEL_PASSWORD_LENGTH);
        text_final += TEOPL_LEVEL_PASSWORD_LENGTH;

        *text_final = '_';
        text_final++;
    }
    else {
        *text_final = '_';
        text_final++;
    }

    // chips collected (REMARK: show only when no pill is taken)
    if (!g_teopl_main_pill) {
        cpct_memcpy(text_final, (void*)(g_teopl_main_text + 70), 7);
        text_final += 7;

        teopl_util_u32_to_string(str, &str_len, g_main_num_chips_total, 0);

        cpct_memcpy(text_final, str, str_len);
        //text_final += str_len;
    }

    teopl_font_async_print_prepare(
        TEOPL_DEFINES_LEVEL_PASS_X_B, TEOPL_DEFINES_LEVEL_PASS_Y_PX,
        88,
        400,
        TEOPL_BILLBOARD_TICKS_LETTER,
        0, 0
    );

    // drawing done
    // ---

    teopl_events_time_start_set(g_teopl_interrupt_ticks_total);
}

/*
static void _teopl_video_draw_entities() {

    u8 i, num;
    TEOPL_MEMORY_POINTER_TYPE output;

    // entities
    // ---

    num = g_teopl_level_data.entites.num_elements;

    for (i = 0; i < num; ++i) {
        if (teopl_ringbuffer_peek(&(g_teopl_level_data.entites), i, &output)) {
            teopl_entity_draw((TeoplEntity*)output);
        }
    }
}
*/

void teopl_video_draw_letter() {

    if (g_teopl_font_async_character_draw == TEOPL_FONT_CHARACTER_DRAW_CHARACTER_YES) {

        u8 is_whitespace = teopl_util_character_is_whitespace(g_teopl_font_async_character);
        u8 is_whitespace_prev = teopl_util_character_is_whitespace(*(g_teopl_font_async_text + g_teopl_font_async_text_offset - 1));

        g_teopl_font_async_character_draw = TEOPL_FONT_CHARACTER_DRAW_CHARACTER_NO;

        // ---

        // hypernation
        if (g_teopl_font_async_x_b_offset > g_teopl_font_async_w_b) {

            if (!is_whitespace) {
                if (!is_whitespace_prev) {
                    teopl_font_draw_character('-', g_teopl_font_async_x_b + g_teopl_font_async_x_b_offset, g_teopl_font_async_y_px + g_teopl_font_async_y_px_offset);
                }

                teopl_font_async_letter_previous();
            }

            g_teopl_font_async_y_px_offset += font_HEIGHT_PX;
            g_teopl_font_async_x_b_offset = 0;
        }
        else {

            if (!is_whitespace) {

                // draw image
                if (teopl_billboard_is_talking()) {
                    teopl_billboard_draw_picture_next();
                }

                // draw letter
                teopl_font_draw_character(g_teopl_font_async_character, g_teopl_font_async_x_b + g_teopl_font_async_x_b_offset, g_teopl_font_async_y_px + g_teopl_font_async_y_px_offset);

                g_teopl_font_async_x_b_offset += font_WIDTH_B;
            }
            else if (g_teopl_font_async_character == '\n' || g_teopl_font_async_character == '_') {
                g_teopl_font_async_y_px_offset += font_HEIGHT_PX;
                g_teopl_font_async_x_b_offset = 0;
            }
            else {
                g_teopl_font_async_x_b_offset += font_WIDTH_B;
            }
        }

        // progress text
        teopl_font_async_letter_next();

        if (!g_teopl_font_async_character) {
            teopl_font_async_init_parameters();
            if (g_teopl_font_async_set_flag_on_start) {
                *g_teopl_font_async_set_flag_on_start = 0;
            }
            if (g_teopl_font_async_set_flag_on_finish) {
                *g_teopl_font_async_set_flag_on_finish = 1;
            }
        }
    }
}

void teopl_video_game_freeze_animating_frame(u8 frame_type) {

    u8 i;

    TeoplVideoFrameData _frame_data;

    u8  x_b;
    u8  y_px;

    u8  w_b;
    u8  h_px;

    // ---

    if (frame_type == 0) {

        x_b = TEOPL_DEFINES_MAIN_PASSWORD_X_B - 1;
        y_px = TEOPL_DEFINES_MAIN_PASSWORD_Y_PX - TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;

        w_b = font_WIDTH_B * TEOPL_LEVEL_PASSWORD_LENGTH + 2;
        h_px = font_HEIGHT_PX + 2 * TEOPL_DEFINES_VIDEO_PARAM_PX_PER_B;

        cpct_memcpy(&_frame_data, &g_teopl_video_frame_data_password, sizeof(TeoplVideoFrameData));
    }
    else {

        x_b = g_teopl_level_data.level_x_b;
        y_px = g_teopl_level_data.level_y_px;

        w_b = g_teopl_level_data.level_width * TEOPL_DEFINES_LEVEL_TILE_W_B;
        h_px = g_teopl_level_data.level_height * TEOPL_DEFINES_LEVEL_TILE_H_PX;

        cpct_memcpy(&_frame_data, &g_teopl_video_frame_data_level, sizeof(TeoplVideoFrameData));
    }

    // ---

    teopl_billboard_say_finish(0);

    for (i = 0; i < 192; ++i) {

        _frame_data.ch1 = cpct_rand8() % (TEOPL_DEFINES_VIDEO_PARAM_COLORS_NUM - 1) + 1;
        _frame_data.cv1 = cpct_rand8() % (TEOPL_DEFINES_VIDEO_PARAM_COLORS_NUM - 1) + 1;

        cpct_waitVSYNC();

        // REMARK: just to update total ticks
        teopl_interrupt_tick_dt_get();

        teopl_video_draw_box_frame(
            &_frame_data,
            x_b, y_px,
            w_b, h_px,
            0
        );
    }
}
