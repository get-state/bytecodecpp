value_sources = ['value.cpp']
valuelib = static_library('value', value_sources, include_directories: incdir)

debug_sources = ['debug.cpp']
debuglib = static_library('debug', debug_sources, include_directories: incdir, link_with: valuelib)

scanner_sources = ['scanner.cpp']
scannerlib = static_library('scanner', scanner_sources, link_with: debuglib, include_directories: incdir)

compiler_sources = ['compiler.cpp', 'scanner.cpp']
compilerlib = static_library('compiler', compiler_sources, link_with: [scannerlib,debuglib], include_directories: incdir)

vm_sources = ['vm.cpp']
vmlib = static_library('vm', vm_sources, include_directories: incdir, link_with: [valuelib, debuglib, scannerlib, compilerlib])
