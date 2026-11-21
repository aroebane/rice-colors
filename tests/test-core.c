/* Unit test for core: no golden table.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include <math.h>
#include <string.h>

#include "harness.h"

static void test_8bit(void)
{
    uint32_t v;
    for (v = 0; v < 256; v++) {
        uint32_t rgb = (v << 16) | ((255 - v) << 8) | ((v * 7) & 0xff);
        ricecol_rgba_t c = ricecol_from_rgb8(rgb);
        CHECK(ricecol_to_argb8(c) == (0xff000000UL | rgb));
        c = ricecol_from_argb8((v << 24) | rgb);
        CHECK(ricecol_to_argb8(c) == ((v << 24) | rgb));
    }
}

static void test_16bit(void)
{
    uint32_t v;
    for (v = 0; v < 65536; v += 17) {
        uint32_t r, g, b, a;
        ricecol_rgba_t c = ricecol_from_rgba16(v, 65535 - v, (v * 3) & 0xffff, v);
        ricecol_to_rgba16(c, &r, &g, &b, &a);
        CHECK(r == v && g == 65535 - v && b == ((v * 3) & 0xffff) && a == v);
    }
    CHECK(ricecol_from_rgba16(70000, 0, 0, 0).r == 1.0);
    ricecol_to_rgba16(ricecol_from_rgb8(0x123456), NULL, NULL, NULL, NULL);
}

static void test_rounding(void)
{
    ricecol_rgba_t c, q, t;
    /* 0.5 / 65535 is an exact half: qRound goes up, truncation goes down. */
    c.r = 0.5 / 65535.0;
    c.g = 1.5 / 65535.0;
    c.b = 0.49 / 65535.0;
    c.a = 1.0;
    q = ricecol_q16(c);
    t = ricecol_t16(c);
    CHECK(q.r == 1.0 / 65535.0);
    CHECK(q.g == 2.0 / 65535.0);
    CHECK(q.b == 0.0);
    CHECK(q.a == 1.0);
    CHECK(t.r == 0.0);
    CHECK(t.g == 1.0 / 65535.0);
    c.r = -1.0;
    c.g = 2.0;
    c.b = 0.0;
    c.a = 1.0;
    q = ricecol_q16(c);
    CHECK(q.r == 0.0 && q.g == 1.0);
    t = ricecol_t16(c);
    CHECK(t.r == 0.0 && t.g == 1.0);
}

static void test_normalize_wrap(void)
{
    CHECK(ricecol_normalize(-1.0) == 0.0);
    CHECK(ricecol_normalize(0.0) == 0.0);
    CHECK(ricecol_normalize(0.5) == 0.5);
    CHECK(ricecol_normalize(1.0) == 1.0);
    CHECK(ricecol_normalize(2.0) == 1.0);
    CHECK(ricecol_normalize(NAN) == 1.0); /* KDE's code, not its comment */
    CHECK(ricecol_wrap(-0.25, 1.0) == 0.75);
    CHECK(ricecol_wrap(1.25, 1.0) == 0.25);
    CHECK(ricecol_wrap(1.0, 1.0) == 0.0);
    CHECK(ricecol_wrap(0.0, 1.0) == 0.0);
    CHECK(ricecol_wrap(-1.0, 1.0) == 0.0);
    CHECK(ricecol_wrap(370.0, 360.0) == 10.0);
}

static void test_version(void)
{
    CHECK(strcmp(ricecol_version(), EXPECT_VERSION) == 0);
}

int main(void)
{
    test_8bit();
    test_16bit();
    test_rounding();
    test_normalize_wrap();
    test_version();
    return harness_done("test-core");
}
