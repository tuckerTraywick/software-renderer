#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include "MiniFB.h"
#include "window.h"
#include "vector.h"
#include "color.h"
#include "sprite.h"

#define MAX_WIDTH 1920

#define MAX_HEIGHT 1080

void Window_draw_pixel(Window *window, Color color, Vector2 position) {
	// float x = roundf(Window_get_size(window).x*0.5f + position.x);
	// float y = roundf(Window_get_size(window).y*0.5f - position.y);
	// assert(x >= 0.0f && y >= 0.0f && "Invalid coordinate.");
	window->frame_buffer[(size_t)(position.y*Window_get_size(window).x + position.x)] = color;
}

// void Window_draw_pixel3(Window *window, Camera *camera, Color color, Vector3 position) {
// 	Window_draw_pixel(window, color, Camera_get_projection(camera, position));
// }

void Window_draw_line(Window *window, Color color, Vector2 start, Vector2 end) {
	const float delta_x = end.x - start.x;
	const float delta_y = end.y - start.y;
	float x = start.x;
	float y = start.y;
	// Draw the line using the DDA algorithm.
	if (fabs(delta_x) >= fabs(delta_y)) {
		while (fabs(x - end.x) > 1.0f) {
			Window_draw_pixel(window, color, (Vector2){x, y});
			x += (delta_x >= 0.0f) ? 1.0f : -1.0f;
			y += delta_y/fabs(delta_x);
		}
	} else {
		while (fabs(y - end.y) > 1.0f) {
			Window_draw_pixel(window, color, (Vector2){x, y});
			x += delta_x/fabs(delta_y);
			y += (delta_y >= 0.0f) ? 1.0f : -1.0f;
		}
	}
}

// void Window_draw_line3(Window *window, Camera *camera, Color color, Vector3 start, Vector3 end) {
// 	Vector2 screen_start = Camera_get_projection(camera, start);
// 	Vector2 screen_end = Camera_get_projection(camera, end);
// 	Window_draw_line(window, color, screen_start, screen_end);
// }

void Window_draw_rectangle(Window *window, Color color, Vector2 position, Vector2 size) {
	Window_draw_line(window, color, position, (Vector2){position.x + size.x, position.y});
	Window_draw_line(window, color, (Vector2){position.x, position.y + size.y}, (Vector2){position.x + size.x, position.y + size.y});
	Window_draw_line(window, color, position, (Vector2){position.x, position.y + size.y});
	Window_draw_line(window, color, (Vector2){position.x + size.x, position.y}, (Vector2){position.x + size.x, position.y + size.y});
}

// void Window_draw_rectangle3(Window *window, Camera *camera, Color color, Vector3 position, Vector2 size, Vector3 angle) {

// }

void Window_draw_rectangle_filled(Window *window, Color color, Vector2 position, Vector2 size) {
	for (float offset_y = 0; offset_y < size.y; ++offset_y) {
		for (float offset_x = 0; offset_x < size.x; ++offset_x) {
			Window_draw_pixel(window, color, (Vector2){position.x + offset_x, position.y + offset_y});
		}
	}
}

void Window_draw_sprite(Window *window, Sprite *sprite, Vector2 position, Vector2 size, float angle) {
	float scale_x = (float)size.x/(float)sprite->size.x;
	float scale_y = (float)size.y/(float)sprite->size.y;
	for (float offset_y = -size.y*0.5f; offset_y < size.y*0.5f; offset_y += 1.0f) {
		for (float offset_x = -size.x*0.5f; offset_x < size.x*0.5f; offset_x += 1.0f) {
			// Use nearest-neighbor scaling to render the bitmap.
			Vector2 sprite_position = (Vector2){
				offset_x/scale_x,
				offset_y/scale_y,
			};
			Window_draw_pixel(window, Sprite_get_pixel(sprite, sprite_position), (Vector2){position.x + offset_x, position.y + offset_y});
		}
	}
}

