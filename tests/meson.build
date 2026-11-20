harness = static_library('harness', 'harness.c', include_directories: inc, dependencies: [m_dep])

# Each test links only its own unit's static library (which carries its lower
# layers), so a unit that reaches upward or sideways fails to link.
foreach t : [
  ['core', 'core', []],
  ['hcy', 'hcy', ['hcy.tsv']],
  ['hls', 'hls', ['hls.tsv']],
  ['utils', 'utils', ['utils.tsv']],
  ['shade', 'shade', ['shade.tsv']],
  ['effects', 'effects', ['effects.tsv']],
  ['roles', 'roles', ['roles.tsv']],
]
  exe = executable('test-' + t[0], 'test-' + t[0] + '.c',
    include_directories: inc, c_args: unit_args,
    link_with: [harness, unit_lib[t[1]]], dependencies: [m_dep])
  args = []
  foreach g : t[2]
    args += meson.current_source_dir() / 'golden' / g
  endforeach
  test('test-' + t[0], exe, args: args)
endforeach

if get_option('default_library') != 'static'
  test('test-exports', find_program('sh'),
    args: [files('check-exports.sh'), rice_colors])
endif

# C++98: the umbrella header compiles as gnu++98 (G6), against the shared library.
if add_languages('cpp', required: false, native: false)
  test('test-cxx98', executable('test-cxx98', 'cxx98.cpp',
    include_directories: inc, link_with: rice_colors,
    override_options: ['cpp_std=gnu++98', 'werror=true']))
endif

# A vendored copy (all sources, -DRICECOL_STATIC) exports no ricecol_ symbol.
vendor_probe = shared_library('vendor-probe', ['vendor-probe.c', '../src/core.c', '../src/hcy.c',
    '../src/hls.c', '../src/utils.c', '../src/shade.c', '../src/effects.c', '../src/roles.c'],
  include_directories: inc, c_args: ['-DRICECOL_STATIC'], dependencies: [m_dep],
  gnu_symbol_visibility: 'hidden', install: false)
test('test-vendor', find_program('sh'),
  args: [files('check-exports.sh'), '--vendored', vendor_probe])
