#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "MiniFB.h"
#include "MiniFB_enums.h"

#include <stdint.h>
#include <stdlib.h>
#include "vector.h"
#include "color.h"
#include "sprite.h"
#include "window.h"
#include "entity.h"

int main(void) {
	Window window = Window_create("window", (Vector2){1920, 1080});
	if (!Window_is_valid(&window)) {
		fprintf(stderr, "Couldn't open window. Exiting.\n");
		return 1;
	}
	// Font font = Font_read_from_path("font.bmp");
	// if (!Font_is_valid(&font)) {
	// 	fprintf(stderr, "Couldn't read font. Exiting.\n");
	// 	Window_destroy(&window);
	// 	return 1;
	// }
	// Sprite sprite = Sprite_read_from_path("mario.bmp");
	// if (!Sprite_is_valid(&sprite)) {
	// 	fprintf(stderr, "Couldn't read sprite. Exiting.\n");
	// 	Window_destroy(&window);
	// 	return 1;
	// }

	float x = 100.0f;
	do {
		Window_fill(&window, black);
		Window_draw_line(&window, red, (Vector2){100, 100}, (Vector2){100 + x, 100});
		x += 10.0f;
	} while (Window_update(&window));

	Window_destroy(&window);
	return 0;
}

// int main(void) {
// 	mfb_set_target_fps(60);
// 	Window window = Window_create("window", (Vector2){1920, 1080});
// 	if (!Window_is_valid(&window)) {
// 		fprintf(stderr, "Couldn't open window. Exiting.\n");
// 		return 1;
// 	}

// 	bool pressed = false;
// 	float value = 0;
// 	do {
// 		Window_fill(&window, black);
// 		Window_begin_subwindow(&window, 123, 456, 400, 200);
// 			Window_begin_row(&window, CENTER);
// 				Window_draw_label(&window, "hi");
// 				if (Window_draw_button(&window, &pressed)) {
// 					do_something();
// 				}
// 				Window_draw_slider(&window, &value)
// 			Window_end_row(&window);
// 		Window_end_subwindow(&window);
// 	} while (Window_update(&window));

// 	Window_destroy(&window);
// 	return 0;
// }
