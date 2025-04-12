#include <assert.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "sprite.h"
#include "color.h"

bool readBmpHeader(FILE *file, BmpHeader *header) {
	// Skip the signature.
	uint16_t signature = 0;
	if (fread(&signature, 1, 2, file) != 2) {
		return false;
	}
	size_t valuesRead = fread(header, sizeof *header, 1, file);
	return valuesRead == 1;
}

bool SpriteReadFromBmp(Sprite *sprite, FILE *file) {
	BmpHeader header = {0};
	if (!readBmpHeader(file, &header)) {
		return false;
	}
	fseek(file, header.dataOffset, SEEK_SET);

	size_t valuesRead = 0;
	// Read the rows from the file starting at the last row.
	for (uint32_t row = header.height - 1; row >= 0; --row) {
		size_t valuesInRow = fread(sprite->bitmap + row*header.width, sizeof *sprite->bitmap, header.width, file);
		if (!valuesInRow) {
			break;
		}
		valuesRead += valuesInRow;
	}

	if (valuesRead != sprite->width*sprite->height) {
		return false;
	}
	sprite->width = header.width;
	sprite->height = header.height;
	return valuesRead == header.width*header.height;
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

Sprite SpriteGetVerticalSlice(Sprite *sprite, uint32_t height, uint32_t index) {
	return (Sprite){
		.width = sprite->width,
		.height = height,
		.bitmap = sprite->bitmap + index*height*sprite->width,
	};
}

Sprite SpriteGetFontCharacter(Sprite *font, char ch) {
	return SpriteGetVerticalSlice(font, 8, (uint32_t)ch - 65);
}
