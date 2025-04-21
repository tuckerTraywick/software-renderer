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
	mfb_set_target_fps(60);
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
	Sprite sprite = Sprite_read_from_path("mario.bmp");
	if (!Sprite_is_valid(&sprite)) {
		fprintf(stderr, "Couldn't read sprite. Exiting.\n");
		Window_destroy(&window);
		return 1;
	}

	Viewport *viewport = Window_get_global_viewport(&window);
	Camera camera = {0};
	do {
		printf("angle = %f\n", camera.angle.x);
		Viewport_fill(viewport, black);
		Viewport_draw_line(viewport, red, (Vector2){-400, 0}, (Vector2){400, 0});
		Viewport_draw_line(viewport, red, (Vector2){0, -300}, (Vector2){0, 300});
		// Viewport_draw_sprite(viewport, &sprite, (Vector2){0, 0}, (Vector2){100, 100}, 0.0f);
		// Viewport_draw_line(viewport, white, (Vector2){-100, -100}, (Vector2){0, 0});
		Viewport_draw_line3(viewport, &camera, white, (Vector3){0, 0, 0}, (Vector3){100, 100, 0});
		camera.angle.z += 0.01f;
		if (camera.angle.z >= 2.0f*M_PI) {
			camera.angle.z = 0.0f;
		}
	} while (Window_update(&window));

	Window_destroy(&window);
	return 0;
}
