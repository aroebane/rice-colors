/* Unit test for hcy: analytic checks; golden tables are added with the oracle.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include <math.h>

#include "harness.h"
#include "rice-colors/hcy.h"

static void test_gamma(void)
{
    CHECK(ricecol_gamma(0.0) == 0.0);
    CHECK(ricecol_gamma(1.0) == 1.0);
    CHECK(fabs(ricecol_igamma(ricecol_gamma(0.37)) - 0.37) < 1e-12);
    CHECK(ricecol_gamma(2.0) == 1.0); /* normalized first */
    CHECK(ricecol_gamma(-1.0) == 0.0);
}

static void test_luma(void)
{
    ricecol_rgba_t k = ricecol_from_rgb8(0x000000), w = ricecol_from_rgb8(0xffffff);
    CHECK(ricecol_luma(k) == 0.0);
    CHECK(fabs(ricecol_luma(w) - 1.0) < 1e-12);
    CHECK(ricecol_luma(ricecol_from_rgb8(0x00ff00)) > ricecol_luma(ricecol_from_rgb8(0xff0000)));
}

static void test_roundtrip(void)
{
    uint32_t v;
    for (v = 0; v < 0x1000000UL; v += 9973) {
        ricecol_rgba_t c = ricecol_from_rgb8(v);
        ricecol_rgba_t b = ricecol_hcy_to_rgba(ricecol_hcy_from_rgba(c));
        CHECK_COLOR(c, b, 2);
    }
}

static void test_grey_and_hue(void)
{
    ricecol_hcy_t h = ricecol_hcy_from_rgba(ricecol_from_rgb8(0x808080));
    CHECK(h.c == 0.0 && h.h == 0.0);
    h = ricecol_hcy_from_rgba(ricecol_from_rgb8(0xff0000));
    CHECK(h.h == 0.0 && h.c > 0.99);
    h = ricecol_hcy_from_rgba(ricecol_from_rgb8(0xff00ff));
    CHECK(h.h < 0.0); /* not wrapped, like KHCY */
    h = ricecol_hcy_from_rgba(ricecol_from_rgb8(0x0000ff));
    CHECK(fabs(h.h - 2.0 / 3.0) < 1e-12);
    h = ricecol_hcy_from_rgba(ricecol_from_rgb8(0x00ff00));
    CHECK(fabs(h.h - 1.0 / 3.0) < 1e-12);
}

static void test_sectors(void)
{
    int i;
    for (i = 0; i < 12; i++) {
        ricecol_hcy_t h;
        ricecol_rgba_t c, b;
        h.h = (i + 0.5) / 12.0;
        h.c = 0.6;
        h.y = 0.5;
        h.a = 1.0;
        c = ricecol_hcy_to_rgba(h);
        b = ricecol_hcy_to_rgba(ricecol_hcy_from_rgba(c));
        CHECK_COLOR(c, b, 2);
    }
    /* hue wraps; dark and light y hit both tm >= y and tm < y branches */
    {
        ricecol_hcy_t lo, hi;
        lo.h = -0.25;
        lo.c = 0.5;
        lo.y = 0.05;
        lo.a = 1.0;
        hi = lo;
        hi.h = 0.75;
        hi.y = 0.95;
        CHECK_COLOR(ricecol_hcy_to_rgba(lo), ricecol_hcy_to_rgba((lo.h = 0.75, lo)), 0);
        CHECK(ricecol_hcy_to_rgba(hi).a == 1.0);
    }
}

static void test_golden(const char *path)
{
    harness_table_t t;
    if (harness_table_open(&t, path)) {
        CHECK(0);
        return;
    }
    while (harness_table_next(&t)) {
        ricecol_rgba_t c = harness_col_rgba16(&t, 0);
        ricecol_hcy_t h = ricecol_hcy_from_rgba(c);
        ricecol_hcy_t k;
        CHECK(fabs(ricecol_luma(c) - t.col[4]) < 1e-6);
        CHECK(fabs(ricecol_wrap(h.h, 1.0) - t.col[5]) < 1e-6);
        CHECK(fabs(h.c - t.col[6]) < 1e-6);
        CHECK(fabs(h.y - t.col[7]) < 1e-6);
        k.h = t.col[5];
        k.c = t.col[6];
        k.y = t.col[7];
        k.a = c.a;
        CHECK_COLOR(ricecol_hcy_to_rgba(k), harness_col_rgba16(&t, 8), 1);
    }
    harness_table_close(&t);
}

int main(int argc, char **argv)
{
    if (argc > 1)
        test_golden(argv[1]);
    test_gamma();
    test_luma();
    test_roundtrip();
    test_grey_and_hue();
    test_sectors();
    return harness_done("hcy");
}
