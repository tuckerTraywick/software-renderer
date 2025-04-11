#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "sprite.h"
#include "window.h"

#define w COLOR_RED

#define b COLOR_BLACK

static uint32_t bitmap[] = {
	b, w, w, w, w, w, w, b,
	b, w, b, b, b, b, w, b,
	b, w, b, b, b, b, w, b,
	b, w, b, b, b, b, w, b,
	b, w, b, b, b, b, w, b,
	b, w, b, b, b, b, w, b,
	b, w, b, b, b, b, w, b,
	b, w, w, w, w, w, w, b,
};

int main(void) {
	Sprite sprite = {
		.width = 8,
		.height = 8,
		.bitmap = bitmap,
	};

	Window window = WindowCreate("my window", 800, 600, 1920, 1080);
	if (!WindowIsOpen(&window)) {
		fprintf(stderr, "Couldn't create window. Exiting.\n");
		return 1;
	}

	WindowDrawSprite(&window, &sprite, 300, 300, 10);
	while (WindowIsOpen(&window)) {
		WindowUpdate(&window);
	}
	WindowDestroy(&window);
	return 0;
}
