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
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
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
	struct Widget_Data *widgets;
	uint16_t widget_capacity;
	uint16_t widget_count;
	uint16_t root_widget;
	struct mfb_window *mfb_window;
} Window;

typedef uint16_t Widget;

typedef Widget Canvas;

typedef Widget Frame;

typedef Widget Row;

typedef Widget Column;

typedef Widget Label;

extern Theme default_theme;

Window Window_create(const char *name, uint16_t width, uint16_t height);

bool Window_is_valid(Window *window);

void Window_destroy(Window *window);

uint16_t Window_get_width(Window *window);

uint16_t Window_get_height(Window *window);

Viewport *Window_get_global_viewport(Window *window);

bool Window_update(Window *window);

void Viewport_draw_pixel(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y);

void Viewport_draw_line(Viewport *viewport, uint32_t color, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);

void Viewport_draw_rectangle(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void Viewport_draw_rectangle_filled(Viewport *viewport, uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void Viewport_draw_sprite(Viewport *viewport, Sprite *sprite, uint16_t x, uint16_t y, float scale);

void Viewport_draw_text(Viewport *viewport, Font *font, const char *text, uint32_t color, uint16_t x, uint16_t y, float scale);

void Viewport_fill(Viewport *viewport, uint32_t color);

Frame Frame_create(Window *window, Alignment_Type horizontal_alignment, Alignment_Type vertical_alignment);

#endif // WINDOW_H
