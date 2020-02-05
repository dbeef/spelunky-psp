//
// Copyright 2019 Sepehr Taghdisian (septag@github). All rights reserved.
// License: https://github.com/septag/atlasc#license-bsd-2-clause
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sx/math.h"

typedef struct atlasc_args {
    int         alpha_threshold;
    float       dist_threshold;
    int         max_width;
    int         max_height;
    int         border;
    int         pot;
    int         padding;
    int         mesh;
    int         max_verts_per_mesh;
    float       scale;
} atlasc_args;

typedef struct atlasc_image_data {
    uint8_t* pixels;    // only supports 32bpp RGBA format
    int      width;
    int      height;
} atlasc_image_data;

typedef struct atlasc_args_frommem {
    atlasc_args              common;
    const atlasc_image_data* images;
    int                      num_images;
} atlasc_args_frommem;

typedef struct atlasc_args_files {
    atlasc_args common;
    char**      in_filepaths;
    int         num_files;
    const char* out_filepath;    // not required for `atlasc_make_in_memory`
} atlasc_args_files;

typedef struct atlasc_sprite {
    uint8_t* src_image;      // RGBA image buffer (32bpp)
    sx_ivec2 src_size;       // widthxheight
    sx_irect sprite_rect;    // cropped rectangle relative to sprite's source image (pixels)
    sx_irect sheet_rect;     // rectangle in final sheet (pixels)

    // sprite-mesh data (if flag is set. see atlas_args)
    uint16_t  num_tris;
    int       num_points;
    sx_ivec2* pts;
    sx_ivec2* uvs;
    uint16_t* tris;
} atlasc_sprite;

typedef struct atlasc_atlas_data {
    atlasc_sprite* sprites;
    int            num_sprites;
    atlasc_image_data atlas_image;
} atlasc_atlas_data;

#ifndef ATLASC__HIDE_API
#ifdef __cplusplus
extern "C" {
#endif

// set allocation callbacks
// uses malloc/free/realloc by default
void atlasc_set_alloc_callbacks(void* (*malloc_fn)(size_t size, void* ctx),
                                void (*free_fn)(void* ptr, void* ctx),
                                void* (*realloc_fn)(void* ptr, size_t size, void* ctx), void* ctx);

// receives arguments (input filepaths) and writes 32bpp PNG to out_filepath
bool atlasc_make(const atlasc_args_files* args);

// receives arguemnts (out_filepath is not required) and returns atlas_data
// you have to free the data after use with `atlasc_free`
atlasc_atlas_data* atlasc_make_inmem(const atlasc_args_files* args);

// recevies input image buffers and common arguments
// you have to free the data after use with `atlasc_free`
atlasc_atlas_data* atlasc_make_inmem_frommem(const atlasc_args_frommem* args);

// frees atlasc_atlas_data memory
void atlasc_free(atlasc_atlas_data* atlas);

// returns the last error string
const char* atlasc_error_string();

#ifdef __cplusplus
}
#endif

#endif // ATLASC__HIDE_API