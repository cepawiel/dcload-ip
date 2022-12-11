project('dcload', 'c',
    version: '2.0.1',
    default_options : [
        'buildtype=minsize',    # -Os
        'c_std=gnu11',
        'warning_level=3'
    ])

objcopy = find_program('objcopy')

add_project_arguments([
    '-ml', 
    '-m4-single-only',
    '-mpretend-cmove',
    '-Wpedantic',
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
    '-DDCLOAD_VERSION="' + meson.project_version() + '"',
    '-DDREAMCAST_IP="0.0.0.0"'
    # '-DDREAMCAST_IP="172.16.0.99"'
], language: 'c')

incdir = include_directories('include')

subdir('dcload')
subdir('loader')

