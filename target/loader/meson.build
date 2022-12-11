
dcloadip_bin = custom_target('dcloadip.bin',
    input: dcloadip_elf,
    output: 'dcloadip.bin',
    command: [
        objcopy, 
        '-R', '.stack',
        '-O', 'binary', 
        '@INPUT@', '@OUTPUT@'])

exception_bin = custom_target('exception.bin',
    input: exception_elf,
    output: 'exception.bin',
    command: [
        objcopy, 
        '-R', '.stack',
        '-O', 'binary', 
        '@INPUT@', '@OUTPUT@'])

executable('loader.elf',
    'loader.s',
    'disable.s',
    dcloadip_bin,
    exception_bin,
    link_args: [
        '-Ttext=0x8c010000',
        # TODO: Replace this with Linker Script
        '-Wl,--defsym=_stack=0x8c01F000',
        # '-Wl,-T../loader/loader.x',
        '-Wl,-Map=loader.map'
    ]
)
