/* rice-colors shade (layer 3): the five 3D shade roles. Port of KColorScheme::shade.
 *
 * SPDX-FileCopyrightText: 2006 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_SHADE_H
#define RICECOL_SHADE_H

#include "rice-colors/utils.h"

#define RICECOL_KDE_CONTRAST 0.7
#define RICECOL_KDE_FRAME_CONTRAST 0.2

/* KDE's enum order. */
typedef enum {
    RICECOL_SHADE_LIGHT,
    RICECOL_SHADE_MIDLIGHT,
    RICECOL_SHADE_MID,
    RICECOL_SHADE_DARK,
    RICECOL_SHADE_SHADOW
} ricecol_shade_role_t;

RICECOL_BEGIN_DECLS

/* contrast is clamped to -1..1 (NaN gives 1.0). An out-of-range role returns
 * the input color unchanged. chroma_adjust: RICECOL_SHADE_CHROMA is KDE's default. */
RICECOL_API ricecol_rgba_t ricecol_shade_role(ricecol_rgba_t c,
                                              ricecol_shade_role_t role,
                                              double contrast,
                                              double chroma_adjust);

/* The "contrast" setting of kdeglobals is an integer; KDE uses 0.1 * n, unclamped. */
RICECOL_API double ricecol_contrast_from_kde(int n);
/* KDE's frameContrast: clamp to 0..1. NaN passes through, as std::clamp does. */
RICECOL_API double ricecol_frame_contrast_clamp(double v);

RICECOL_END_DECLS

#endif
