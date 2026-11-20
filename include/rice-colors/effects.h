/* rice-colors effects (layer 3): the Disabled and Inactive state effects.
 * Port of StateEffects in kcolorscheme.cpp.
 *
 * SPDX-FileCopyrightText: 2006 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-FileCopyrightText: 2007 Olaf Schmidt <ojschmidt@kde.org>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_EFFECTS_H
#define RICECOL_EFFECTS_H

#include "rice-colors/shade.h"

/* Codes are KDE's config values. The struct fields are int so C++98 can fill
 * the struct with a brace list; an unknown code behaves as NONE, as KDE's switch. */
typedef enum {
    RICECOL_INTENSITY_NONE = 0,
    RICECOL_INTENSITY_SHADE = 1,
    RICECOL_INTENSITY_DARKEN = 2,
    RICECOL_INTENSITY_LIGHTEN = 3
} ricecol_intensity_t;
typedef enum {
    RICECOL_COLOR_NONE = 0,
    RICECOL_COLOR_DESATURATE = 1,
    RICECOL_COLOR_FADE = 2,
    RICECOL_COLOR_TINT = 3
} ricecol_color_effect_t;
typedef enum {
    RICECOL_CONTRAST_NONE = 0,
    RICECOL_CONTRAST_FADE = 1,
    RICECOL_CONTRAST_TINT = 2
} ricecol_contrast_effect_t;

typedef struct {
    int enabled;   /* 0: every effect is off */
    int intensity; /* ricecol_intensity_t */
    double intensity_amount;
    int color; /* ricecol_color_effect_t */
    double color_amount;
    ricecol_rgba_t effect_color; /* used by the FADE and TINT color effects */
    int contrast;                /* ricecol_contrast_effect_t */
    double contrast_amount;
} ricecol_effect_t;

RICECOL_BEGIN_DECLS

/* KDE's defaults for the two states. Inactive has enabled = 0, as in KDE. */
extern RICECOL_API const ricecol_effect_t RICECOL_EFFECT_KDE_DISABLED;
extern RICECOL_API const ricecol_effect_t RICECOL_EFFECT_KDE_INACTIVE;

/* Intensity first, then color. */
RICECOL_API ricecol_rgba_t ricecol_effect_background(ricecol_rgba_t color,
                                                     const ricecol_effect_t *e);
/* Contrast against normal_bg first, then the background effects. */
RICECOL_API ricecol_rgba_t ricecol_effect_foreground(ricecol_rgba_t fg,
                                                     ricecol_rgba_t normal_bg,
                                                     const ricecol_effect_t *e);

RICECOL_END_DECLS

#endif
