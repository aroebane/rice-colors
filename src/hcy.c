/* SPDX-FileCopyrightText: 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/hcy.h"

#include <math.h>

/* UPSTREAM-QUIRK: KDE pairs Rec. 709 weights with a plain 2.2 power curve, not
 * the sRGB transfer function, so luma differs slightly from the standard
 * definition. Kept as is: the goal is to match KDE, not WCAG. */
/* UPSTREAM-QUIRK: on Qt 6, QColor::redF() returns float, so KDE feeds float-
 * precision channels into this math (Qt 5 gave double). Results differ from
 * this double math by about 1e-7 relative. We keep double; the golden
 * tests allow 1e-6 absolute on these values and 1 unit on 16-bit colors. */
/* Rec. 709 weights (HCY_REC 709 in kcolorspaces.cpp). */
static const double yc[3] = {0.2126, 0.7152, 0.0722};

double ricecol_gamma(double n)
{
    return pow(ricecol_normalize(n), 2.2);
}

double ricecol_igamma(double n)
{
    return pow(ricecol_normalize(n), 1.0 / 2.2);
}

static double lumag(double r, double g, double b)
{
    return r * yc[0] + g * yc[1] + b * yc[2];
}

double ricecol_luma(ricecol_rgba_t c)
{
    c = ricecol_q16(c);
    return lumag(ricecol_gamma(c.r), ricecol_gamma(c.g), ricecol_gamma(c.b));
}

ricecol_hcy_t ricecol_hcy_from_rgba(ricecol_rgba_t color)
{
    ricecol_hcy_t o;
    double r, g, b, p, n, d;

    color = ricecol_q16(color);
    r = ricecol_gamma(color.r);
    g = ricecol_gamma(color.g);
    b = ricecol_gamma(color.b);
    o.a = color.a;

    o.y = lumag(r, g, b);

    p = fmax(fmax(r, g), b);
    n = fmin(fmin(r, g), b);
    d = 6.0 * (p - n);
    /* UPSTREAM-QUIRK: the hue can come out negative here (r == p with b > g);
     * KHCY does not wrap it. Callers must wrap (ricecol_hue does). */
    if (n == p)
        o.h = 0.0;
    else if (r == p)
        o.h = (g - b) / d;
    else if (g == p)
        o.h = (b - r) / d + (1.0 / 3.0);
    else
        o.h = (r - g) / d + (2.0 / 3.0);

    if (r == g && g == b)
        o.c = 0.0;
    else
        /* UPSTREAM-QUIRK: y is never 0 or 1 here because r == g == b is handled
         * above, but a non-grey color with a near-zero y after q16 rounding
         * could still divide by a tiny number. KDE has the same code. */
        o.c = fmax((o.y - n) / o.y, (p - o.y) / (1 - o.y));

    return o;
}

ricecol_rgba_t ricecol_hcy_to_rgba(ricecol_hcy_t hcy)
{
    double h = ricecol_wrap(hcy.h, 1.0);
    double c = ricecol_normalize(hcy.c);
    double y = ricecol_normalize(hcy.y);
    double hs = h * 6.0;
    double th, tm, tn, to, tp;
    ricecol_rgba_t o;

    if (hs < 1.0) {
        th = hs;
        tm = yc[0] + yc[1] * th;
    } else if (hs < 2.0) {
        th = 2.0 - hs;
        tm = yc[1] + yc[0] * th;
    } else if (hs < 3.0) {
        th = hs - 2.0;
        tm = yc[1] + yc[2] * th;
    } else if (hs < 4.0) {
        th = 4.0 - hs;
        tm = yc[2] + yc[1] * th;
    } else if (hs < 5.0) {
        th = hs - 4.0;
        tm = yc[2] + yc[0] * th;
    } else {
        th = 6.0 - hs;
        tm = yc[0] + yc[2] * th;
    }

    if (tm >= y) {
        tp = y + y * c * (1.0 - tm) / tm;
        to = y + y * c * (th - tm) / tm;
        tn = y - (y * c);
    } else {
        tp = y + (1.0 - y) * c;
        to = y + (1.0 - y) * c * (th - tm) / (1.0 - tm);
        tn = y - (1.0 - y) * c * tm / (1.0 - tm);
    }

    tp = ricecol_igamma(tp);
    to = ricecol_igamma(to);
    tn = ricecol_igamma(tn);
    if (hs < 1.0) {
        o.r = tp;
        o.g = to;
        o.b = tn;
    } else if (hs < 2.0) {
        o.r = to;
        o.g = tp;
        o.b = tn;
    } else if (hs < 3.0) {
        o.r = tn;
        o.g = tp;
        o.b = to;
    } else if (hs < 4.0) {
        o.r = tn;
        o.g = to;
        o.b = tp;
    } else if (hs < 5.0) {
        o.r = to;
        o.g = tn;
        o.b = tp;
    } else {
        o.r = tp;
        o.g = tn;
        o.b = to;
    }
    o.a = hcy.a;
    return ricecol_q16(o);
}
