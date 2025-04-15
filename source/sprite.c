#include <assert.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "sprite.h"
#include "color.h"

Bmp_Header Bmp_Header_read_from_file(FILE *file) {
	// Skip the signature.
	uint16_t signature = 0;
	if (fread(&signature, 1, 2, file) != 2) {
		return (Bmp_Header){0};
	}

	Bmp_Header header = {0};
	size_t values_read = fread(&header, sizeof header, 1, file);
	if (values_read == 1) {
		return header;
	}
	return (Bmp_Header){0};
}

bool Bmp_Header_is_valid(Bmp_Header *header) {
	return header->width != 0;
}

void Sprite_copy(Sprite *source, Sprite *destination) {
	assert(source->width <= destination->width && source->height <= destination->height && "Cannot copy to a smaller sprite.");
	for (uint32_t i = 0; i < source->width*source->height; ++i) {
		destination->bitmap[i] = source->bitmap[i];
	}
	destination->width = source->width;
	destination->height = source->height;
}

void Sprite_apply_color(Sprite *sprite, uint32_t color) {
	for (uint32_t i = 0; i < sprite->width*sprite->height; ++i) {
		if (A(sprite->bitmap[i])) {
			sprite->bitmap[i] = color;
		}
	}
}

Sprite Sprite_read_from_file(FILE *file) {
	Bmp_Header header = Bmp_Header_read_from_file(file);
	if (!Bmp_Header_is_valid(&header)) {
		return (Sprite){0};
	}
	fseek(file, header.data_offset, SEEK_SET);
	
	Sprite sprite = {
		.width = header.width,
		.height = header.height,
		.bitmap = malloc(header.width*header.height*sizeof *sprite.bitmap),
	};
	if (!sprite.bitmap) {
		return (Sprite){0};
	}
	
	// Read the rows from the file starting at the last row.
	int32_t row = 0;
	for (row = sprite.height - 1; row >= 0; --row) {
		if (fread(sprite.bitmap + row*sprite.width, sizeof *sprite.bitmap, sprite.width, file) != (size_t)sprite.width) {
			free(sprite.bitmap);
			return (Sprite){0};
		}
	}
	return sprite;
}

Sprite Sprite_read_from_path(const char *path) {
	FILE *file = fopen(path, "rb");
	if (!file) {
		return (Sprite){0};
	}
	return Sprite_read_from_file(file);
}

bool Sprite_is_valid(Sprite *sprite) {
	return sprite->bitmap != NULL;
}

void Sprite_destroy(Sprite *sprite) {
	free(sprite->bitmap);
	*sprite = (Sprite){0};
}

Sprite Atlas_get_sprite(Atlas *atlas, uint16_t sprite_height, uint16_t sprite_index) {
	assert(sprite_index < atlas->height/sprite_height && "Sprite index out of bounds.");
	return (Sprite){
		.width = atlas->width,
		.height = sprite_height,
		.bitmap = atlas->bitmap + sprite_index*sprite_height*atlas->width,
	};
}

Atlas Atlas_get_subatlas(Atlas *atlas, uint16_t subatlas_height, uint16_t subatlas_index) {
	assert(subatlas_index < atlas->height/subatlas_height && "Subatlas index out of bounds.");
	return (Atlas){
		.width = atlas->width,
		.height = subatlas_height,
		.bitmap = atlas->bitmap + subatlas_index*subatlas_height*atlas->width,
	};
}
