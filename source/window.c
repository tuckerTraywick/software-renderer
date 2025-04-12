#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "MiniFB.h"
#include "window.h"
#include "sprite.h"

#define abs(x) ((x < 0) ? (-x) : (x))

// Used for coloring characters.
static uint32_t characterScratch[8*8];

Window WindowCreate(const char *name, uint32_t width, uint32_t height, uint32_t maxWidth, uint32_t maxHeight) {
	Window window = {
		.maxWidth = maxWidth,
		.maxHeight = maxHeight,
		.isOpen = true,
		.frameBuffer = calloc(maxWidth*maxHeight, sizeof *window.frameBuffer),
		.miniFBWindow = mfb_open_ex(name, width, height, WF_RESIZABLE),
	};
	// TODO: Handle these errors.
	assert(window.miniFBWindow && "Failed to create MiniFB window.");
	assert(window.frameBuffer && "Failed to allocate framebuffer.");
	if (!window.miniFBWindow || !window.frameBuffer) {window.isOpen = false;}
	return window;
}

void WindowDestroy(Window *window) {
	if (window->isOpen) {
		mfb_close(window->miniFBWindow);
	}
	free(window->frameBuffer);
	*window = (Window){0};
}

void WindowUpdate(Window *window) {
	if (!window->isOpen) {
		return;
	}

	mfb_update_state state = mfb_update(window->miniFBWindow, window->frameBuffer);
	if (state != STATE_OK) {
		window->isOpen = false;
		return;
	}

	if (!mfb_wait_sync(window->miniFBWindow)) {
		window->isOpen = false;
	}
}

void WindowFill(Window *window, uint32_t color) {
	// TODO: Maybe optimize this to copy in 64 byte chunks.
	for (uint32_t i = 0; i < WindowGetWidth(window)*WindowGetHeight(window); ++i) {
		window->frameBuffer[i] = color;	
	}
}

void WindowDrawPixel(Window *window, uint32_t color, uint32_t x, uint32_t y) {
	window->frameBuffer[y*WindowGetWidth(window) + x] = color;
}

void WindowDrawLine(Window *window, uint32_t color, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY) {
	// Make sure the coordinates are ordered least to greatest.
	if (startX > endX) {
		uint32_t temp = startX;
		startX = endX;
		endX = temp;
	}
	if (startY > endY) {
		uint32_t temp = startY;
		startY = endY;
		endY = temp;
	}
	const float deltaX = (float)endX - (float)startX;
	const float deltaY = (float)endY - (float)startY;
	float x = (float)startX;
	float y = (float)startY;
	// Draw the line using the DDA algorithm.
	if (deltaX >= deltaY) {
		while (x < (float)endX) {
			WindowDrawPixel(window, color, (uint32_t)x, (uint32_t)y);
			x += 1.0f;
			y += deltaY/deltaX;
		}
	} else {
		while (y < (float)endY) {
			WindowDrawPixel(window, color, (uint32_t)x, (uint32_t)y);
			x += deltaX/deltaY;
			y += 1.0f;
		}
	}
}

void WindowDrawSprite(Window *window, Sprite *sprite, uint32_t x, uint32_t y, uint32_t scale) {
	for (uint32_t offsetY = 0; offsetY < sprite->height*scale; ++offsetY) {
		for (uint32_t offsetX = 0; offsetX < sprite->width*scale; ++offsetX) {
			// Use nearest-neighbor scaling to render the bitmap.
			WindowDrawPixel(window, sprite->bitmap[offsetY/scale*sprite->width + offsetX/scale], x + offsetX, y + offsetY);
		}
	}
}

void WindowDrawText(Window *window, Sprite *font, const char *text, uint32_t color, uint32_t x, uint32_t y, uint32_t scale) {
	uint32_t characterX = x;
	uint32_t characterY = y;
	while (*text) {
		if (*text == '\n') {
			characterX = x;
			characterY += 9*scale;
			++text;
			continue;
		}
		Sprite ch = SpriteGetFontCharacter(font, *text);
		Sprite coloredCh = {
			.width = ch.width,
			.height = ch.height,
			.bitmap = characterScratch,
		};
		// TODO: Figure out a way to color fonts without copying every character every time. Maybe make multiple colors of the same font atlas?
		SpriteCopy(&ch, &coloredCh);
		SpriteApplyColor(&coloredCh, color);
		WindowDrawSprite(window, &coloredCh, characterX, characterY, scale);
		characterX += 8*scale;
		++text;
	}
}

uint32_t WindowGetWidth(Window *window) {
	return mfb_get_window_width(window->miniFBWindow);
}

uint32_t WindowGetHeight(Window *window) {
	return mfb_get_window_height(window->miniFBWindow);
}

bool WindowIsOpen(Window *window) {
	return window->isOpen;
}

#undef abs
