#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Represents a bitmap and its dimensions.
typedef struct Sprite {
	uint32_t width;
	uint32_t height;
	uint32_t *bitmap; // Not owned by this struct. The user of this struct is responsible for managing the memory for bitmaps.
} Sprite;

// Represents multiple bitmaps that share one contiguous block of memory.
typedef struct Atlas {
	uint32_t spriteWidth;
	uint32_t spriteHeight;
	size_t spriteCount;
	uint32_t *bitmap; // Sprites are arranged in a vertical row.
} Atlas;

// Represents the header of a .bmp file. Omits the signature so the fields are aligned correctly.
typedef struct BmpHeader {
	uint32_t fileSize;
	uint32_t reserved; // Unused.
	uint32_t dataOffset;
	uint32_t infoHeaderSize; // Always 40.
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t imageSize;
	uint32_t horizontalResolution;
	uint32_t verticalResolution;
	uint32_t colorsUsed;
	uint32_t importantColors;
} BmpHeader;

// Assumes `sprite` has already been initialized and has a bitmap large enough to hold the sprite in
// stored in `file`. Returns true if the sprite was read successfully.
bool SpriteReadFromBmp(FILE *file, Sprite *sprite);

// Returns true if the header was successfully read from the file.
bool readBmpHeader(FILE *file, BmpHeader *header);

Atlas AtlasFromSprite(Sprite *sprite, uint32_t spriteWidth, uint32_t spriteHeight);

Sprite SpriteFromAtlas(Atlas *atlas, size_t spriteIndex);

#endif // SPRITE_H
