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

#define DEFAULT_WIDGET_CAPACITY 100

typedef enum Widget_Type {
	CANVAS,
	FRAME,
	ROW,
	LABEL,
} Widget_Type;

typedef struct Widget_Data {
	Widget_Type type;
	uint16_t next;
	uint16_t previous;
	uint16_t parent;
	union {
		Viewport canvas;
		struct {uint16_t child; Alignment_Type horizontal_alignment, vertical_alignment;} frame;
		struct {uint16_t child; Alignment_Type horizontal_alignment;} row;
		struct {const char *text;} label;
	};
} Widget_Data;

// Used for changing color of character sprites.
static uint32_t character_scratch[8*8];

Theme default_theme = {
	.font = &default_font,
};

Window Window_create(const char *name, uint16_t width, uint16_t height) {
	Window window = {
		.name = name,
		.theme = &default_theme,
		.global_viewport = {
			.width = width,
			.height = height,
			// TODO: Allow changing the size of the framebuffer.
			.frame_buffer = calloc(MAX_WIDTH*MAX_HEIGHT, sizeof *window.global_viewport.frame_buffer),
		},
		.widget_capacity = DEFAULT_WIDGET_CAPACITY,
		.widgets = malloc(window.widget_capacity*sizeof *window.widgets),
		.mfb_window = mfb_open_ex(name, width, height, WF_RESIZABLE),
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
	free(window->widgets);
	mfb_close(window->mfb_window);
	*window = (Window){0};
}

uint16_t Window_get_width(Window *window) {
	return mfb_get_window_width(window->mfb_window);
}

uint16_t Window_get_height(Window *window) {
	return mfb_get_window_height(window->mfb_window);
}

Viewport *Window_get_global_viewport(Window *window) {
	return &window->global_viewport;
}

bool Window_update(Window *window) {
	mfb_update_state state = mfb_update(window->mfb_window, window->global_viewport.frame_buffer);
	if (state != STATE_OK || !mfb_wait_sync(window->mfb_window)) {
		return false;
	}
	window->global_viewport.width = mfb_get_window_width(window->mfb_window);
	window->global_viewport.height = mfb_get_window_height(window->mfb_window);
	return true;
}

void Viewport_draw_pixel(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y) {
	viewport->frame_buffer[y*viewport->width + x] = color;
}

void Viewport_draw_line(Viewport *viewport, uint32_t color, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y) {
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
			Viewport_draw_pixel(viewport, color, (uint32_t)x, (uint32_t)y);
			x += 1.0f;
			y += delta_y/delta_x;
		}
	} else {
		while (y < (float)end_y) {
			Viewport_draw_pixel(viewport, color, (uint32_t)x, (uint32_t)y);
			x += delta_x/delta_y;
			y += 1.0f;
		}
	}
}

void Viewport_draw_rectangle(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	Viewport_draw_line(viewport, color, x, y, x + width, y);
	Viewport_draw_line(viewport, color, x, y + height, x + width, y + height);
	Viewport_draw_line(viewport, color, x, y, x, y + height);
	Viewport_draw_line(viewport, color, x + width, y, x + width, y + height);
}

void Viewport_draw_rectangle_filled(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	for (uint16_t offset_y = 0; offset_y < height; ++offset_y) {
		for (uint16_t offset_x = 0; offset_x < width; ++offset_x) {
			Viewport_draw_pixel(viewport, color, x + offset_x, y + offset_y);
		}
	}
}

void Viewport_draw_sprite(Viewport *viewport, Sprite *sprite, uint16_t x, uint16_t y, float scale) {
	for (uint16_t offset_y = 0; offset_y < (uint32_t)(sprite->height*scale); ++offset_y) {
		for (uint16_t offset_x = 0; (uint32_t)(offset_x < sprite->width*scale); ++offset_x) {
			// Use nearest-neighbor scaling to render the bitmap.
			Viewport_draw_pixel(viewport, sprite->bitmap[(uint32_t)(offset_y/scale)*sprite->width + (uint32_t)(offset_x/scale)], x + offset_x, y + offset_y);
		}
	}
}

void Viewport_draw_text(Viewport *viewport, Font *font, const char *text, uint32_t color, uint16_t x, uint16_t y, float scale) {
	uint32_t character_x = x;
	uint32_t character_y = y;
	while (*text) {
		if (*text == '\n') {
			character_x = x;
			character_y += (FONT_SPRITE_HEIGHT + FONT_PADDING)*scale;
			++text;
			continue;
		}
		Sprite ch = Font_get_character_sprite(font, *text);
		Sprite colored_ch = {
			.width = ch.width,
			.height = ch.height,
			.bitmap = character_scratch,
		};
		// TODO: Figure out a way to color fonts without copying every character every time. Maybe make multiple colors of the same font atlas?
		Sprite_copy(&ch, &colored_ch);
		Sprite_apply_color(&colored_ch, color);
		Viewport_draw_sprite(viewport, &colored_ch, character_x, character_y, scale);
		character_x += (FONT_SPRITE_WIDTH + FONT_PADDING)*scale;
		++text;
	}
}

void Viewport_fill(Viewport *viewport, uint32_t color) {
	for (uint32_t i = 0; i < viewport->width*viewport->height; ++i) {
		viewport->frame_buffer[i] = color;
	}
}

static Widget Window_allocate_widget(Window *window) {
	if (window->widget_count >= window->widget_capacity) {
		// Reallocate widgets.
		// Return window->widget_capacity + 1 on `realloc()` failure.
	}
	uint16_t previous_count = window->widget_count;
	++window->widget_count;
	return previous_count;
}

Frame Frame_create(Window *window, Alignment_Type horizontal_alignment, Alignment_Type vertical_alignment) {
	Widget widget = Window_allocate_widget(window);
	if (widget > window->widget_capacity) {
		return widget;
	}
	window->widgets[widget] = (Widget_Data){
		.type = FRAME,
		.frame = {
			.horizontal_alignment = horizontal_alignment,
			.vertical_alignment = vertical_alignment,
		},
	};
	return widget;
}

Label Label_create(Window *window, const char *text) {
	Widget widget = Window_allocate_widget(window);
	if (widget > window->widget_capacity) {
		return widget;
	}
	window->widgets[widget] = (Widget_Data){
		.type = LABEL,
		.label = {
			.text = text
		},
	};
	return widget;
}

#undef MAX_WIDTH
#undef MAX_HEIGHT
