

testing_headers = include_directories('include')
executable('run_tests',
    'src/Main.cpp',
    'src/Tests.cpp',
    'tests/001-basic_tests.cpp',
    link_with: [libdctool_lib],
    include_directories: [testing_headers, libdctool_inc],
    )