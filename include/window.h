#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "color.h"
#include "sprite.h"

typedef struct Viewport {
	Vector2 position;
	Vector2 size;
	volatile Color *frame_buffer;
} Viewport;

typedef struct Window {
	const char *name;
	Viewport global_viewport;
	struct mfb_window *mfb_window;
} Window;

Window Window_create(const char *name, Vector2 size);

bool Window_is_valid(Window *window);

void Window_destroy(Window *window);

Vector2 Window_get_size(Window *window);

Viewport *Window_get_global_viewport(Window *window);

bool Window_update(Window *window);

void Viewport_draw_pixel(Viewport *viewport, Color color, Vector2 position);

void Viewport_draw_line(Viewport *viewport, Color color, Vector2 start, Vector2 end);

void Viewport_draw_rectangle(Viewport *viewport, Color color, Vector2 position, Vector2 size);

void Viewport_draw_rectangle_filled(Viewport *viewport, Color color, Vector2 position, Vector2 size);

void Viewport_draw_sprite(Viewport *viewport, Sprite *sprite, Vector2 position, Vector2 size, uint16_t angle);

void Viewport_draw_sprite3(Viewport *viewport, Sprite *sprite, Vector2 position, Vector2 size, Vector3 angle, Vector3 camera_angle);

void Viewport_draw_text(Viewport *viewport, Font *font, const char *text, Color color, Vector2 position, Vector2 size, Vector3 angle);

void Viewport_fill(Viewport *viewport, Color color);

#endif // WINDOW_H
