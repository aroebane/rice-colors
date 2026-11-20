/* rice-colors hcy (layer 1): gamma, luma, and RGB <-> HCY. Port of kcolorspaces.cpp.
 *
 * SPDX-FileCopyrightText: 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_HCY_H
#define RICECOL_HCY_H

#include "rice-colors/core.h"

RICECOL_BEGIN_DECLS

/* h in turns (0..1, may be negative straight from rgba: wrap it), c and y in 0..1. */
typedef struct {
    double h, c, y, a;
} ricecol_hcy_t;

/* pow(normalize(n), 2.2) and pow(normalize(n), 1 / 2.2). */
RICECOL_API double ricecol_gamma(double n);
RICECOL_API double ricecol_igamma(double n);

/* Rec. 709 luma of the gamma-expanded channels. The color is rounded to 16 bits first. */
RICECOL_API double ricecol_luma(ricecol_rgba_t c);

/* Like KHCY(QColor): the hue is not wrapped and can be negative. */
RICECOL_API ricecol_hcy_t ricecol_hcy_from_rgba(ricecol_rgba_t c);
/* Like KHCY::qColor(): the result is rounded to 16 bits. */
RICECOL_API ricecol_rgba_t ricecol_hcy_to_rgba(ricecol_hcy_t hcy);

RICECOL_END_DECLS

#endif
