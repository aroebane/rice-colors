/* SPDX-FileCopyrightText: 2006 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * SPDX-License-Identifier: GPL-3.0-or-later (ported from LGPL-2.0-or-later)
 */
#include "rice-colors/roles.h"

ricecol_rgba_t ricecol_calculated_background(ricecol_rgba_t normal_bg, ricecol_rgba_t fg)
{
    return ricecol_tint(normal_bg, fg, RICECOL_TINT_AMOUNT);
}
