project('dcload', 'c',
    version: '3.0.0')

objcopy = find_program('objcopy')

add_project_arguments([
    '-O2', 
    '-ml', 
    '-m4-single-only',
    '-mpretend-cmove',
    '-Wpedantic'
], language: 'c')

add_project_arguments([
    '-Wall', 
    '-Wextra', 
    '-ffreestanding',
    '-fno-zero-initialized-in-bss',
    '-fno-common',
    '-fomit-frame-pointer',
    '-fno-strict-aliasing',
    '-fno-unwind-tables',
    '-fno-asynchronous-unwind-tables',
    '-fno-exceptions',
    '-fno-delete-null-pointer-checks',
    '-fno-stack-protector',
    '-fno-stack-check',
    '-fno-merge-constants',
    '-fno-merge-all-constants',
    '-DEXCEPTION_SECONDS=15',
    '-std=gnu11'
], language: 'c')

add_project_link_arguments([
    '-Wl,--warn-common',
    '-Wl,--no-undefined',
    '-nostartfiles',
    '-nostdlib',
    '-static',
    '-Wl,-z,now'
], language: 'c')

add_project_arguments([
    '-DDCLOAD_VERSION="2.0.1"',
    '-DDREAMCAST_IP="172.16.0.99"'
], language: 'c')

incdir = include_directories('include')

bin_gen = generator(objcopy,
                output  : '@BASENAME@.bin',
                arguments : ['@INPUT@', '@OUTPUT@'])

subdir('dcload')
subdir('loader')

