value_sources = ['value.cpp']
valuelib = shared_library('value', value_sources, include_directories: incdir)

debug_sources = ['debug.cpp']
debuglib = shared_library('debug', debug_sources, include_directories: incdir, link_with: valuelib)

scanner_sources = ['scanner.cpp']
scannerlib = shared_library('scanner', scanner_sources, include_directories: incdir)

compiler_sources = ['compiler.cpp']
compilerlib = shared_library('compiler', compiler_sources, link_with: scannerlib, include_directories: incdir)

vm_sources = ['vm.cpp']
vmlib = shared_library('vm', vm_sources, include_directories: incdir, link_with: [valuelib, debuglib, scannerlib, compilerlib])
