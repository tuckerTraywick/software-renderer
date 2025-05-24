#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "color.h"
#include "sprite.h"

typedef struct Window {
	const char *name;
	Color *frame_buffer;
	struct mfb_window *mfb_window;
} Window;

typedef struct Camera {
	Vector3 position;
	Vector3 angle;
	Vector2 fov;
} Camera;

void Window_draw_pixel(Window *window, Color color, Vector2 position);

void Window_draw_pixel3(Window *window, Camera *camera, Color color, Vector3 position);

void Window_draw_line(Window *window, Color color, Vector2 start, Vector2 end);

void Window_draw_line3(Window *window, Camera *camera, Color color, Vector3 start, Vector3 end);

void Window_draw_rectangle(Window *window, Color color, Vector2 position, Vector2 size);

void Window_draw_rectangle3(Window *window, Camera *camera, Color color, Vector3 position, Vector2 size, Vector3 angle);

void Window_draw_rectangle_filled(Window *window, Color color, Vector2 position, Vector2 size);

void Window_draw_rectangle_filled3(Window *window, Camera *camera, Color color, Vector3 position, Vector3 size, Vector3 angle);

void Window_draw_sprite(Window *window, Sprite *sprite, Vector2 position, Vector2 size, float angle);

void Window_draw_sprite3(Window *window, Camera *camera, Sprite *sprite, Vector3 position, Vector3 size, Vector3 angle);

void Window_draw_text(Window *window, Font *font, uint16_t font_size, Color color, const char *text, Vector2 position, float angle);

void Window_draw_text3(Window *window, Camera *camera, Font *font, uint16_t font_size, Color color, const char *text, Vector3 position, Vector3 angle);

void Window_fill(Window *window, Color color);

Window Window_create(const char *name, Vector2 size);

bool Window_is_valid(Window *window);

void Window_destroy(Window *window);

Vector2 Window_get_size(Window *window);

bool Window_update(Window *window);

// Vector2 Camera_get_projection(Camera *camera, Vector3 point);

#endif // WINDOW_H
