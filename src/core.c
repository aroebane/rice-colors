/* SPDX-FileCopyrightText: 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/core.h"

#include <math.h>

#define STR2(x) #x
#define STR(x) STR2(x)

/* Qt's qRound for positive values: floor(x + 0.5). */
static double round16(double x)
{
    return floor(x * 65535.0 + 0.5);
}

static double clamp01(double x)
{
    return x < 0.0 ? 0.0 : (x > 1.0 ? 1.0 : x);
}

static double q16_channel(double x)
{
    return round16(clamp01(x)) / 65535.0;
}

static double t16_channel(double x)
{
    return floor(clamp01(x) * 65535.0) / 65535.0;
}

ricecol_rgba_t ricecol_from_rgb8(uint32_t rrggbb)
{
    return ricecol_from_argb8(UINT32_C(0xff000000) | (rrggbb & UINT32_C(0xffffff)));
}

ricecol_rgba_t ricecol_from_argb8(uint32_t aarrggbb)
{
    ricecol_rgba_t c;
    c.a = (double)((aarrggbb >> 24U) & UINT32_C(0xff)) / 255.0;
    c.r = (double)((aarrggbb >> 16U) & UINT32_C(0xff)) / 255.0;
    c.g = (double)((aarrggbb >> 8U) & UINT32_C(0xff)) / 255.0;
    c.b = (double)(aarrggbb & UINT32_C(0xff)) / 255.0;
    return c;
}

ricecol_rgba_t ricecol_from_rgba16(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    ricecol_rgba_t c;
    c.r = (double)(r > UINT32_C(65535) ? UINT32_C(65535) : r) / 65535.0;
    c.g = (double)(g > UINT32_C(65535) ? UINT32_C(65535) : g) / 65535.0;
    c.b = (double)(b > UINT32_C(65535) ? UINT32_C(65535) : b) / 65535.0;
    c.a = (double)(a > UINT32_C(65535) ? UINT32_C(65535) : a) / 65535.0;
    return c;
}

uint32_t ricecol_to_argb8(ricecol_rgba_t c)
{
    uint32_t a = (uint32_t)floor(clamp01(c.a) * 255.0 + 0.5);
    uint32_t r = (uint32_t)floor(clamp01(c.r) * 255.0 + 0.5);
    uint32_t g = (uint32_t)floor(clamp01(c.g) * 255.0 + 0.5);
    uint32_t b = (uint32_t)floor(clamp01(c.b) * 255.0 + 0.5);
    return (a << 24U) | (r << 16U) | (g << 8U) | b;
}

void ricecol_to_rgba16(ricecol_rgba_t c, uint32_t *r, uint32_t *g, uint32_t *b, uint32_t *a)
{
    if (r)
        *r = (uint32_t)round16(clamp01(c.r));
    if (g)
        *g = (uint32_t)round16(clamp01(c.g));
    if (b)
        *b = (uint32_t)round16(clamp01(c.b));
    if (a)
        *a = (uint32_t)round16(clamp01(c.a));
}

ricecol_rgba_t ricecol_q16(ricecol_rgba_t c)
{
    ricecol_rgba_t o;
    o.r = q16_channel(c.r);
    o.g = q16_channel(c.g);
    o.b = q16_channel(c.b);
    o.a = q16_channel(c.a);
    return o;
}

ricecol_rgba_t ricecol_t16(ricecol_rgba_t c)
{
    ricecol_rgba_t o;
    o.r = t16_channel(c.r);
    o.g = t16_channel(c.g);
    o.b = t16_channel(c.b);
    o.a = t16_channel(c.a);
    return o;
}

double ricecol_normalize(double a)
{
    return a < 1.0 ? (a > 0.0 ? a : 0.0) : 1.0;
}

double ricecol_wrap(double a, double d)
{
    double r = fmod(a, d);
    return r < 0.0 ? d + r : (r > 0.0 ? r : 0.0);
}

const char *ricecol_version(void)
{
    return STR(RICECOL_VERSION_MAJOR) "." STR(RICECOL_VERSION_MINOR) "." STR(RICECOL_VERSION_MICRO);
}
