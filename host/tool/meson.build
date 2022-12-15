
libelf = dependency('libelf')

executable('dctool',
    'dctool.cpp',
    link_with: [libdctool_lib],
    include_directories: [libdctool_inc],
    dependencies: [libelf],
    )
