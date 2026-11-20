/* SPDX-FileCopyrightText: 2006 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/shade.h"

double ricecol_contrast_from_kde(int n)
{
    return 0.1 * n;
}

double ricecol_frame_contrast_clamp(double v)
{
    /* UPSTREAM-QUIRK: std::clamp(NaN, 0, 1) returns NaN, so a NaN frameContrast
     * survives. Same here; ricecol_shade_role turns a NaN contrast into 1.0. */
    if (v < 0.0)
        return 0.0;
    if (v > 1.0)
        return 1.0;
    return v;
}

ricecol_rgba_t ricecol_shade_role(ricecol_rgba_t color,
                                  ricecol_shade_role_t role,
                                  double contrast,
                                  double chroma_adjust)
{
    double y, yi, light_amount, dark_amount;

    if (role < RICECOL_SHADE_LIGHT || role > RICECOL_SHADE_SHADOW)
        return color;

    /* NaN gives 1.0 because both comparisons are false. */
    contrast = (1.0 > contrast ? (-1.0 < contrast ? contrast : -1.0) : 1.0);
    y = ricecol_luma(color);
    yi = 1.0 - y;

    /* very dark colors: shadow and midlight use the default row */
    if (y < 0.006) {
        switch (role) {
        case RICECOL_SHADE_LIGHT:
            return ricecol_shade(color, 0.05 + 0.95 * contrast, chroma_adjust);
        case RICECOL_SHADE_MID:
            return ricecol_shade(color, 0.01 + 0.20 * contrast, chroma_adjust);
        case RICECOL_SHADE_DARK:
            return ricecol_shade(color, 0.02 + 0.40 * contrast, chroma_adjust);
        default:
            return ricecol_shade(color, 0.03 + 0.60 * contrast, chroma_adjust);
        }
    }

    /* very light colors: light and mid use the default row */
    if (y > 0.93) {
        switch (role) {
        case RICECOL_SHADE_MIDLIGHT:
            return ricecol_shade(color, -0.02 - 0.20 * contrast, chroma_adjust);
        case RICECOL_SHADE_DARK:
            return ricecol_shade(color, -0.06 - 0.60 * contrast, chroma_adjust);
        case RICECOL_SHADE_SHADOW:
            return ricecol_shade(color, -0.10 - 0.90 * contrast, chroma_adjust);
        default:
            return ricecol_shade(color, -0.04 - 0.40 * contrast, chroma_adjust);
        }
    }

    light_amount = (0.05 + y * 0.55) * (0.25 + contrast * 0.75);
    dark_amount = (-y) * (0.55 + contrast * 0.35);
    switch (role) {
    case RICECOL_SHADE_LIGHT:
        return ricecol_shade(color, light_amount, chroma_adjust);
    case RICECOL_SHADE_MIDLIGHT:
        return ricecol_shade(color, (0.15 + 0.35 * yi) * light_amount, chroma_adjust);
    case RICECOL_SHADE_MID:
        return ricecol_shade(color, (0.35 + 0.15 * y) * dark_amount, chroma_adjust);
    case RICECOL_SHADE_DARK:
        return ricecol_shade(color, dark_amount, chroma_adjust);
    default:
        /* UPSTREAM-QUIRK: the shadow role darkens by 0.5 + 0.3y; for y near 0.93
         * that is 0.78, not obviously intended, but it is KDE's value. */
        return ricecol_darken(
            ricecol_shade(color, dark_amount, chroma_adjust), 0.5 + 0.3 * y, RICECOL_DARKEN_CHROMA);
    }
}
