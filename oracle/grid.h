/* Input grid shared by both oracle programs, so every golden table uses the
 * same colors and amounts. Colors are 16-bit rgba.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef ORACLE_GRID_H
#define ORACLE_GRID_H

#define GRID_AMOUNTS 9
static const double grid_amounts[GRID_AMOUNTS] = {-0.5, 0.0, 0.1, 0.3, 0.5, 0.65, 0.9, 1.0, 1.5};

/* 7 values per channel, from 8-bit and from 16-bit. */
static const unsigned grid_v8[7] = {0, 1, 64, 127, 128, 254, 255};
static const unsigned grid_v16[7] = {0, 1, 16384, 32767, 32768, 65534, 65535};

/* Greys whose luma straddles the shade() branch edges 0.006 and 0.93, as 16-bit grey channels. */
typedef struct {
    unsigned r, g, b, a;
} grid_color_t;

/* Writes the whole color list (alpha 65535) to out; returns the count. */
static int grid_colors(grid_color_t *out)
{
    int n = 0, i, j, k, set;
    for (set = 0; set < 2; set++) {
        const unsigned *v = set ? grid_v16 : grid_v8;
        for (i = 0; i < 7; i++)
            for (j = 0; j < 7; j++)
                for (k = 0; k < 7; k++) {
                    unsigned m = set ? 1 : 257;
                    out[n].r = v[i] * m;
                    out[n].g = v[j] * m;
                    out[n].b = v[k] * m;
                    out[n].a = 65535;
                    n++;
                }
    }
    /* Edge greys: channel values around luma 0.006 (v ~ 6405) and 0.93 (v ~ 63408). */
    {
        static const unsigned e[] = {
            6403, 6404, 6405, 6406, 6407, 63406, 63407, 63408, 63409, 63410};
        for (i = 0; i < 10; i++) {
            out[n].r = out[n].g = out[n].b = e[i];
            out[n].a = 65535;
            n++;
        }
    }
    return n;
}
#define GRID_MAX_COLORS 700

#endif
