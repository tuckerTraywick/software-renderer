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
	float fontScale;
	float uiScale;
	uint32_t backgroundColor;
	uint32_t activeBorderColor;
	uint32_t inactiveBorderColor;
	uint32_t activeFillColor;
	uint32_t inactiveFillColor;
	uint32_t activeTextColor;
	uint32_t inactiveTextColor;
} Theme;

typedef struct Window {
	const char *name;
	uint32_t maxWidth;
	uint32_t maxHeight;
	Theme *theme;
	bool isOpen;
	uint32_t *frameBuffer;
	struct mfb_window *miniFBWindow;
} Window;

extern Theme defaultTheme;

Window WindowCreate(const char *name, uint32_t width, uint32_t height, uint32_t maxWidth, uint32_t maxHeight);

void WindowDestroy(Window *window);

void WindowUpdate(Window *window);

void WindowFill(Window *window);

void WindowMove(Window *window, uint32_t vertical, uint32_t horizontal);

void WindowDrawPixel(Window *window);

void WindowDrawLine(Window *window, uint32_t xLength, uint32_t yLength);

void WindowDrawRectangle(Window *window, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

void WindowDrawFilledRectangle(Window *window, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

void WindowDrawSprite(Window *window, Sprite *sprite, uint32_t x, uint32_t y, float scale);

void WindowDrawText(Window *window, const char *text, uint32_t x, uint32_t y, uint32_t *width, uint32_t *height);

void WindowDrawButton(Window *window, bool *value, const char *text);

uint32_t WindowGetWidth(Window *window);

uint32_t WindowGetHeight(Window *window);

bool WindowIsOpen(Window *window);

#endif // WINDOW_H
