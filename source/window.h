#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include "MiniFB.h"
#include "color.h"
#include "sprite.h"
#include "font.h"

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
	uint16_t max_width;
	uint16_t max_height;
	Theme *theme;
	Canvas default_canvas;
	bool is_open;
	uint32_t *frame_buffer;
	struct mfb_window *mfb_window;
} Window;

typedef struct Label {
	const char *text;
} Label;

extern Theme default_theme;

Window Window_create(const char *name, uint16_t width, uint16_t height, uint16_t max_width, uint16_t max_height);

void Window_destroy(Window *window);

void Window_update(Window *window);

bool Window_is_open(Window *window);

Canvas *WindowGetDefaultCanvas(Window *window);

Canvas *WindowGetCanvas(Window *window, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

Canvas *WindowAddCanvas(Window *window, uint16_t width, uint16_t height);

Label *WindowAddLabel(Window *window, const char *text);



#endif // WINDOW_H
