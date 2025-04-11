#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include "MiniFB.h"

#define RGB(r, g, b) (MFB_RGB(r, g, b))

#define ARGB(a, r, g, b) (MFB_ARGB(a, r, g, b))

#define COLOR_BLACK (ARGB(255, 0, 0, 0))

#define COLOR_WHITE (ARGB(255, 255, 255, 255))

#define COLOR_RED (ARGB(255, 255, 0, 0))

#define COLOR_GREEN (ARGB(255, 0, 255, 0))

#define COLOR_BLUE (ARGB(255, 0, 0, 255))

typedef struct Window {
	struct mfb_window *miniFBWindow;
	uint32_t *frameBuffer;
	const char *name;
	uint32_t maxWidth;
	uint32_t maxHeight;
	bool isOpen;
} Window;

Window WindowCreate(const char *name, uint32_t width, uint32_t height, uint32_t maxWidth, uint32_t maxHeight);

void WindowDestroy(Window *window);

void WindowUpdate(Window *window);

void WindowFill(Window *window, uint32_t color);

void WindowDrawPixel(Window *window, uint32_t color, uint32_t x, uint32_t y);

void WindowDrawLine(Window *window, uint32_t color, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY);

uint32_t WindowGetWidth(Window *window);

uint32_t WindowGetHeight(Window *window);

bool WindowIsOpen(Window *window);

#endif // WINDOW_H
