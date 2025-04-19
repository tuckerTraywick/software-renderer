#include "font.h"
#include "sprite.h"

char default_font_bitmap[FONT_SPRITE_COUNT*FONT_SPRITE_WIDTH*FONT_SPRITE_HEIGHT*4];

Font default_font = {
	.width = FONT_SPRITE_WIDTH,
	.height = FONT_SPRITE_COUNT*FONT_SPRITE_HEIGHT,
	.bitmap = (uint32_t*)default_font_bitmap,
};

Sprite Font_get_character_sprite(Font *font, char ch) {
	return Atlas_get_sprite((Atlas*)font, FONT_SPRITE_HEIGHT, (uint32_t)ch - 32);
}
