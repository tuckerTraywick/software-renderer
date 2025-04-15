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
	Window window = Window_create("window", 1280, 720);
	if (!Window_is_valid(&window)) {
		fprintf(stderr, "Couldn't open window. Exiting.\n");
		return 1;
	}
	Viewport *global = Window_get_global_viewport(&window);
	Viewport_fill(global, COLOR_RED);
	Viewport_draw_pixel(global, COLOR_BLUE, 100, 100);
	do {

	} while (Window_update(&window));

	Window_destroy(&window);
	return 0;
}

// int main(void) {
// 	Window window = Window_create("name", 800, 600);
// 	if (!Window_is_open(&window)) {
// 		fprintf(stderr, "Couldn't open window. Exiting.\n");
// 		return 1;
// 	}
// 	Frame frame = Frame_create(&window, CENTER, CENTER);
// 	Row row = Row_create(&window, frame, CENTER);
// 	Label_create(&window, row, "text");
// 	Button button = Button_create(&window, row, "press me");
// 	Canvas canvas = Canvas_create(&window, row, 400, 400);
// 	Viewport viewport = Canvas_get_viewport(canvas);
// 	Viewport global = Window_get_global_viewport(window);

// 	while (Window_update(&window)) {
// 		if (Button_was_pressed(button)) {

// 		}

// 		Viewport_draw_line(viewport, COLOR_BLUE, 100, 100, 200, 200);
// 		Window_draw_ui(&window); // draws gui tree to framebuffer.
// 		Viewport_draw_line(global, COLOR_RED, 40, 40, 100, 100); // draws on top of the gui.
// 	}
	
// 	Window_destroy(&window);
// 	return 0;
// }
