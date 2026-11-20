/* rice-colors utils (layer 2): contrast, lighten, darken, shade, mix, tint.
 * Port of kcolorutils.cpp.
 *
 * SPDX-FileCopyrightText: 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Thomas Zander <zander@kde.org>
 * SPDX-FileCopyrightText: 2007 Zack Rusin <zack@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_UTILS_H
#define RICECOL_UTILS_H

#include "rice-colors/hcy.h"

/* KDE's default arguments. C has none, so the caller passes these. */
#define RICECOL_MIX_BIAS 0.5
#define RICECOL_TINT_AMOUNT 0.3
#define RICECOL_LIGHTEN_CHROMA 1.0
#define RICECOL_DARKEN_CHROMA 1.0
#define RICECOL_SHADE_CHROMA 0.0

RICECOL_BEGIN_DECLS

/* Hue wrapped into 0..1 and chroma, as KColorUtils::hue and ::chroma. */
RICECOL_API double ricecol_hue(ricecol_rgba_t c);
RICECOL_API double ricecol_chroma(ricecol_rgba_t c);
/* KColorUtils::hcyColor. */
RICECOL_API ricecol_rgba_t ricecol_hcy_color(double h, double c, double y, double a);

RICECOL_API double ricecol_contrast_ratio(ricecol_rgba_t c1, ricecol_rgba_t c2);

/* ky and kc as in KColorUtils; see the RICECOL_*_CHROMA defaults. */
RICECOL_API ricecol_rgba_t ricecol_lighten(ricecol_rgba_t c, double ky, double kc);
RICECOL_API ricecol_rgba_t ricecol_darken(ricecol_rgba_t c, double ky, double kc);
RICECOL_API ricecol_rgba_t ricecol_shade(ricecol_rgba_t c, double ky, double kc);

/* bias <= 0 or NaN gives c1; bias >= 1 gives c2. Alpha is blended; colors are
 * blended premultiplied. */
RICECOL_API ricecol_rgba_t ricecol_mix(ricecol_rgba_t c1, ricecol_rgba_t c2, double bias);
/* amount <= 0 or NaN gives base; amount >= 1 gives color. */
RICECOL_API ricecol_rgba_t ricecol_tint(ricecol_rgba_t base, ricecol_rgba_t color, double amount);

RICECOL_END_DECLS

#endif
