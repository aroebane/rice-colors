/* SPDX-FileCopyrightText: 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Thomas Zander <zander@kde.org>
 * SPDX-FileCopyrightText: 2007 Zack Rusin <zack@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/utils.h"

#include <math.h>

static double mix_double(double a, double b, double bias)
{
    return a + (b - a) * bias;
}

double ricecol_hue(ricecol_rgba_t c)
{
    return ricecol_wrap(ricecol_hcy_from_rgba(c).h, 1.0);
}

double ricecol_chroma(ricecol_rgba_t c)
{
    return ricecol_hcy_from_rgba(c).c;
}

ricecol_rgba_t ricecol_hcy_color(double h, double c, double y, double a)
{
    ricecol_hcy_t v;
    v.h = h;
    v.c = c;
    v.y = y;
    v.a = a;
    return ricecol_hcy_to_rgba(v);
}

static double contrast_for_luma(double y1, double y2)
{
    if (y1 > y2)
        return (y1 + 0.05) / (y2 + 0.05);
    return (y2 + 0.05) / (y1 + 0.05);
}

double ricecol_contrast_ratio(ricecol_rgba_t c1, ricecol_rgba_t c2)
{
    return contrast_for_luma(ricecol_luma(c1), ricecol_luma(c2));
}

ricecol_rgba_t ricecol_lighten(ricecol_rgba_t color, double ky, double kc)
{
    ricecol_hcy_t c = ricecol_hcy_from_rgba(color);
    c.y = 1.0 - ricecol_normalize((1.0 - c.y) * (1.0 - ky));
    c.c = 1.0 - ricecol_normalize((1.0 - c.c) * kc);
    return ricecol_hcy_to_rgba(c);
}

ricecol_rgba_t ricecol_darken(ricecol_rgba_t color, double ky, double kc)
{
    ricecol_hcy_t c = ricecol_hcy_from_rgba(color);
    c.y = ricecol_normalize(c.y * (1.0 - ky));
    c.c = ricecol_normalize(c.c * kc);
    return ricecol_hcy_to_rgba(c);
}

ricecol_rgba_t ricecol_shade(ricecol_rgba_t color, double ky, double kc)
{
    ricecol_hcy_t c = ricecol_hcy_from_rgba(color);
    c.y = ricecol_normalize(c.y + ky);
    c.c = ricecol_normalize(c.c + kc);
    return ricecol_hcy_to_rgba(c);
}

ricecol_rgba_t ricecol_mix(ricecol_rgba_t c1, ricecol_rgba_t c2, double bias)
{
    double a, r, g, b;
    ricecol_rgba_t o;

    if (bias <= 0.0)
        return c1;
    if (bias >= 1.0)
        return c2;
    /* UPSTREAM-QUIRK: KDE tests NaN last, so it only works because every NaN
     * comparison above is false. Same order kept, same result (c1). */
    if (isnan(bias))
        return c1;

    c1 = ricecol_q16(c1);
    c2 = ricecol_q16(c2);
    a = mix_double(c1.a, c2.a, bias);
    if (a <= 0.0) {
        o.r = o.g = o.b = o.a = 0.0; /* Qt::transparent */
        return o;
    }
    r = ricecol_normalize(mix_double(c1.r * c1.a, c2.r * c2.a, bias));
    g = ricecol_normalize(mix_double(c1.g * c1.a, c2.g * c2.a, bias));
    b = ricecol_normalize(mix_double(c1.b * c1.a, c2.b * c2.a, bias));
    o.r = r / a;
    o.g = g / a;
    o.b = b / a;
    o.a = a;
    return ricecol_q16(o);
}

static ricecol_rgba_t
tint_result(ricecol_rgba_t base, double base_luma, ricecol_rgba_t color, double amount)
{
    ricecol_hcy_t result = ricecol_hcy_from_rgba(ricecol_mix(base, color, pow(amount, 0.3)));
    result.y = mix_double(base_luma, result.y, amount);
    return ricecol_hcy_to_rgba(result);
}

static double tint_luma(ricecol_rgba_t base, double base_luma, ricecol_rgba_t color, double amount)
{
    double result = ricecol_luma(ricecol_mix(base, color, pow(amount, 0.3)));
    return mix_double(base_luma, result, amount);
}

ricecol_rgba_t ricecol_tint(ricecol_rgba_t base, ricecol_rgba_t color, double amount)
{
    double base_luma, ri, rg, u, l, a;
    int i;

    if (amount <= 0.0)
        return base;
    if (amount >= 1.0)
        return color;
    if (isnan(amount))
        return base;

    base_luma = ricecol_luma(base);
    ri = contrast_for_luma(base_luma, ricecol_luma(color));
    /* KDE's constants: amount^3 here and pow(a, 0.3) in tint_luma. KDE documents
     * no derivation for them. */
    rg = 1.0 + ((ri + 1.0) * amount * amount * amount);
    u = 1.0;
    l = 0.0;
    a = 0.5;
    for (i = 12; i; --i) {
        double result_luma, ra;
        a = 0.5 * (l + u);
        result_luma = tint_luma(base, base_luma, color, a);
        ra = contrast_for_luma(base_luma, result_luma);
        if (ra > rg)
            u = a;
        else
            l = a;
    }
    /* UPSTREAM-QUIRK: KDE uses the last probe `a`, not the interval midpoint
     * left after the final step, so the result is off by half a step (1/8192).
     * Kept to match KDE; tests/golden/utils.tsv enforces it. */
    return tint_result(base, base_luma, color, a);
}
