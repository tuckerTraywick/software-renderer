#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "vector.h"
#include "color.h"
#include "sprite.h"
#include "window.h"
#include "entity.h"

int main(void) {
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
	Viewport *viewport = Window_get_global_viewport(&window);

	Viewport_draw_line(viewport, red, (Vector2){100, 100}, (Vector2){200, 200});
	do {

	} while (Window_update(&window));

	Window_destroy(&window);
	return 0;
}
