project('rice-colors', ['c'],
  version: '0.1.0',
  license: 'GPL-3.0-or-later',
  meson_version: '>=1.0',
  default_options: ['c_std=c99', 'warning_level=3', 'werror=true'])

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: false)
inc = include_directories('include')

# A vendored copy (static library) exports nothing; the shared library exports the API.
if get_option('default_library') == 'static'
  unit_args = ['-DRICECOL_STATIC']
else
  unit_args = ['-DRICECOL_BUILDING']
endif

# One internal static library per unit. A unit links only its lower layers, so a
# unit that reaches upward or sideways fails to link in its own test.
unit_lib = {}
unit_libs = []
foreach unit : [
  ['core', []],
  ['hcy', ['core']],
  ['hls', ['core']],
  ['utils', ['core', 'hcy']],
  ['shade', ['core', 'hcy', 'utils']],
  ['effects', ['core', 'hcy', 'utils', 'shade']],
  ['roles', ['core', 'hcy', 'utils', 'effects']],
]
  name = unit[0]
  lower = []
  foreach l : unit[1]
    lower += unit_lib[l]
  endforeach
  lib = static_library('ricecol-' + name, 'src/' + name + '.c',
    include_directories: inc, c_args: unit_args, pic: true,
    gnu_symbol_visibility: 'hidden', dependencies: [m_dep], link_with: lower)
  unit_lib += {name: lib}
  unit_libs += lib
endforeach

rice_colors = library('rice-colors',
  link_whole: unit_libs,
  dependencies: [m_dep],
  version: meson.project_version(),
  soversion: '0',
  gnu_symbol_visibility: 'hidden',
  install: true)

install_subdir('include/rice-colors', install_dir: get_option('includedir'))

pkg = import('pkgconfig')
pkg.generate(rice_colors,
  name: 'rice-colors',
  description: 'Shared color math',
  subdirs: [],
  libraries_private: ['-lm'])

rice_colors_dep = declare_dependency(
  include_directories: inc,
  link_with: rice_colors,
  compile_args: get_option('default_library') == 'static' ? ['-DRICECOL_STATIC'] : [])

if get_option('tests')
  subdir('tests')
endif

if get_option('oracle').enabled()
  add_languages('cpp', native: false)
  subdir('oracle')
endif
