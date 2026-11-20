/* Unit test for hls: GTK2 HLS shades, exact equality against the golden table.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "harness.h"
#include "rice-colors/hls.h"

static uint32_t ch(double v)
{
    return (uint32_t)(v * 65535.0 + 0.5);
}

static void test_basic(void)
{
    ricecol_rgba_t grey = ricecol_from_rgba16(32768, 32768, 32768, 65535), l, d, m;
    uint32_t r, g, b, a;
    /* a grey has saturation 0: lightness times k, no hue */
    l = ricecol_gtk2_shade(grey, RICECOL_GTK2_LIGHT_FACTOR);
    d = ricecol_gtk2_shade(grey, RICECOL_GTK2_DARK_FACTOR);
    ricecol_to_rgba16(l, &r, &g, &b, &a);
    CHECK(r == g && g == b && r > 32768 && a == 65535);
    ricecol_to_rgba16(d, &r, &g, &b, &a);
    CHECK(r == g && g == b && r < 32768);
    /* k = 0 makes black, k large clamps to white */
    ricecol_to_rgba16(
        ricecol_gtk2_shade(ricecol_from_rgba16(40000, 20000, 10000, 65535), 0.0), &r, &g, &b, &a);
    CHECK(r == 0 && g == 0 && b == 0);
    ricecol_to_rgba16(
        ricecol_gtk2_shade(ricecol_from_rgba16(40000, 20000, 10000, 65535), 100.0), &r, &g, &b, &a);
    CHECK(r == 65535 && g == 65535 && b == 65535);
    /* a negative k clamps lightness and saturation to 0: black */
    ricecol_to_rgba16(
        ricecol_gtk2_shade(ricecol_from_rgba16(40000, 20000, 10000, 65535), -1.0), &r, &g, &b, &a);
    CHECK(r == 0 && g == 0 && b == 0);
    /* mid is the integer average: (1 + 2) / 2 = 1 */
    m = ricecol_gtk2_mid(ricecol_from_rgba16(1, 3, 65535, 65535),
                         ricecol_from_rgba16(2, 4, 65535, 65535));
    ricecol_to_rgba16(m, &r, &g, &b, &a);
    CHECK(r == 1 && g == 3 && b == 65535);
    ricecol_to_rgba16(ricecol_gtk2_text_aa(ricecol_from_rgba16(10, 20, 30, 65535),
                                           ricecol_from_rgba16(11, 21, 31, 65535)),
                      &r,
                      &g,
                      &b,
                      &a);
    CHECK(r == 10 && g == 20 && b == 30);
    (void)ch;
}

static void test_golden(const char *path)
{
    harness_table_t t;
    if (harness_table_open(&t, path)) {
        CHECK(0);
        return;
    }
    while (harness_table_next(&t)) {
        int kind = (int)t.col[0];
        ricecol_rgba_t a =
            ricecol_from_rgba16((uint32_t)t.col[1], (uint32_t)t.col[2], (uint32_t)t.col[3], 65535);
        ricecol_rgba_t b =
            ricecol_from_rgba16((uint32_t)t.col[4], (uint32_t)t.col[5], (uint32_t)t.col[6], 65535);
        ricecol_rgba_t r = kind == 0   ? ricecol_gtk2_shade(a, t.col[7])
                           : kind == 1 ? ricecol_gtk2_mid(a, b)
                                       : ricecol_gtk2_text_aa(a, b);
        uint32_t rr, rg, rb, ra;
        ricecol_to_rgba16(r, &rr, &rg, &rb, &ra);
        /* hls expects exact equality */
        if (rr != (uint32_t)t.col[8] || rg != (uint32_t)t.col[9] || rb != (uint32_t)t.col[10]) {
            fprintf(stderr,
                    "row %ld kind %d: got %u %u %u want %g %g %g\n",
                    t.row,
                    kind,
                    rr,
                    rg,
                    rb,
                    t.col[8],
                    t.col[9],
                    t.col[10]);
            CHECK(0);
        }
    }
    harness_table_close(&t);
}

int main(int argc, char **argv)
{
    test_basic();
    if (argc > 1)
        test_golden(argv[1]);
    return harness_done("hls");
}
