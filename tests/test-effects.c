/* Unit test for effects: every kind alone, the KDE defaults, enabled = 0, an
 * unknown code, then the golden table.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "harness.h"
#include "rice-colors/effects.h"

static ricecol_rgba_t rgb(uint32_t v)
{
    return ricecol_from_rgb8(v);
}

static ricecol_effect_t none(void)
{
    ricecol_effect_t e = RICECOL_EFFECT_KDE_INACTIVE;
    e.enabled = 1;
    e.intensity = 0;
    e.color = 0;
    e.contrast = 0;
    return e;
}

static void test_kinds(void)
{
    ricecol_rgba_t c = rgb(0x4080c0), bg = rgb(0x202020), same;
    ricecol_effect_t e = none();

    same = ricecol_effect_background(c, &e);
    CHECK_COLOR(same, c, 0);
    CHECK_COLOR(ricecol_effect_foreground(c, bg, &e), c, 0);

    e.intensity = RICECOL_INTENSITY_SHADE;
    e.intensity_amount = 0.2;
    CHECK(ricecol_luma(ricecol_effect_background(c, &e)) > ricecol_luma(c));
    e.intensity = RICECOL_INTENSITY_DARKEN;
    e.intensity_amount = 0.2;
    CHECK(ricecol_luma(ricecol_effect_background(c, &e)) < ricecol_luma(c));
    e.intensity = RICECOL_INTENSITY_LIGHTEN;
    e.intensity_amount = 0.2;
    CHECK(ricecol_luma(ricecol_effect_background(c, &e)) > ricecol_luma(c));
    e.intensity = 99; /* unknown: behaves as NONE */
    CHECK_COLOR(ricecol_effect_background(c, &e), c, 0);
    e.intensity = 0;

    e.color = RICECOL_COLOR_DESATURATE;
    e.color_amount = 1.0;
    CHECK(ricecol_hcy_from_rgba(ricecol_effect_background(c, &e)).c == 0.0);
    e.color = RICECOL_COLOR_FADE;
    e.color_amount = 1.0;
    e.effect_color = rgb(0xff0000);
    CHECK_COLOR(ricecol_effect_background(c, &e), rgb(0xff0000), 0);
    e.color = RICECOL_COLOR_TINT;
    e.color_amount = 1.0;
    CHECK_COLOR(ricecol_effect_background(c, &e), rgb(0xff0000), 0);
    e.color = -4;
    CHECK_COLOR(ricecol_effect_background(c, &e), c, 0);
    e.color = 0;

    e.contrast = RICECOL_CONTRAST_FADE;
    e.contrast_amount = 1.0;
    CHECK_COLOR(ricecol_effect_foreground(c, bg, &e), bg, 0);
    e.contrast = RICECOL_CONTRAST_TINT;
    CHECK_COLOR(ricecol_effect_foreground(c, bg, &e), bg, 0);
    e.contrast = 7;
    CHECK_COLOR(ricecol_effect_foreground(c, bg, &e), c, 0);

    e.enabled = 0;
    e.intensity = RICECOL_INTENSITY_SHADE;
    e.intensity_amount = 0.5;
    CHECK_COLOR(ricecol_effect_background(c, &e), c, 0);
    CHECK_COLOR(ricecol_effect_foreground(c, bg, &e), c, 0);
    CHECK_COLOR(ricecol_effect_background(c, NULL), c, 0);
    CHECK_COLOR(ricecol_effect_foreground(c, bg, NULL), c, 0);
}

static void test_defaults(void)
{
    CHECK(RICECOL_EFFECT_KDE_DISABLED.enabled == 1);
    CHECK(RICECOL_EFFECT_KDE_DISABLED.intensity == RICECOL_INTENSITY_DARKEN);
    CHECK(RICECOL_EFFECT_KDE_DISABLED.contrast == RICECOL_CONTRAST_FADE);
    CHECK(RICECOL_EFFECT_KDE_INACTIVE.enabled == 0);
    CHECK(RICECOL_EFFECT_KDE_INACTIVE.color == RICECOL_COLOR_DESATURATE);
    CHECK(RICECOL_EFFECT_KDE_INACTIVE.contrast == RICECOL_CONTRAST_TINT);
}

static void test_golden(const char *path)
{
    harness_table_t t;
    if (harness_table_open(&t, path)) {
        CHECK(0);
        return;
    }
    while (harness_table_next(&t)) {
        ricecol_effect_t e;
        ricecol_rgba_t bg = harness_col_rgba16(&t, 11), fg = harness_col_rgba16(&t, 15);
        ricecol_rgba_t rb, rf;
        int kind = (int)t.col[0];
        if (kind == 1)
            e = RICECOL_EFFECT_KDE_DISABLED;
        else if (kind == 2)
            e = RICECOL_EFFECT_KDE_INACTIVE;
        else {
            e.enabled = (int)t.col[1];
            e.intensity = (int)t.col[2];
            e.intensity_amount = t.col[3];
            e.color = (int)t.col[4];
            e.color_amount = t.col[5];
            e.effect_color =
                rgb(((uint32_t)t.col[6] << 16) | ((uint32_t)t.col[7] << 8) | (uint32_t)t.col[8]);
            e.contrast = (int)t.col[9];
            e.contrast_amount = t.col[10];
        }
        rb = ricecol_effect_background(bg, &e);
        rf = ricecol_effect_foreground(fg, bg, &e);
        if (harness_diff16(rb, harness_col_rgba16(&t, 19)) > 1 ||
            harness_diff16(rf, harness_col_rgba16(&t, 23)) > 1) {
            fprintf(stderr, "row %ld kind %d\n", t.row, kind);
            CHECK(0);
        }
    }
    harness_table_close(&t);
}

int main(int argc, char **argv)
{
    test_kinds();
    test_defaults();
    if (argc > 1)
        test_golden(argv[1]);
    return harness_done("effects");
}
