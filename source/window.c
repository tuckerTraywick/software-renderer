#include <assert.h>

#include <stdint.h>
#include <stdlib.h>
#include "window.h"
#include "MiniFB.h"

Window WindowCreate(const char *name, uint32_t width, uint32_t height, uint32_t maxWidth, uint32_t maxHeight) {
	Window window = {
		.miniFBWindow = mfb_open_ex(name, width, height, WF_RESIZABLE),
		.frameBuffer = calloc(maxWidth*maxHeight, sizeof *window.frameBuffer),
		.maxWidth = maxWidth,
		.maxHeight = maxHeight,
		.isOpen = true,
	};
	// TODO: Handle these errors.
	assert(window.miniFBWindow && "Failed to create MiniFB window.");
	assert(window.frameBuffer && "Failed to allocate framebuffer.");
	// if (!window.miniFBWindow || !window.frameBuffer) {window.isOpen = false;}
	return window;
}

void WindowDestroy(Window *window) {
	mfb_close(window->miniFBWindow);
	free(window->frameBuffer);
	*window = (Window){0};
}

void WindowUpdate(Window *window) {
	mfb_update_state state = mfb_update(window->miniFBWindow, window->frameBuffer);
	if (state != STATE_OK) {
		window->isOpen = false;
	}
}

void WindowFill(Window *window, uint32_t color) {
	// TODO: Maybe optimize this to copy in 64 byte chunks.
	for (uint32_t i = 0; i < WindowGetWidth(window)*WindowGetHeight(window); ++i) {
		window->frameBuffer[i] = color;	
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
