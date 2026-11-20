#!/bin/sh
# Checks the symbols a built object exports.
#   check-exports.sh LIB            every exported symbol starts with ricecol_ or RICECOL_,
#                                   and the soname is librice-colors.so.0
#   check-exports.sh --vendored OBJ no ricecol_ or RICECOL_ symbol is exported
set -eu

vendored=0
if [ "${1:-}" = "--vendored" ]; then
  vendored=1
  shift
fi
[ $# -eq 1 ] || { echo "usage: check-exports.sh [--vendored] FILE" >&2; exit 2; }
file=$1

symbols=$(nm -D --defined-only "$file" | awk '{ print $NF }')

if [ "$vendored" -eq 1 ]; then
  leaked=$(printf '%s\n' "$symbols" | grep -E '^(ricecol_|RICECOL_)' || true)
  if [ -n "$leaked" ]; then
    echo "vendored copy exports library symbols:" >&2
    printf '%s\n' "$leaked" >&2
    exit 1
  fi
  exit 0
fi

# Linker-provided and sanitizer (__odr_asan.*) symbols are not ours.
stray=$(printf '%s\n' "$symbols" | grep -Ev '^(ricecol_|RICECOL_|__odr_asan\.|_init$|_fini$|_edata$|_end$|__bss_start$)' || true)
if [ -n "$stray" ]; then
  echo "unexpected exported symbols:" >&2
  printf '%s\n' "$stray" >&2
  exit 1
fi

soname=$(readelf -d "$file" | sed -n 's/.*(SONAME).*\[\(.*\)\]/\1/p')
[ "$soname" = "librice-colors.so.0" ] || { echo "soname is '$soname', expected librice-colors.so.0" >&2; exit 1; }
