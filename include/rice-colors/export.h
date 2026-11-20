/* rice-colors: symbol visibility and C++ linkage macros.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef RICECOL_EXPORT_H
#define RICECOL_EXPORT_H

/* RICECOL_STATIC:   a vendored copy; exports nothing (hidden visibility).
 * RICECOL_BUILDING: building the shared library; exports the public API.
 * neither:          a program linking the shared library; plain declarations. */
#if defined(RICECOL_STATIC)
#define RICECOL_API __attribute__((visibility("hidden")))
#elif defined(RICECOL_BUILDING)
#define RICECOL_API __attribute__((visibility("default")))
#else
#define RICECOL_API
#endif

#ifdef __cplusplus
#define RICECOL_BEGIN_DECLS extern "C" {
#define RICECOL_END_DECLS }
#else
#define RICECOL_BEGIN_DECLS
#define RICECOL_END_DECLS
#endif

#endif
