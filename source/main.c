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
	Font font = Font_read_from_path("font.bmp");
	if (!Font_is_valid(&font)) {
		fprintf(stderr, "Couldn't read sprite. Exiting.\n");
		Window_destroy(&window);
		return 1;
	}
	Sprite sprite = Sprite_read_from_path("mario.bmp");
	Viewport *viewport = Window_get_global_viewport(&window);

	uint16_t size = 0;
	do {
		// Viewport_fill(viewport, black);
		// Viewport_draw_sprite(viewport, &sprite, (Vector2){100, 100}, (Vector2){size, size}, (Vector3){0, 0, 0});
		// size = (size + 1)%100;
		printf("update = %d\n", mfb_update_ex(window.mfb_window, window.global_viewport.frame_buffer, 800, 600));
	} while (mfb_wait_sync(window.mfb_window));

	Window_destroy(&window);
	return 0;
}
