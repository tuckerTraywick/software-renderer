#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "window.h"

int main(void) {
	Window window = WindowCreate("my window", 800, 600, 1920, 1080);
	if (!WindowIsOpen(&window)) {
		fprintf(stderr, "Couldn't create window. Exiting.");
		return 1;
	}

	WindowFill(&window, COLOR_RED);
	while (WindowIsOpen(&window)) {
		WindowUpdate(&window);

	}
	WindowDestroy(&window);
	return 0;
}
