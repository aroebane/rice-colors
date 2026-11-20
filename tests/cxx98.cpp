// Compile test: the umbrella header must build as C++98 (G6). Nothing is printed.
// SPDX-License-Identifier: GPL-3.0-or-later
#include "rice-colors/rice-colors.h"

int main()
{
    ricecol_rgba_t c = ricecol_from_rgb8(0x3daee9UL);
    ricecol_rgba_t d = ricecol_from_rgb8(0x31363bUL);
    ricecol_effect_t e = {
        1, RICECOL_INTENSITY_SHADE, 0.1, RICECOL_COLOR_NONE, 0.0, c, RICECOL_CONTRAST_NONE, 0.0};
    double sum = 0.0;

    sum += ricecol_luma(c);
    sum += ricecol_contrast_ratio(c, d);
    sum += ricecol_tint(c, d, RICECOL_TINT_AMOUNT).r;
    sum += ricecol_shade_role(c, RICECOL_SHADE_LIGHT, 0.0, RICECOL_SHADE_CHROMA).r;
    sum += ricecol_effect_background(c, &e).r;
    sum += ricecol_effect_foreground(c, d, &RICECOL_EFFECT_KDE_DISABLED).r;
    sum += ricecol_calculated_background(d, c).r;
    sum += ricecol_gtk2_shade(c, RICECOL_GTK2_LIGHT_FACTOR).r;
    return sum < 0.0; /* keeps the calls alive; never true */
}
