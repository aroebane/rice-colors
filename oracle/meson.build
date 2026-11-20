# Maintainer tool: regenerates tests/golden from KDE's own code. Never installed.
qt6 = dependency('Qt6Gui', method: 'cmake', modules: ['Qt6::Gui'])
colorscheme = dependency('KF6ColorScheme', method: 'cmake', modules: ['KF6::ColorScheme'])
guiaddons = dependency('KF6GuiAddons', method: 'cmake', modules: ['KF6::GuiAddons'])

kde_oracle = executable('kde-oracle', 'kde-oracle.cpp',
  dependencies: [qt6, guiaddons, colorscheme],
  override_options: ['cpp_std=c++17', 'warning_level=1', 'werror=false', 'b_pie=true'],
  cpp_args: ['-fPIC'], link_args: ['-pie'], install: false)

# GTK2 keeps _gtk_style_shade private, so gtk2-shade.c carries a verbatim copy.
gtk2_oracle = executable('gtk2-oracle', ['gtk2-oracle.c', 'gtk2-shade.c'],
  dependencies: [m_dep], install: false)

golden_dir = meson.project_source_root() / 'tests' / 'golden'
run_target('golden',
  command: ['sh', '-c', '"$0" "$2" && "$1" "$2"', kde_oracle, gtk2_oracle, golden_dir])
