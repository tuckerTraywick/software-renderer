#ifndef FONT_H
#define FONT_H

#include "sprite.h"

#define FONT_SPRITE_WIDTH 8

#define FONT_SPRITE_HEIGHT 8

#define FONT_SPRITE_COUNT 94

typedef Atlas Font;

// `*4` because each pixel is 4 bytes.
extern char default_font_bitmap[FONT_SPRITE_COUNT*FONT_SPRITE_WIDTH*FONT_SPRITE_HEIGHT*4];

extern Font default_font;

inline Font Font_read_from_file(FILE *file) {
	Atlas atlas = Atlas_read_from_file(file);
	return *(Font*)&atlas;
}

inline Font Font_read_from_path(const char *path) {
	Atlas atlas = Atlas_read_from_path(path);
	return *(Font*)&atlas;
}

inline void Font_destroy(Font *font) {
	Atlas_destroy((Atlas*)font);
}

inline bool Font_is_valid(Font *font) {
	return Atlas_is_valid((Atlas*)font);
}

// Returns the sprite for the given character from the given font atlas. Each character must be 8x8.
Sprite Font_get_character_sprite(Font *font, char ch);

#endif // FONT_H
