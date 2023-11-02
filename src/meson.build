value_sources = ['value.cpp']
valuelib = shared_library('value',
                        value_sources,
                        include_directories : incdir)


debug_sources = ['debug.cpp']
debuglib = shared_library('debug',
                        debug_sources,
                        include_directories : incdir,
                        link_with : valuelib,
                        install : true)

