/* SPDX-FileCopyrightText: 2006 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/effects.h"

#define RGB8(r, g, b) {(r) / 255.0, (g) / 255.0, (b) / 255.0, 1.0}

/* UPSTREAM-QUIRK: KDE reads Color only when the color effect is not NONE, so
 * for KDE_DISABLED (color effect NONE) the color below is never used. */
const ricecol_effect_t RICECOL_EFFECT_KDE_DISABLED = {1,
                                                      RICECOL_INTENSITY_DARKEN,
                                                      0.10,
                                                      RICECOL_COLOR_NONE,
                                                      0.0,
                                                      RGB8(56, 56, 56),
                                                      RICECOL_CONTRAST_FADE,
                                                      0.65};
const ricecol_effect_t RICECOL_EFFECT_KDE_INACTIVE = {0,
                                                      RICECOL_INTENSITY_NONE,
                                                      0.0,
                                                      RICECOL_COLOR_DESATURATE,
                                                      -0.9,
                                                      RGB8(112, 111, 110),
                                                      RICECOL_CONTRAST_TINT,
                                                      0.25};

ricecol_rgba_t ricecol_effect_background(ricecol_rgba_t color, const ricecol_effect_t *e)
{
    if (!e || !e->enabled)
        return color;
    switch (e->intensity) {
    case RICECOL_INTENSITY_SHADE:
        color = ricecol_shade(color, e->intensity_amount, RICECOL_SHADE_CHROMA);
        break;
    case RICECOL_INTENSITY_DARKEN:
        color = ricecol_darken(color, e->intensity_amount, RICECOL_DARKEN_CHROMA);
        break;
    case RICECOL_INTENSITY_LIGHTEN:
        color = ricecol_lighten(color, e->intensity_amount, RICECOL_LIGHTEN_CHROMA);
        break;
    default:
        break;
    }
    switch (e->color) {
    case RICECOL_COLOR_DESATURATE:
        color = ricecol_darken(color, 0.0, 1.0 - e->color_amount);
        break;
    case RICECOL_COLOR_FADE:
        color = ricecol_mix(color, e->effect_color, e->color_amount);
        break;
    case RICECOL_COLOR_TINT:
        color = ricecol_tint(color, e->effect_color, e->color_amount);
        break;
    default:
        break;
    }
    return color;
}

ricecol_rgba_t
ricecol_effect_foreground(ricecol_rgba_t fg, ricecol_rgba_t normal_bg, const ricecol_effect_t *e)
{
    if (!e || !e->enabled)
        return fg;
    switch (e->contrast) {
    case RICECOL_CONTRAST_FADE:
        fg = ricecol_mix(fg, normal_bg, e->contrast_amount);
        break;
    case RICECOL_CONTRAST_TINT:
        fg = ricecol_tint(fg, normal_bg, e->contrast_amount);
        break;
    default:
        break;
    }
    return ricecol_effect_background(fg, e);
}
