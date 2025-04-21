#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "MiniFB.h"

#include <stdint.h>
#include <stdlib.h>
#include "vector.h"
#include "color.h"
#include "sprite.h"
#include "window.h"
#include "entity.h"

int main(void) {
	mfb_set_target_fps(60);
	Window window = Window_create("window", (Vector2){800, 600});
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
	Sprite sprite = Sprite_read_from_path("mario.bmp");
	if (!Sprite_is_valid(&sprite)) {
		fprintf(stderr, "Couldn't read sprite. Exiting.\n");
		Window_destroy(&window);
		return 1;
	}
	Viewport *viewport = Window_get_global_viewport(&window);

	uint16_t angle = 0;
	do {
		Viewport_fill(viewport, black);
		Viewport_draw_sprite(viewport, &sprite, (Vector2){100, 100}, (Vector2){100, 100}, (Vector3){angle, 0, 0});
		Viewport_draw_line(viewport, red, (Vector2){150, 100}, (Vector2){150, 300});
		angle = (angle + 1)%360;
	} while (Window_update(&window));

	Window_destroy(&window);
	return 0;
}
