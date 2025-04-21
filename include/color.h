#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct Color {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} Color;

extern const Color transparent;
extern const Color black;
extern const Color white;
extern const Color red;
extern const Color green;
extern const Color blue;

#endif // COLOR_H
