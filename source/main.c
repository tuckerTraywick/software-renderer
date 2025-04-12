#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "arena.h"
#include "color.h"
#include "sprite.h"
#include "window.h"

int main(void) {
	Arena heap = ArenaCreate(4*1024);
	FILE *file = fopen("atlas.bmp", "rb");
	assert(file);

	Sprite font = {
		.width = 8,
		.height = 24,
		.bitmap = ArenaAllocate(&heap, font.width*font.height*sizeof *font.bitmap),
	};
	if (!SpriteReadFromBmp(file, &font)) {
		fprintf(stderr, "Couldn't read file.\n");
		ArenaDestroy(&heap);
		fclose(file);
		return 1;
	}
	Sprite ch = SpriteGetFontCharacter(&font, 'B');
	SpriteApplyColor(&ch, COLOR_RED);

	Window window = WindowCreate("my window", 800, 600, 1920, 1080);
	if (!WindowIsOpen(&window)) {
		fprintf(stderr, "Couldn't create window. Exiting.\n");
		return 1;
	}
	
	WindowFill(&window, COLOR_BLACK);
	WindowDrawRectangle(&window, COLOR_RED, 200, 200, 100, 50);
	WindowDrawText(&window, &font, "ABC\nBC", COLOR_RED, 100, 100, 5);
	while (WindowIsOpen(&window)) {
		WindowUpdate(&window);
	}

	ArenaDestroy(&heap);
	WindowDestroy(&window);
	return 0;
}
