/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "harness.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

int harness_failures;

void harness_fail(const char *file, int line, const char *what)
{
    harness_failures++;
    if (harness_failures <= 20)
        fprintf(stderr, "FAIL %s:%d: %s\n", file, line, what);
}

static double d16(double a, double b)
{
    return fabs(a - b) * 65535.0;
}

double harness_diff16(ricecol_rgba_t a, ricecol_rgba_t b)
{
    double m = d16(a.r, b.r), d;
    if ((d = d16(a.g, b.g)) > m)
        m = d;
    if ((d = d16(a.b, b.b)) > m)
        m = d;
    if ((d = d16(a.a, b.a)) > m)
        m = d;
    return m;
}

int harness_table_open(harness_table_t *t, const char *path)
{
    memset(t, 0, sizeof *t);
    t->fp = fopen(path, "r");
    if (!t->fp) {
        fprintf(stderr, "cannot open golden table %s\n", path);
        return 1;
    }
    return 0;
}

int harness_table_next(harness_table_t *t)
{
    char line[4096];
    while (fgets(line, sizeof line, t->fp)) {
        char *p, *end;
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
            continue;
        t->ncol = 0;
        for (p = line; *p && *p != '\n';) {
            if (t->ncol >= HARNESS_MAX_COLS)
                break;
            t->col[t->ncol++] = strtod(p, &end);
            if (end == p)
                break;
            p = end;
            while (*p == '\t' || *p == ' ')
                p++;
        }
        t->row++;
        return 1;
    }
    return 0;
}

void harness_table_close(harness_table_t *t)
{
    if (t->fp)
        fclose(t->fp);
    t->fp = NULL;
}

ricecol_rgba_t harness_col_rgba16(const harness_table_t *t, int i)
{
    return ricecol_from_rgba16((uint32_t)t->col[i],
                               (uint32_t)t->col[i + 1],
                               (uint32_t)t->col[i + 2],
                               (uint32_t)t->col[i + 3]);
}

int harness_done(const char *name)
{
    if (harness_failures) {
        fprintf(stderr, "%s: %d failure(s)\n", name, harness_failures);
        return 1;
    }
    printf("%s: ok\n", name);
    return 0;
}
