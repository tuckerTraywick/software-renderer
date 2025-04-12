#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include "MiniFB.h"
#include "color.h"
#include "sprite.h"

typedef struct Window {
	const char *name;
	uint32_t maxWidth;
	uint32_t maxHeight;
	bool isOpen;
	uint32_t *frameBuffer;
	struct mfb_window *miniFBWindow;
} Window;

Window WindowCreate(const char *name, uint32_t width, uint32_t height, uint32_t maxWidth, uint32_t maxHeight);

void WindowDestroy(Window *window);

void WindowUpdate(Window *window);

void WindowFill(Window *window, uint32_t color);

void WindowDrawPixel(Window *window, uint32_t color, uint32_t x, uint32_t y);

void WindowDrawLine(Window *window, uint32_t color, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY);

void WindowDrawRectangle(Window *window, uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

void WindowDrawSprite(Window *window, Sprite *sprite, uint32_t x, uint32_t y, float scale);

void WindowDrawText(Window *window, Sprite *font, const char *text, uint32_t color, uint32_t x, uint32_t y, float scale);

uint32_t WindowGetWidth(Window *window);

uint32_t WindowGetHeight(Window *window);

bool WindowIsOpen(Window *window);

#endif // WINDOW_H
