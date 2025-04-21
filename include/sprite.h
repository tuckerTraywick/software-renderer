#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "vector.h"
#include "color.h"

#define FONT_SPRITE_WIDTH 7

#define FONT_SPRITE_HEIGHT 7

#define FONT_PADDING 2

#define FONT_SPRITE_COUNT 94

#define ASCII_TEST_STRING " !\"$%&'()*+,-./0123456789:;<=>?@`\nABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`\nabcdefghijklmnopqrstuvwxyz{|}~"

// Represents the header of a .bmp file. Omits the signature so the fields are aligned correctly.
typedef struct Bmp_Header {
	uint32_t file_size;
	uint32_t reserved; // Unused.
	uint32_t data_offset;
	uint32_t info_header_size; // Always 40.
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t image_size;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t colors_used;
	uint32_t important_colors;
} Bmp_Header;

// Represents a bitmap and its dimensions.
typedef struct Sprite {
	Color *bitmap;
	Vector2 size;
} Sprite;

typedef struct Atlas {
	Color *bitmap;
	Vector2 sprite_size;
	uint16_t sprite_count;
} Atlas;

typedef struct Font {
	Atlas atlas;
	Vector2 padding;
} Font;

extern Color default_font_bitmap[FONT_SPRITE_COUNT*FONT_SPRITE_WIDTH*FONT_SPRITE_HEIGHT];

extern Font default_font;

Bmp_Header Bmp_Header_read_from_file(FILE *file);

bool Bmp_Header_is_valid(Bmp_Header *header);

Sprite Sprite_read_from_file(FILE *file);

Sprite Sprite_read_from_path(const char *path);

// Only call this on sprites you read from files.
void Sprite_destroy(Sprite *sprite);

bool Sprite_is_valid(Sprite *sprite);

Color Sprite_get_pixel(Sprite *sprite, Vector2 position);

// Copies the dimensions and the bitmap's content from `source` to `destination`.
// void Sprite_copy(Sprite *source, Sprite *destination);

Atlas Atlas_read_from_file(FILE *file);

Atlas Atlas_read_from_path(const char *path);

void Atlas_destroy(Atlas *atlas);

bool Atlas_is_valid(Atlas *atlas);

Sprite Atlas_get_sprite(Atlas *atlas, uint16_t sprite_index);

Font Font_read_from_file(FILE *file);

Font Font_read_from_path(const char *path);

void Font_destroy(Font *font);

bool Font_is_valid(Font *font);

// Returns the sprite for the given character from the given font atlas. Each character must be 8x8.
Sprite Font_get_character_sprite(Font *font, char ch);

#endif // SPRITE_H
