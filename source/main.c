#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "arena.h"
#include "color.h"
#include "sprite.h"
#include "font.h"
#include "window.h"

int main(void) {
	Window window = Window_create("window", 800, 600);
	if (!Window_is_valid(&window)) {
		fprintf(stderr, "Couldn't open window. Exiting.\n");
		return 1;
	}

	while (Window_update(&window)) {
		
	}

	Window_destroy(&window);
	return 0;
}

// int main(void) {
// 	Window window = Window_create("name", 800, 600);
// 	if (!Window_is_open(&window)) {
// 		fprintf(stderr, "Couldn't open window. Exiting.\n");
// 		return 1;
// 	}
// 	Frame *frame = Frame_create(&window, CENTER, CENTER);
// 	Row *row = Row_create(&window, frame, CENTER);
// 	Label_create(&window, row, "text");
// 	Button *button = Button_create(&window, row, "press me");
// 	Canvas *canvas = Window_get_default_canvas(&window);

// 	while (true) {
// 		if (!Window_update(&window)) { // updates screen and events.
// 			break;
// 		}

// 		if (Button_was_pressed(button)) {

// 		}

// 		Window_draw(&window); // draws gui tree to framebuffer.
// 		Canvas_draw_line(&canvas, COLOR_BLUE, 100, 100, 200, 200); // will display on top of gui.
// 	}
	
// 	Window_destroy(&window);
// 	return 0;
// }
