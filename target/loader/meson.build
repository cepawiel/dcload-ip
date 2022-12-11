
dcloadip_bin = custom_target('dcloadip.bin',
    input: dcloadip_elf,
    output: 'dcloadip.bin',
    command: [
        objcopy, 
        # '-j', '.text',
        '-O', 'binary', 
        '@INPUT@', '@OUTPUT@'])

exception_bin = custom_target('exception.bin',
    input: exception_elf,
    output: 'exception.bin',
    command: [
        objcopy, 
        '-j', '.text',
        '-O', 'binary', 
        '@INPUT@', '@OUTPUT@'])

# dcloadip_o = custom_target('dcloadip.o',
#     input: dcloadip_bin,
#     output: 'dcloadip.o',
#     command: [
#         objcopy, 
#         '-I', 'binary', 
#         '-O', 'elf32-shl', 
#         '--rename-section', '.data=.dcload',
#         '@INPUT@', '@OUTPUT@'])

# exception_o = custom_target('exception.o',
#     input: exception_bin,
#     output: 'exception.o',
#     command: [
#         objcopy, 
#         '-I', 'binary', 
#         '-O', 'elf32-shl',
#         '--rename-section', '.text=.exception',
#         '@INPUT@', '@OUTPUT@'])

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
