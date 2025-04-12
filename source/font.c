#include "font.h"
#include "sprite.h"

char defaultFontBitmap[FONT_SPRITE_COUNT*FONT_SPRITE_WIDTH*FONT_SPRITE_HEIGHT*4];

Font defaultFont = {
	.width = FONT_SPRITE_WIDTH,
	.height = FONT_SPRITE_COUNT*FONT_SPRITE_HEIGHT,
	.bitmap = (uint32_t*)defaultFontBitmap,
};
