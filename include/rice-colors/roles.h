/* rice-colors roles (layer 3): the calculated KColorScheme backgrounds.
 *
 * SPDX-FileCopyrightText: 2006 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#ifndef RICECOL_ROLES_H
#define RICECOL_ROLES_H

#include "rice-colors/effects.h"

RICECOL_BEGIN_DECLS

/* KDE derives six backgrounds as tint(NormalBackground, <text color>, 0.3):
 *   ActiveBackground   from ActiveText
 *   LinkBackground     from LinkText
 *   VisitedBackground  from VisitedText
 *   NegativeBackground from NegativeText
 *   NeutralBackground  from NeutralText
 *   PositiveBackground from PositiveText
 * Pass the (already state-effected) normal background and text color. */
RICECOL_API ricecol_rgba_t ricecol_calculated_background(ricecol_rgba_t normal_bg,
                                                         ricecol_rgba_t fg);

RICECOL_END_DECLS

#endif
