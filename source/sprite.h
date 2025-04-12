#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Represents a bitmap and its dimensions.
typedef struct Sprite {
	uint32_t *bitmap; // Not owned by this struct. The user of this struct is responsible for managing the memory for bitmaps.
	uint16_t width;
	uint16_t height;
} Sprite;

typedef Sprite Atlas;

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

Atlas AtlasReadFromFile(FILE *file);

Atlas AtlasReadFromPath(const char *path);

void AtlasDestroy(Atlas *atlas);

bool AtlasIsValid(Atlas *atlas);

Sprite AtlasGetSprite(Atlas *atlas, uint16_t spriteHeight, uint16_t spriteIndex);

Atlas AtlasGetSubatlas(Atlas *atlas, uint16_t subatlasHeight, uint16_t subatlasIndex);

// Copies the dimensions and the bitmap's content from `source` to `destination`.
void SpriteCopy(Sprite *source, Sprite *destination);

// Makes every opaque pixel in `sprite` have value `color`. Used to color font characters.
void SpriteApplyColor(Sprite *sprite, uint32_t color);

BmpHeader BmpHeaderReadFromFile(FILE *file);

bool BmpHeaderIsValid(BmpHeader *header);

#endif // SPRITE_H
