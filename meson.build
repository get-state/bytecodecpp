project('bcpplox', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

# glib_dep = dependency('glib-2.0')
incdir = include_directories('include')

subdir('src')

exe = executable('bcpplox', 'bcpplox.cpp', include_directories : incdir, link_with : debuglib,
  install : true)

test('basic', exe)
