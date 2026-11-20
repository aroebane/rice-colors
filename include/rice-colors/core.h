/* rice-colors core (layer 0): color type, conversions, rounding, helpers.
 *
 * Ported helpers: normalize from kguiaddons_colorhelpers_p.h and wrap from
 * kcolorspaces.cpp.
 * SPDX-FileCopyrightText: 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_CORE_H
#define RICECOL_CORE_H

#include <stdint.h>

#include "rice-colors/export.h"

#define RICECOL_VERSION_MAJOR 0
#define RICECOL_VERSION_MINOR 1
#define RICECOL_VERSION_MICRO 0

RICECOL_BEGIN_DECLS

/* Each channel is in 0..1. Passed and returned by value. */
typedef struct {
    double r, g, b, a;
} ricecol_rgba_t;

/* 0xRRGGBB gives alpha 1; 0xAARRGGBB keeps its alpha byte. */
RICECOL_API ricecol_rgba_t ricecol_from_rgb8(uint32_t rrggbb);
RICECOL_API ricecol_rgba_t ricecol_from_argb8(uint32_t aarrggbb);
RICECOL_API ricecol_rgba_t ricecol_from_rgba16(uint32_t r, uint32_t g, uint32_t b, uint32_t a);

/* Rounded to the nearest 8-bit value, packed as 0xAARRGGBB. */
RICECOL_API uint32_t ricecol_to_argb8(ricecol_rgba_t c);
/* Rounded to the nearest 16-bit value (Qt rounding). */
RICECOL_API void
ricecol_to_rgba16(ricecol_rgba_t c, uint32_t *r, uint32_t *g, uint32_t *b, uint32_t *a);

/* Qt rounding: each channel becomes round_half_away(x * 65535) / 65535, as
 * QColor::fromRgbF stores it. Inputs outside 0..1 are clamped first. */
RICECOL_API ricecol_rgba_t ricecol_q16(ricecol_rgba_t c);
/* GTK2 rounding: each channel becomes trunc(x * 65535) / 65535. */
RICECOL_API ricecol_rgba_t ricecol_t16(ricecol_rgba_t c);

/* Clamp to 0..1. NaN gives 1.0, as KDE's code does (not as its comment says). */
RICECOL_API double ricecol_normalize(double a);
/* fmod(a, d) mapped into [0, d). */
RICECOL_API double ricecol_wrap(double a, double d);

/* "major.minor.micro" of the library (or of the vendored copy). */
RICECOL_API const char *ricecol_version(void);

RICECOL_END_DECLS

#endif
