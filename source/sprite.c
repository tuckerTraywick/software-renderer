#include <assert.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "sprite.h"
#include "vector.h"
#include "color.h"

// TODO: This needs to be imported from gimp as an array or read from a file.
Color default_font_bitmap[FONT_SPRITE_COUNT*FONT_SPRITE_WIDTH*FONT_SPRITE_HEIGHT];

Font default_font = {
	.atlas = {
		.bitmap = default_font_bitmap,
		.sprite_size = (Vector2){FONT_SPRITE_WIDTH, FONT_SPRITE_HEIGHT},
		.sprite_count = FONT_SPRITE_COUNT,
	},
	.padding = (Vector2){FONT_PADDING, FONT_PADDING},
};

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

Sprite Sprite_read_from_file(FILE *file) {
	Bmp_Header header = Bmp_Header_read_from_file(file);
	if (!Bmp_Header_is_valid(&header)) {
		return (Sprite){0};
	}
	fseek(file, header.data_offset, SEEK_SET);
	
	Sprite sprite = {
		.size = (Vector2){header.width, header.height},
		.bitmap = malloc(header.width*header.height*sizeof *sprite.bitmap),
	};
	if (!sprite.bitmap) {
		return (Sprite){0};
	}
	
	// Read the rows from the file starting at the last row.
	int32_t row = 0;
	for (row = sprite.size.y - 1; row >= 0; --row) {
		if (fread(sprite.bitmap + row*sprite.size.y, sizeof *sprite.bitmap, sprite.size.x, file) != (size_t)sprite.size.x) {
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

void Sprite_destroy(Sprite *sprite) {
	free(sprite->bitmap);
	*sprite = (Sprite){0};
}

bool Sprite_is_valid(Sprite *sprite) {
	return sprite->bitmap != NULL;
}

void Atlas_destroy(Atlas *atlas) {
	free(atlas->bitmap);
	*atlas = (Atlas){0};
}

bool Atlas_is_valid(Atlas *atlas) {
	return atlas->bitmap != NULL;
}

Sprite Atlas_get_sprite(Atlas *atlas, uint16_t sprite_index) {
	assert(sprite_index < atlas->sprite_count && "Sprite index out of bounds.");
	return (Sprite){
		.size = atlas->sprite_size,
		.bitmap = atlas->bitmap + sprite_index*atlas->sprite_size.y*atlas->sprite_size.x,
	};
}

Font Font_read_from_file(FILE *file) {
	Sprite sprite = Sprite_read_from_file(file);
	if (!Sprite_is_valid(&sprite)) {
		return (Font){0};
	}
	return (Font){
		.atlas = {
			.bitmap = sprite.bitmap,
			.sprite_count = FONT_SPRITE_COUNT,
			.sprite_size = (Vector2){FONT_SPRITE_WIDTH, FONT_SPRITE_HEIGHT},
		},
		.padding = (Vector2){FONT_PADDING, FONT_PADDING},
	};
}

Font Font_read_from_path(const char *path) {
	Sprite sprite = Sprite_read_from_path(path);
	if (!Sprite_is_valid(&sprite)) {
		return (Font){0};
	}
	return (Font){
		.atlas = {
			.bitmap = sprite.bitmap,
			.sprite_count = FONT_SPRITE_COUNT,
			.sprite_size = (Vector2){FONT_SPRITE_WIDTH, FONT_SPRITE_HEIGHT},
		},
		.padding = (Vector2){FONT_PADDING, FONT_PADDING},
	};
}

void Font_destroy(Font *font) {
	Atlas_destroy(&font->atlas);

}

bool Font_is_valid(Font *font) {
	return Atlas_is_valid(&font->atlas);
}

Sprite Font_get_character_sprite(Font *font, char ch) {
	return Atlas_get_sprite((Atlas*)font, (uint16_t)ch - 32);
}
