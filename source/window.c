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

Window Window_create(const char *name, Vector2 size) {
	Window window = {
		.name = name,
		.global_viewport = {
			.size = size,
			// TODO: Allow changing the size of the framebuffer.
			.frame_buffer = calloc(MAX_WIDTH*MAX_HEIGHT, sizeof *window.global_viewport.frame_buffer),
			// .frame_buffer = calloc(size.x*size.y, sizeof *window.global_viewport.frame_buffer),
		},
		.mfb_window = mfb_open_ex(name, size.x, size.y, WF_RESIZABLE),
	};
	if (window.mfb_window && !window.global_viewport.frame_buffer) {
		mfb_close(window.mfb_window);
	}
	if (window.global_viewport.frame_buffer && !window.mfb_window) {
		free(window.global_viewport.frame_buffer);
	}
	return window;
}

bool Window_is_valid(Window *window) {
	return window->global_viewport.frame_buffer && window->mfb_window;
}

void Window_destroy(Window *window) {
	free(window->global_viewport.frame_buffer);
	mfb_close(window->mfb_window);
	*window = (Window){0};
}

Vector2 Window_get_size(Window *window) {
	return window->global_viewport.size;
}

Viewport *Window_get_global_viewport(Window *window) {
	return &window->global_viewport;
}

bool Window_update(Window *window) {
	if (!mfb_wait_sync(window->mfb_window)) {
		return false;
	}
	mfb_update_state state = mfb_update(window->mfb_window, (uint32_t*)window->global_viewport.frame_buffer);
	if (state != STATE_OK) {
		return false;
	}
	window->global_viewport.size.x = mfb_get_window_width(window->mfb_window);
	window->global_viewport.size.y = mfb_get_window_height(window->mfb_window);
	return true;
}

void Viewport_draw_pixel(Viewport *viewport, Color color, Vector2 position) {
	viewport->frame_buffer[position.y*viewport->size.x + position.x] = color;
}

void Viewport_draw_line(Viewport *viewport, Color color, Vector2 start, Vector2 end) {
	// Make sure the coordinates are ordered least to greatest.
	if (start.x > end.x) {
		uint16_t temp = start.x;
		start.x = end.x;
		end.x = temp;
	}
	if (start.y > end.y) {
		uint16_t temp = start.y;
		start.y = end.y;
		end.y = temp;
	}
	const float delta_x = (float)end.x - (float)start.x;
	const float delta_y = (float)end.y - (float)start.y;
	float x = (float)start.x;
	float y = (float)start.y;
	// Draw the line using the DDA algorithm.
	if (delta_x >= delta_y) {
		while (x < (float)end.x) {
			Viewport_draw_pixel(viewport, color, (Vector2){(uint16_t)x, (uint16_t)y});
			x += 1.0f;
			y += delta_y/delta_x;
		}
	} else {
		while (y < (float)end.y) {
			Viewport_draw_pixel(viewport, color, (Vector2){(uint16_t)x, (uint16_t)y});
			x += delta_x/delta_y;
			y += 1.0f;
		}
	}
}

void Viewport_draw_rectangle(Viewport *viewport, Color color, Vector2 position, Vector2 size) {
	Viewport_draw_line(viewport, color, position, (Vector2){position.x + size.x, position.y});
	Viewport_draw_line(viewport, color, (Vector2){position.x, position.y + size.y}, (Vector2){position.x + size.x, position.y + size.y});
	Viewport_draw_line(viewport, color, position, (Vector2){position.x, position.y + size.y});
	Viewport_draw_line(viewport, color, (Vector2){position.x + size.x, position.y}, (Vector2){position.x + size.x, position.y + size.y});
}

void Viewport_draw_rectangle_filled(Viewport *viewport, Color color, Vector2 position, Vector2 size) {
	for (uint16_t offset_y = 0; offset_y < size.y; ++offset_y) {
		for (uint16_t offset_x = 0; offset_x < size.x; ++offset_x) {
			Viewport_draw_pixel(viewport, color, (Vector2){position.x + offset_x, position.y + offset_y});
		}
	}
}

void Viewport_draw_sprite(Viewport *viewport, Sprite *sprite, Vector2 position, Vector2 size, Vector3 angle) {
	float angle_divisions = 360.0f;
	float rotation_scale_x = cosf(2.0f*M_PI/angle_divisions*(float)angle.x);
	float scale_x = rotation_scale_x*(float)size.x/(float)sprite->size.x;
	float scale_y = (float)size.y/(float)sprite->size.y;
	ssize_t adjusted_size_x = fabsf(rotation_scale_x*size.x);
	position.x -= 0.5f*adjusted_size_x;
	for (ssize_t offset_y = 0; offset_y < size.y; ++offset_y) {
		for (ssize_t offset_x = 0; offset_x < adjusted_size_x; ++offset_x) {
			// Use nearest-neighbor scaling to render the bitmap.
			ssize_t pixel_index = (ssize_t)(offset_y/scale_y)*sprite->size.x + (ssize_t)(offset_x/scale_x);
			Viewport_draw_pixel(viewport, sprite->bitmap[pixel_index], (Vector2){position.x + offset_x, position.y + offset_y});
		}
	}
}

// void Viewport_draw_text(Viewport *viewport, Font *font, const char *text, uint32_t color, uint16_t x, uint16_t y, float scale) {
// 	uint32_t character_x = x;
// 	uint32_t character_y = y;
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
// 		Viewport_draw_sprite(viewport, &colored_ch, character_x, character_y, scale);
// 		character_x += (FONT_SPRITE_WIDTH + FONT_PADDING)*scale;
// 		++text;
// 	}
// }

void Viewport_fill(Viewport *viewport, Color color) {
	for (size_t i = 0; i < viewport->size.x*viewport->size.y; ++i) {
		viewport->frame_buffer[i] = color;
	}
}

#undef MAX_WIDTH
#undef MAX_HEIGHT
