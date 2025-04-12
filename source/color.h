#ifndef COLORS_H
#define COLORS_H

#include "MiniFB.h"

#define RGB(r, g, b) (MFB_RGB(r, g, b))

#define ARGB(a, r, g, b) (MFB_ARGB(a, r, g, b))

#define R(rgb) ( (uint8_t)((uint32_t)(rgb) >> 16) )
#define G(rgb) ( (uint8_t)((uint32_t)(rgb) >> 8) )
#define B(rgb) ( (uint8_t)(rgb) )
#define A(rgba) ( (uint8_t)((uint32_t)(rgba) >> 24) )

#define COLOR_BLACK (ARGB(255, 0, 0, 0))

#define COLOR_WHITE (ARGB(255, 255, 255, 255))

#define COLOR_RED (ARGB(255, 255, 0, 0))

#define COLOR_GREEN (ARGB(255, 0, 255, 0))

#define COLOR_BLUE (ARGB(255, 0, 0, 255))

#endif // COLOR_H
