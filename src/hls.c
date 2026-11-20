/* SPDX-FileCopyrightText: 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/hls.h"

/* The operations below follow gtk/gtkstyle.c in the same order, so the result is
 * bit-for-bit what GTK2 computes. */

static void rgb_to_hls(double *r, double *g, double *b)
{
    double min, max, red = *r, green = *g, blue = *b, h, l, s, delta;

    if (red > green) {
        max = red > blue ? red : blue;
        min = green < blue ? green : blue;
    } else {
        max = green > blue ? green : blue;
        min = red < blue ? red : blue;
    }

    l = (max + min) / 2;
    s = 0;
    h = 0;

    if (max != min) {
        if (l <= 0.5)
            s = (max - min) / (max + min);
        else
            s = (max - min) / (2 - max - min);

        delta = max - min;
        if (red == max)
            h = (green - blue) / delta;
        else if (green == max)
            h = 2 + (blue - red) / delta;
        else if (blue == max)
            h = 4 + (red - green) / delta;

        h *= 60;
        if (h < 0.0)
            h += 360;
    }
    *r = h;
    *g = l;
    *b = s;
}

static double hls_channel(double m1, double m2, double hue)
{
    while (hue > 360)
        hue -= 360;
    while (hue < 0)
        hue += 360;

    /* UPSTREAM-QUIRK: hue == 360 exactly is not wrapped to 0 and lands in the
     * last branch (m1), where 0 would give m1 + (m2 - m1) * 0 / 60, also m1.
     * Both agree, so the edge is harmless; kept as GTK2 has it. */
    if (hue < 60)
        return m1 + (m2 - m1) * hue / 60;
    if (hue < 180)
        return m2;
    if (hue < 240)
        return m1 + (m2 - m1) * (240 - hue) / 60;
    return m1;
}

static void hls_to_rgb(double *h, double *l, double *s)
{
    double lightness = *l, saturation = *s, m1, m2;

    if (lightness <= 0.5)
        m2 = lightness * (1 + saturation);
    else
        m2 = lightness + saturation - lightness * saturation;
    m1 = 2 * lightness - m2;

    if (saturation == 0) {
        *h = lightness;
        *l = lightness;
        *s = lightness;
    } else {
        double r = hls_channel(m1, m2, *h + 120);
        double g = hls_channel(m1, m2, *h);
        double b = hls_channel(m1, m2, *h - 120);
        *h = r;
        *l = g;
        *s = b;
    }
}

static uint32_t truncate16(double v)
{
    /* GTK2 assigns the double to a guint16: truncation, not rounding. A value a
     * hair above 65535 cannot happen because every channel is at most 1.0. */
    return (uint32_t)(v * 65535.0);
}

ricecol_rgba_t ricecol_gtk2_shade(ricecol_rgba_t bg, double k)
{
    uint32_t r16, g16, b16, a16;
    double red, green, blue;

    ricecol_to_rgba16(bg, &r16, &g16, &b16, &a16);
    red = (double)r16 / 65535.0;
    green = (double)g16 / 65535.0;
    blue = (double)b16 / 65535.0;

    rgb_to_hls(&red, &green, &blue);

    green *= k;
    if (green > 1.0)
        green = 1.0;
    else if (green < 0.0)
        green = 0.0;

    blue *= k;
    if (blue > 1.0)
        blue = 1.0;
    else if (blue < 0.0)
        blue = 0.0;

    hls_to_rgb(&red, &green, &blue);

    {
        ricecol_rgba_t o =
            ricecol_from_rgba16(truncate16(red), truncate16(green), truncate16(blue), a16);
        return o;
    }
}

ricecol_rgba_t ricecol_gtk2_mid(ricecol_rgba_t light, ricecol_rgba_t dark)
{
    uint32_t lr, lg, lb, la, dr, dg, db, da;
    ricecol_to_rgba16(light, &lr, &lg, &lb, &la);
    ricecol_to_rgba16(dark, &dr, &dg, &db, &da);
    (void)da;
    return ricecol_from_rgba16((lr + dr) / 2, (lg + dg) / 2, (lb + db) / 2, la);
}

ricecol_rgba_t ricecol_gtk2_text_aa(ricecol_rgba_t text, ricecol_rgba_t base)
{
    return ricecol_gtk2_mid(text, base);
}
