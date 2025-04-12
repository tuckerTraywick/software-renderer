#include <assert.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "sprite.h"
#include "color.h"

BmpHeader BmpHeaderReadFromFile(FILE *file) {
	// Skip the signature.
	uint16_t signature = 0;
	if (fread(&signature, 1, 2, file) != 2) {
		return (BmpHeader){0};
	}

	BmpHeader header = {0};
	size_t valuesRead = fread(&header, sizeof header, 1, file);
	if (valuesRead == 1) {
		return header;
	}
	return (BmpHeader){0};
}

bool BmpHeaderIsValid(BmpHeader *header) {
	return header->width != 0;
}

Atlas AtlasReadFromFile(FILE *file) {
	BmpHeader header = BmpHeaderReadFromFile(file);
	if (!BmpHeaderIsValid(&header)) {
		return (Atlas){0};
	}
	fseek(file, header.dataOffset, SEEK_SET);

	Atlas atlas = {
		.width = header.width,
		.height = header.height,
		.bitmap = malloc(header.width*header.height*sizeof *atlas.bitmap),
	};
	if (!atlas.bitmap) {
		return (Atlas){0};
	}

	// Read the rows from the file starting at the last row.
	uint32_t row = 0;
	for (row = atlas.height - 1; row >= 0; --row) {
		if (fread(atlas.bitmap + row*atlas.width, sizeof *atlas.bitmap, atlas.width, file) != (size_t)atlas.width) {
			break;
		}
	}
	if (row != 0) {
		free(atlas.bitmap);
		return (Atlas){0};
	}
	return atlas;
}

Atlas AtlasReadFromPath(const char *path) {
	FILE *file = fopen(path, "rb");
	if (!file) {
		return (Atlas){0};
	}
	return AtlasReadFromFile(file);
}

void AtlasDestroy(Atlas *atlas) {
	free(atlas->bitmap);
	*atlas = (Atlas){0};
}

bool AtlasIsValid(Atlas *atlas) {
	return atlas->bitmap != NULL;
}

Sprite AtlasGetSprite(Atlas *atlas, uint16_t spriteHeight, uint16_t spriteIndex) {
	assert(spriteIndex < atlas->height/spriteHeight && "Sprite index out of bounds.");
	return (Sprite){
		.width = atlas->width,
		.height = spriteHeight,
		.bitmap = atlas->bitmap + spriteIndex*spriteHeight*atlas->width,
	};
}

Atlas AtlasGetSubatlas(Atlas *atlas, uint16_t subatlasHeight, uint16_t subatlasIndex) {
	assert(subatlasIndex < atlas->height/subatlasHeight && "Subatlas index out of bounds.");
	return (Atlas){
		.width = atlas->width,
		.height = subatlasHeight,
		.bitmap = atlas->bitmap + subatlasIndex*subatlasHeight*atlas->width,
	};
}

Sprite AtlasGetFontCharacter(Atlas *font, char ch) {
	return AtlasGetSprite(font, 8, (uint32_t)ch - 65);
}

void SpriteCopy(Sprite *source, Sprite *destination) {
	assert(source->width <= destination->width && source->height <= destination->height && "Cannot copy to a smaller sprite.");
	for (uint32_t i = 0; i < source->width*source->height; ++i) {
		destination->bitmap[i] = source->bitmap[i];
	}
	destination->width = source->width;
	destination->height = source->height;
}

void SpriteApplyColor(Sprite *sprite, uint32_t color) {
	for (uint32_t i = 0; i < sprite->width*sprite->height; ++i) {
		if (A(sprite->bitmap[i])) {
			sprite->bitmap[i] = color;
		}
	}
}
