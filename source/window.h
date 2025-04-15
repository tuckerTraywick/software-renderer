#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include "MiniFB.h"
#include "color.h"
#include "sprite.h"
#include "font.h"

typedef enum Alignment_Type {
	CENTER,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
} Alignment_Type;

typedef struct Theme {
	Font *font;
	float font_scale;
	float ui_scale;
	uint32_t background_color;
	uint32_t active_border_color;
	uint32_t inactive_border_color;
	uint32_t active_fill_color;
	uint32_t inactive_fill_color;
	uint32_t active_text_color;
	uint32_t inactive_text_color;
} Theme;

typedef struct Viewport {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint32_t *frame_buffer;
} Viewport;

typedef struct Window {
	const char *name;
	Theme *theme;
	Viewport global_viewport;
	struct mfb_window *mfb_window;
} Window;

typedef uint16_t Widget;

typedef Widget Frame;

typedef Widget Row;

typedef Widget Column;

typedef Widget Canvas;

typedef Widget Label;

extern Theme default_theme;

Window Window_create(const char *name, uint16_t width, uint16_t height);

bool Window_is_valid(Window *window);

void Window_destroy(Window *window);

uint16_t Window_get_width(Window *window);

uint16_t Window_get_height(Window *window);

Viewport *Window_get_global_viewport(Window *window);

bool Window_update(Window *window);

void Window_draw_ui(Window *window);

void Viewport_draw_pixel(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y);

void Viewport_fill(Viewport *viewport, uint32_t color);

#endif // WINDOW_H
