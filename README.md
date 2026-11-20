# rice-colors

Pure C color math. It ports the color math of KDE
(kcolorspaces, kcolorutils, KColorScheme shade and state effects) and GTK2's HLS
shade. It reads no files and has no global state. It does not choose or extract
colors.

## Units

Seven units in layers; a unit links only lower layers.

| Layer | Unit | Header | Content |
|---|---|---|---|
| 0 | core | `rice-colors/core.h` | color type, 8/16-bit conversion, Qt rounding, version |
| 1 | hcy | `rice-colors/hcy.h` | gamma 2.2, luma, RGB to HCY and back |
| 1 | hls | `rice-colors/hls.h` | GTK2 HLS shade, mid, text_aa |
| 2 | utils | `rice-colors/utils.h` | contrast, lighten, darken, shade, mix, tint |
| 3 | shade | `rice-colors/shade.h` | the five shade roles |
| 3 | effects | `rice-colors/effects.h` | disabled and inactive effects |
| 3 | roles | `rice-colors/roles.h` | the calculated backgrounds |

`rice-colors/rice-colors.h` includes all of them. The API prefix is `ricecol_`.

## Build, test, install

    make build
    make check          # format-check, test, lint (clang-tidy), shellcheck
    make install        # PREFIX defaults to ~/.local
    make compdb         # compile_commands.json for clangd, via bear

It installs `librice-colors.so.0`, the headers and `rice-colors.pc`.

## Linking

pkg-config: `pkg-config --cflags --libs rice-colors`. With CMake use
`pkg_check_modules`; with qmake `CONFIG += link_pkgconfig` and
`PKGCONFIG += rice-colors`; with Meson `dependency('rice-colors')`. If the library
is installed outside the default path, set an rpath to it.

## Vendoring

A project may copy `include/rice-colors/` and the `src/*.c` of the units it uses
plus their lower layers, with the `VERSION` file, and compile them with
`-DRICECOL_STATIC`. That makes every symbol hidden, so the plugin exports none of
the library's. Never edit the copies: fix rice-colors and take a new tag.
`ricecol_version()` reports the version. With Meson use
`subproject('rice-colors', default_options: ['default_library=static'])`.

## Golden tables

`tests/golden/*.tsv` come from KDE and GTK2 themselves. To regenerate them you
need Qt 6 and KF6 (GuiAddons, ColorScheme):

    make oracle

Qt 6 does its color math in `float`; this library uses `double`, so the tests
allow 1 unit on 16-bit values and 1e-6 on real values (absolute for the hcy values,
relative for the contrast ratio). A few `tint` inputs flip
a bisection step; they are listed as exceptions in `tests/test-utils.c` and
`tests/test-roles.c`. The hls unit matches GTK2 exactly.

## Upstream quirks

Where the ported code looks wrong the behavior is kept and the spot carries an
`UPSTREAM-QUIRK` comment.

## Licence

GPL-3.0-or-later (`LICENSE`). The color math is ported from LGPL-2.0-or-later code; see
`NOTICE` and `CREDITS.md` for what comes from where.
