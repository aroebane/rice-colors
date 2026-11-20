/* A vendored build must export none of the library's symbols. This probe is
 * built with every C source of src/ under -DRICECOL_STATIC; its one exported function is
 * not a ricecol_ symbol.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "rice-colors/rice-colors.h"

__attribute__((visibility("default"))) double probe_tint(void)
{
    return ricecol_tint(ricecol_from_rgb8(0x3daee9UL), ricecol_from_rgb8(0x31363bUL), 0.3).r;
}
