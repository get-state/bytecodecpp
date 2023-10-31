project('bcpplox', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

incdir = include_directories('include')
exe = executable('bcpplox', 'bcpplox.cpp', include_directories : incdir,
  install : true)

test('basic', exe)
