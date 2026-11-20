/* Unit test for shade: branch edges, then the golden table.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include <math.h>

#include "harness.h"
#include "rice-colors/shade.h"

static ricecol_rgba_t grey16(uint32_t v)
{
    return ricecol_from_rgba16(v, v, v, 65535);
}

static void test_edges(void)
{
    /* luma 0.006 is channel ~6405, 0.93 is ~63408 (16-bit grey). */
    uint32_t vs[] = {6000, 6404, 6406, 7000, 63000, 63408, 63410, 64000};
    size_t i;
    int role;
    for (i = 0; i < sizeof vs / sizeof vs[0]; i++)
        for (role = RICECOL_SHADE_LIGHT; role <= RICECOL_SHADE_SHADOW; role++) {
            ricecol_rgba_t r =
                ricecol_shade_role(grey16(vs[i]), (ricecol_shade_role_t)role, 0.7, 0.0);
            CHECK(r.a == 1.0);
        }
}

static void test_misc(void)
{
    ricecol_rgba_t c = grey16(30000), r;
    CHECK(ricecol_to_argb8(ricecol_shade_role(c, (ricecol_shade_role_t)7, 0.7, 0.0)) ==
          ricecol_to_argb8(c));
    CHECK(ricecol_to_argb8(ricecol_shade_role(c, (ricecol_shade_role_t)-1, 0.7, 0.0)) ==
          ricecol_to_argb8(c));
    r = ricecol_shade_role(c, RICECOL_SHADE_LIGHT, NAN, 0.0);
    CHECK_COLOR(r, ricecol_shade_role(c, RICECOL_SHADE_LIGHT, 1.0, 0.0), 0);
    CHECK(ricecol_luma(ricecol_shade_role(c, RICECOL_SHADE_LIGHT, 0.7, 0.0)) > ricecol_luma(c));
    CHECK(ricecol_luma(ricecol_shade_role(c, RICECOL_SHADE_DARK, 0.7, 0.0)) < ricecol_luma(c));
    CHECK(fabs(ricecol_contrast_from_kde(7) - 0.7) < 1e-12);
    CHECK(ricecol_contrast_from_kde(-3) < 0.0);
    CHECK(ricecol_frame_contrast_clamp(-1.0) == 0.0);
    CHECK(ricecol_frame_contrast_clamp(4.0) == 1.0);
    CHECK(ricecol_frame_contrast_clamp(0.25) == 0.25);
    CHECK(isnan(ricecol_frame_contrast_clamp(NAN)));
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
        ricecol_rgba_t r =
            ricecol_shade_role(c, (ricecol_shade_role_t)(int)t.col[4], t.col[5], t.col[6]);
        if (harness_diff16(r, harness_col_rgba16(&t, 7)) > 1) {
            fprintf(stderr,
                    "row %ld role %g contrast %g chroma %g\n",
                    t.row,
                    t.col[4],
                    t.col[5],
                    t.col[6]);
            CHECK(0);
        }
    }
    harness_table_close(&t);
}

int main(int argc, char **argv)
{
    test_edges();
    test_misc();
    if (argc > 1)
        test_golden(argv[1]);
    return harness_done("shade");
}