// void Window_draw_text(Window *window, Font *font, const char *text, ufloat color, uint16_t x, uint16_t y, float scale) {
// 	ufloat character_x = x;
// 	ufloat character_y = y;
// 	while (*text) {
// 		if (*text == '\n') {
// 			character_x = x;
// 			character_y += (FONT_SPRITE_HEIGHT + FONT_PADDING)*scale;
// 			++text;
// 			continue;
// 		}
// 		Sprite ch = Font_get_character_sprite(font, *text);
// 		Sprite colored_ch = {
// 			.width = ch.width,
// 			.height = ch.height,
// 			.bitmap = character_scratch,
// 		};
// 		// TODO: Figure out a way to color fonts without copying every character every time. Maybe make multiple colors of the same font atlas?
// 		Sprite_copy(&ch, &colored_ch);
// 		Sprite_apply_color(&colored_ch, color);
// 		Window_draw_sprite(window, &colored_ch, character_x, character_y, scale);
// 		character_x += (FONT_SPRITE_WIDTH + FONT_PADDING)*scale;
// 		++text;
// 	}
// }

void Window_fill(Window *window, Color color) {
	for (size_t i = 0; i < Window_get_size(window).x*Window_get_size(window).y; ++i) {
		window->frame_buffer[i] = color;
	}
}

Window Window_create(const char *name, Vector2 size) {
	Window window = {
		.name = name,
		// TODO: Allow changing the size of the framebuffer.
		// .frame_buffer = calloc(MAX_WIDTH*MAX_HEIGHT, sizeof *window.frame_buffer),
		.frame_buffer = calloc(size.x*size.y, sizeof *window.frame_buffer),
		.mfb_window = mfb_open_ex(name, size.x, size.y, WF_RESIZABLE),
	};
	if (window.mfb_window && !window.frame_buffer) {
		mfb_close(window.mfb_window);
	}
	if (window.frame_buffer && !window.mfb_window) {
		free((void*)window.frame_buffer);
	}
	return window;
}

bool Window_is_valid(Window *window) {
	return window->frame_buffer && window->mfb_window;
}

void Window_destroy(Window *window) {
	free((void*)window->frame_buffer);
	mfb_close(window->mfb_window);
	*window = (Window){0};
}

Vector2 Window_get_size(Window *window) {
	return (Vector2){mfb_get_window_width(window->mfb_window), mfb_get_window_height(window->mfb_window)};
}

bool Window_update(Window *window) {
	if (!mfb_wait_sync(window->mfb_window)) {
		return false;
	}
	mfb_update_state state = mfb_update(window->mfb_window, (float*)window->frame_buffer);
	if (state != STATE_OK) {
		return false;
	}
	return true;
}

// Vector2 Camera_get_projection(Camera *camera, Vector3 point) {
// 	// Translate.
// 	float z = fabs(point.z - camera->position.z)/10.0f;
// 	z = (z < 0.001f) ? 0.001f : z;
// 	Vector3 screen_coordinates = {
// 		(point.x - camera->position.x)/z,
// 		(point.y - camera->position.y)/z,
// 		point.z - camera->position.z,
// 	};
// 	// Rotate around the x axis.
// 	screen_coordinates = (Vector3){
// 		screen_coordinates.x,
// 		screen_coordinates.y*cosf(camera->angle.x) - screen_coordinates.z*sinf(camera->angle.x),
// 		screen_coordinates.y*sinf(camera->angle.x) + screen_coordinates.z*cosf(camera->angle.x),
// 	};
// 	// Rotate around the y axis.
// 	screen_coordinates = (Vector3){
// 		screen_coordinates.x*cosf(camera->angle.y) + screen_coordinates.z*sinf(camera->angle.y),
// 		screen_coordinates.y,
// 		-screen_coordinates.x*sinf(camera->angle.y) + screen_coordinates.z*cosf(camera->angle.y),
// 	};
// 	// Rotate around the z axis.
// 	screen_coordinates = (Vector3){
// 		screen_coordinates.x*cosf(camera->angle.z) - screen_coordinates.y*sinf(camera->angle.z),
// 		screen_coordinates.x*sinf(camera->angle.z) + screen_coordinates.y*cosf(camera->angle.z),
// 		screen_coordinates.z,
// 	};
// 	return (Vector2){
// 		screen_coordinates.x,
// 		screen_coordinates.y,
// 	};
// }

#undef MAX_WIDTH
#undef MAX_HEIGHT
