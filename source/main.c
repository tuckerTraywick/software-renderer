#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
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

	Vector2 player_position = (Vector2){0, 0};
	Vector2 sprite_position = (Vector2){100, 50};
	Vector3 sprite_angle = (Vector3){
		.y = 180.0f/M_PI*atanf((float)(sprite_position.x - player_position.x)/(float)(sprite_position.y - player_position.y))
	};
	printf("y = %d\n", sprite_angle.y);
	uint16_t angle = 0;
	do {
		Viewport_fill(viewport, black);
		Viewport_draw_sprite(viewport, &sprite, (Vector2){400, 300}, (Vector2){100, 100}, sprite_angle);
		Viewport_draw_line(viewport, red, (Vector2){400, 0}, (Vector2){400, 600});
		Viewport_draw_line(viewport, red, (Vector2){0, 300}, (Vector2){800, 300});
		angle = (angle + 1)%360;
	} while (Window_update(&window));

	Window_destroy(&window);
	return 0;
}
