#ifndef FONT_H
#define FONT_H

#include "sprite.h"

#define FONT_SPRITE_WIDTH 8

#define FONT_SPRITE_HEIGHT 8

#define FONT_SPRITE_COUNT 94

typedef Atlas Font;

// `*4` because each pixel is 4 bytes.
extern char defaultFontBitmap[FONT_SPRITE_COUNT*FONT_SPRITE_WIDTH*FONT_SPRITE_HEIGHT*4];

extern Font defaultFont;

inline Font FontReadFromFile(FILE *file) {
	Atlas atlas = AtlasReadFromFile(file);
	return *(Font*)&atlas;
}

inline Font FontReadFromPath(const char *path) {
	Atlas atlas = AtlasReadFromPath(path);
	return *(Font*)&atlas;
}

inline void FontDestroy(Font *font) {
	AtlasDestroy((Atlas*)font);
}

inline bool FontIsValid(Font *font) {
	return AtlasIsValid((Atlas*)font);
}

// Returns the sprite for the given character from the given font atlas. Each character must be 8x8.
Sprite FontGetCharacterSprite(Font *font, char ch);

#endif // FONT_H
