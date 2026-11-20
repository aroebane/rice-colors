# Credits

rice-colors is distributed under GPL-3.0-or-later (see `LICENSE`, `NOTICE`).

Unlike an inspiration-only project, rice-colors is a port: its colour math is translated from
the code below. The original is LGPL-2.0-or-later, which allows the combined work to be
distributed under the GPL. The copyright line of each original stays in the SPDX header of the
file that ports it. Thanks to the authors.

## KDE Frameworks

### kguiaddons
- Upstream: https://invent.kde.org/frameworks/kguiaddons
- Licence: LGPL-2.0-or-later.
- `src/colors/kcolorspaces.cpp` is ported to `src/hcy.c`. Copyright 2007 Matthew Woehlke,
  2007 Olaf Schmidt.
- `src/colors/kcolorutils.cpp` is ported to `src/utils.c`. Copyright 2007 Matthew Woehlke,
  2007 Thomas Zander, 2007 Zack Rusin.

### kcolorscheme
- Upstream: https://invent.kde.org/frameworks/kcolorscheme
- Licence: LGPL-2.0-or-later.
- `src/kcolorscheme.cpp` is ported to `src/shade.c`, `src/effects.c` and `src/roles.c`.
  Copyright Matthew Woehlke (and Olaf Schmidt in `shade.c` and `effects.c`), as the SPDX header of
  each ported file lists them.

## GTK+ 2.24

- Upstream: https://gitlab.gnome.org/GNOME/gtk (GTK+ 2.24 branch, commit `6863194`).
- Licence: LGPL-2.0-or-later.
- `gtk/gtkstyle.c` (`_gtk_style_shade`, `rgb_to_hls`, `hls_to_rgb`) is ported to `src/hls.c`.
  Copyright 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald.
- `oracle/gtk2-shade.c`, a maintainer tool, holds a verbatim copy of that code and keeps its
  LGPL-2.0-or-later header.

## Test oracle

The golden tables are produced by `oracle/kde-oracle.cpp`, which links KDE's own KColorScheme
and KGuiAddons (Qt 6 and KF6) and writes the values the tests compare against. Nothing of
those libraries ships in rice-colors.
