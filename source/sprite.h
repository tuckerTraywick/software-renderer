#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

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
	uint32_t *bitmap; // Not owned by sprites. Is owned by atlases and fonts.
	uint16_t width;
	uint16_t height;
} Sprite;

typedef Sprite Atlas;

Bmp_Header Bmp_Header_read_from_file(FILE *file);

bool Bmp_Header_is_valid(Bmp_Header *header);

Sprite Sprite_read_from_file(FILE *file);

Sprite Sprite_read_from_path(const char *path);

bool Sprite_is_valid(Sprite *sprite);

void Sprite_destroy(Sprite *sprite);

// Copies the dimensions and the bitmap's content from `source` to `destination`.
void Sprite_copy(Sprite *source, Sprite *destination);

// Makes every opaque pixel in `sprite` have value `color`. Used to color font characters.
void Sprite_apply_color(Sprite *sprite, uint32_t color);

inline Atlas Atlas_read_from_file(FILE *file) {
	Sprite sprite = Sprite_read_from_file(file);
	return *(Atlas*)&sprite;
}

inline Atlas Atlas_read_from_path(const char *path) {
	Sprite sprite = Sprite_read_from_path(path);
	return *(Atlas*)&sprite;
}

inline bool Atlas_is_valid(Atlas *atlas) {
	return Sprite_is_valid((Sprite*)atlas);
}

inline void Atlas_destroy(Atlas *atlas) {
	Sprite_destroy((Sprite*)atlas);
}

Sprite Atlas_get_sprite(Atlas *atlas, uint16_t sprite_height, uint16_t sprite_index);

Atlas Atlas_get_subatlas(Atlas *atlas, uint16_t subatlas_height, uint16_t subatlas_index);

#endif // SPRITE_H
