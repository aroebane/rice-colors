/* Minimal test support: checks with a 16-bit tolerance and a golden table reader.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef RICECOL_HARNESS_H
#define RICECOL_HARNESS_H

#include <stdio.h>

#include "rice-colors/core.h"

extern int harness_failures;

void harness_fail(const char *file, int line, const char *what);

#define CHECK(cond)                                                                                \
    do {                                                                                           \
        if (!(cond))                                                                               \
            harness_fail(__FILE__, __LINE__, #cond);                                               \
    } while (0)

/* Channel difference of two colors in 16-bit units (max over r, g, b, a). */
double harness_diff16(ricecol_rgba_t a, ricecol_rgba_t b);

/* Checks |a - b| <= tol_units / 65535 on every channel. */
#define CHECK_COLOR(a, b, tol_units)                                                               \
    do {                                                                                           \
        if (harness_diff16((a), (b)) > (tol_units))                                                \
            harness_fail(__FILE__, __LINE__, "color " #a " vs " #b);                               \
    } while (0)

/* Golden table: '#' lines are comments, other lines are tab-separated numbers. */
#define HARNESS_MAX_COLS 32
typedef struct {
    FILE *fp;
    double col[HARNESS_MAX_COLS];
    int ncol;
    long row;
} harness_table_t;

/* Returns 0 on success; prints the reason to stderr on failure. */
int harness_table_open(harness_table_t *t, const char *path);
/* Returns 1 with the next row in t->col, 0 at end of file. */
int harness_table_next(harness_table_t *t);
void harness_table_close(harness_table_t *t);

/* Reads columns i..i+3 as a 16-bit rgba color. */
ricecol_rgba_t harness_col_rgba16(const harness_table_t *t, int i);

/* Prints the result and returns the process exit code. */
int harness_done(const char *name);

#endif
