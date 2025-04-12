#include <assert.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "sprite.h"

bool readBmpHeader(FILE *file, BmpHeader *header) {
	// Skip the signature.
	uint16_t signature = 0;
	if (fread(&signature, 1, 2, file) != 2) {
		return false;
	}
	size_t valuesRead = fread(header, sizeof *header, 1, file);
	return valuesRead == 1;
}

bool SpriteReadFromBmp(FILE *file, Sprite *sprite) {
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

Sprite SpriteGetVerticalSlice(Sprite *sprite, uint32_t height, uint32_t index) {
	return (Sprite){
		.width = sprite->width,
		.height = height,
		.bitmap = sprite->bitmap + index*height*sprite->width,
	};
}
