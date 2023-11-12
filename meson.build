project('bcpplox', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

# glib_dep = dependency('glib-2.0')
incdir = include_directories('include')

subdir('src')

libs = [debuglib, valuelib, vmlib, scannerlib, compilerlib]
exe = executable('bcpplox', 'bcpplox.cpp', include_directories : incdir, link_with : libs,
  install : true)

test('basic', exe)
