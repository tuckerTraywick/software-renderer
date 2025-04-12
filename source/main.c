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
	FILE *file = fopen("A.bmp", "rb");
	assert(file);

	Sprite sprite = {
		.width = 8,
		.height = 8,
		.bitmap = ArenaAllocate(&heap, sprite.width*sprite.height*sizeof *sprite.bitmap),
	};
	if (!SpriteReadFromBmp(file, &sprite)) {
		fprintf(stderr, "Couldn't read file.\n");
		ArenaDestroy(&heap);
		fclose(file);
		return 1;
	}

	Window window = WindowCreate("my window", 800, 600, 1920, 1080);
	WindowFill(&window, COLOR_BLACK);
	if (!WindowIsOpen(&window)) {
		fprintf(stderr, "Couldn't create window. Exiting.\n");
		return 1;
	}

	WindowDrawSprite(&window, &sprite, 100, 100, 5);
	while (WindowIsOpen(&window)) {
		WindowUpdate(&window);
	}

	ArenaDestroy(&heap);
	WindowDestroy(&window);
	return 0;
}
