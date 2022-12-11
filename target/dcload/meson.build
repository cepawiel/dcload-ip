
dcloadip_elf = executable('dcloadip.elf',
    'dcload-crt0.s',
    'adapter.c',
    # 'bswap.s',
    'cdfs_redir.s',
    'cdfs_syscalls.c',
    'commands.c',
    'dcload.c',
    'dhcp.c',
    'disable.s',
    'go.s',
    'lan_adapter.c',
    'maple.c',
    'memcpy.S',
    'memfuncs.c',
    'net.c',
    'packet.c',
    'perfctr.c',
    'rtl8139.c',
    'scif.c',
    'startup_support.c',
    'syscalls.c',
    'video.s',
    include_directories: incdir,
    link_args: [
        '-Wl,-T../dcload/dcload.x',
        '-Wl,-Map,dcload.map',
        '-lgcc'
    ]
)

exception_elf = executable('exception.elf',
    'exception.S',
    link_args: [
        '-Wl,-Ttext=0x8c00f400',
        '-Wl,-Map,exception.map',
    ]
)