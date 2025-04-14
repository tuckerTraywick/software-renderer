#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "window.h"
#include "sprite.h"
#include "font.h"

#define MAX_WIDTH 1920

#define MAX_HEIGHT 1080

// Used for coloring characters.
static uint32_t characterScratch[8*8];

Theme default_theme = {
	.font = &default_font,
};

Window Window_create(const char *name, uint16_t width, uint16_t height) {
	Window window = {
		.theme = &default_theme,
		.frame_buffer = calloc(MAX_WIDTH*MAX_HEIGHT, sizeof *window.frame_buffer),
		.mfb_window = mfb_open_ex(name, width, height, WF_RESIZABLE),
	};
	if (window.mfb_window && !window.frame_buffer) {
		mfb_close(window.mfb_window);
	}
	if (window.frame_buffer && !window.mfb_window) {
		free(window.frame_buffer);
	}
	return window;
}

bool Window_is_valid(Window *window) {
	return window->frame_buffer && window->mfb_window;
}

void Window_destroy(Window *window) {
	free(window->frame_buffer);
	*window = (Window){0};
}

uint32_t Window_get_width(Window *window) {
	return mfb_get_window_width(window->mfb_window);
}

uint32_t Window_get_height(Window *window) {
	return mfb_get_window_height(window->mfb_window);
}

bool Window_update(Window *window) {
	mfb_update_state state = mfb_update(window->mfb_window, window->frame_buffer);
	if (state != STATE_OK) {
		return false;
	}
	if (!mfb_wait_sync(window->mfb_window)) {
		return false;
	}
	return true;
}

void Window_fill(Window *window, uint32_t color) {
	// TODO: Maybe optimize this to copy in 64 byte chunks.
	for (uint32_t i = 0; i < Window_get_width(window)*Window_get_height(window); ++i) {
		window->frame_buffer[i] = color;	
	}
}

void Window_draw_pixel(Window *window, uint32_t color, uint16_t x, uint16_t y) {
	window->frame_buffer[y*Window_get_width(window) + x] = color;
}

void Window_draw_line(Window *window, uint32_t color, uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y) {
	// Make sure the coordinates are ordered least to greatest.
	if (start_x > end_x) {
		uint16_t temp = start_x;
		start_x = end_x;
		end_x = temp;
	}
	if (start_y > end_y) {
		uint16_t temp = start_y;
		start_y = end_y;
		end_y = temp;
	}
	const float delta_x = (float)end_x - (float)start_x;
	const float delta_y = (float)end_y - (float)start_y;
	float x = (float)start_x;
	float y = (float)start_y;
	// Draw the line using the DDA algorithm.
	if (delta_x >= delta_y) {
		while (x < (float)end_x) {
			Window_draw_pixel(window, color, (uint32_t)x, (uint32_t)y);
			x += 1.0f;
			y += delta_y/delta_x;
		}
	} else {
		while (y < (float)end_y) {
			Window_draw_pixel(window, color, (uint32_t)x, (uint32_t)y);
			x += delta_x/delta_y;
			y += 1.0f;
		}
	}
}

void Window_draw_rectangle(Window *window, uint32_t color, uint16_t x, uint16_t y, uint32_t width, uint32_t height) {
	Window_draw_line(window, color, x, y, x + width, y);
	Window_draw_line(window, color, x, y + height, x + width, y + height);
	Window_draw_line(window, color, x, y, x, y + height);
	Window_draw_line(window, color, x + width, y, x + width, y + height);
}

void Window_draw_rectangle_filled(Window *window, uint32_t color, uint16_t x, uint16_t y, uint32_t width, uint32_t height) {
	for (uint16_t offset_y = 0; offset_y < height; ++offset_y) {
		for (uint16_t offset_x = 0; offset_x < width; ++offset_x) {
			Window_draw_pixel(window, color, x + offset_x, y + offset_y);
		}
	}
}

void Window_draw_sprite(Window *window, Sprite *sprite, uint16_t x, uint16_t y, float scale) {
	for (uint16_t offset_y = 0; offset_y < (uint32_t)(sprite->height*scale); ++offset_y) {
		for (uint16_t offset_x = 0; (uint32_t)(offset_x < sprite->width*scale); ++offset_x) {
			// Use nearest-neighbor scaling to render the bitmap.
			Window_draw_pixel(window, sprite->bitmap[(uint32_t)(offset_y/scale)*sprite->width + (uint32_t)(offset_x/scale)], x + offset_x, y + offset_y);
		}
	}
}

void Window_draw_text(Window *window, Font *font, const char *text, uint32_t color, uint16_t x, uint16_t y, float scale) {
	uint32_t character_x = x;
	uint32_t character_y = y;
	while (*text) {
		if (*text == '\n') {
			character_x = x;
			character_y += 9*scale;
			++text;
			continue;
		}
		Sprite ch = Font_get_character_sprite(font, *text);
		Sprite coloredCh = {
			.width = ch.width,
			.height = ch.height,
			.bitmap = characterScratch,
		};
		// TODO: Figure out a way to color fonts without copying every character every time. Maybe make multiple colors of the same font atlas?
		Sprite_copy(&ch, &coloredCh);
		Sprite_apply_color(&coloredCh, color);
		Window_draw_sprite(window, &coloredCh, character_x, character_y, scale);
		character_x += 8*scale;
		++text;
	}
}
