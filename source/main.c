#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "arena.h"
#include "sprite.h"
#include "window.h"

int main(void) {
	Arena heap = ArenaCreate(4*1024);
	FILE *file = fopen("mario.bmp", "rb");
	assert(file);

	Sprite sprite = {
		.width = 512,
		.height = 512,
		.bitmap = ArenaAllocate(&heap, sprite.width*sprite.height*sizeof *sprite.bitmap),
	};
	if (!SpriteReadFromBmp(file, &sprite)) {
		fprintf(stderr, "Couldn't read file.\n");
		ArenaDestroy(&heap);
		fclose(file);
		return 1;
	}

	Window window = WindowCreate("my window", 800, 600, 1920, 1080);
	if (!WindowIsOpen(&window)) {
		fprintf(stderr, "Couldn't create window. Exiting.\n");
		return 1;
	}

	WindowDrawSprite(&window, &sprite, 0, 0, 1);
	while (WindowIsOpen(&window)) {
		WindowUpdate(&window);
	}

	ArenaDestroy(&heap);
	WindowDestroy(&window);
	return 0;
}
