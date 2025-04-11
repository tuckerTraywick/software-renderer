#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Sprite {
	uint32_t width;
	uint32_t height;
	uint32_t *bitmap;
} Sprite;

#endif // SPRITE_H
