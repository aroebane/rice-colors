/* rice-colors hls (layer 1): GTK2's light and dark shades. Port of
 * _gtk_style_shade in GTK+ 2.24 gtk/gtkstyle.c.
 *
 * SPDX-FileCopyrightText: 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_HLS_H
#define RICECOL_HLS_H

#include "rice-colors/core.h"

/* GTK2's LIGHTNESS_MULT and DARKNESS_MULT. */
#define RICECOL_GTK2_LIGHT_FACTOR 1.3
#define RICECOL_GTK2_DARK_FACTOR 0.7

RICECOL_BEGIN_DECLS

/* RGB to HLS, multiply lightness and saturation by k (each clamped to 0..1),
 * back to RGB, channels truncated to 16 bits like GTK2's guint16. Alpha is kept. */
RICECOL_API ricecol_rgba_t ricecol_gtk2_shade(ricecol_rgba_t bg, double k);

/* GTK2's style->mid and style->text_aa: integer average of the 16-bit channels
 * (the sum is halved with integer division). Alpha is taken from a. */
RICECOL_API ricecol_rgba_t ricecol_gtk2_mid(ricecol_rgba_t light, ricecol_rgba_t dark);
RICECOL_API ricecol_rgba_t ricecol_gtk2_text_aa(ricecol_rgba_t text, ricecol_rgba_t base);

RICECOL_END_DECLS

#endif
