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

struct Window;
// Used for immediate rendering of 2d primitives.
typedef struct Canvas {
	struct Window *window;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
} Canvas;

typedef struct Window {
	const char *name;
	// uint16_t max_width;
	// uint16_t max_height;
	Theme *theme;
	Canvas default_canvas;
	uint32_t *frame_buffer;
	struct mfb_window *mfb_window;
} Window;

typedef struct Widget {
	struct Widget *next;
	struct Widget *previous;
	struct Widget *parent;
} Widget;

typedef struct Frame {
	Widget widget;
	Widget *child;
	Alignment_Type vertical_alignment;
	Alignment_Type horizontal_alignment;
} Frame;

typedef struct Row {
	Widget widget;
	Widget *child;
	Alignment_Type horizontal_alignment;
} Row;

typedef struct Column {
	Widget widget;
	Widget *child;
	Alignment_Type vertical_alignment;
} Column;

typedef struct Label {
	Widget widget;
	const char *text;
} Label;

extern Theme default_theme;

Window Window_create(const char *name, uint16_t width, uint16_t height);

bool Window_is_valid(Window *window);

void Window_destroy(Window *window);

bool Window_update(Window *window);

#endif // WINDOW_H
