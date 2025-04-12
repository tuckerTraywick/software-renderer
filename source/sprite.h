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

// Returns true if the header was successfully read from the file.
bool readBmpHeader(FILE *file, BmpHeader *header);

// Treats the sprite as a row of tiles `height` pixels tall and returns the `index`th tile. Used for
// getting individual sprites from an atlas.
Sprite SpriteGetVerticalSlice(Sprite *sprite, uint32_t height, uint32_t index);

// Returns the sprite for the given character from the given font atlas. Each character must be 8x8.
Sprite SpriteGetFontCharacter(Sprite *font, char ch);

// Assumes `sprite` has already been initialized and has a bitmap large enough to hold the sprite in
// stored in `file`. Returns true if the sprite was read successfully.
bool SpriteReadFromBmp(Sprite *sprite, FILE *file);

// Copies the dimensions and the bitmap's content from `source` to `destination`.
void SpriteCopy(Sprite *source, Sprite *destination);

// Makes every opaque pixel in `sprite` have value `color`. Used to color font characters.
void SpriteApplyColor(Sprite *sprite, uint32_t color);

#endif // SPRITE_H
