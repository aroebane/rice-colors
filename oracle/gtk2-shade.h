/* Local stand-ins for the GTK2 types used by gtk2-shade.c.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef ORACLE_GTK2_SHADE_H
#define ORACLE_GTK2_SHADE_H

typedef unsigned short guint16;
typedef double gdouble;
typedef struct {
    guint16 red, green, blue;
} GdkColor;

void _gtk_style_shade(const GdkColor *a, GdkColor *b, gdouble k);

#endif
