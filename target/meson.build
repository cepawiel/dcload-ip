project('dcload', 'c',
    version: '2.0.1',
    default_options : [
        'buildtype=minsize',    # -Os
        'c_std=gnu11',
        'warning_level=3',
    ])

objcopy = find_program('objcopy')

repo_str = get_option('repo')
if repo_str == ''
    message('Generating repo_str')
    repo_str = run_command('bash', '-c', 'git config --get remote.origin.url | sed -e "s/^git@.*:\\([[:graph:]]*\\).git/\\1/"', check: true).stdout().strip()
endif

branch_str = get_option('branch')
if branch_str == ''
    message('Generating branch_str')
    branch_str = run_command('git', 'branch', '--show-current', check: true).stdout().strip()
endif

sha_str = get_option('sha').substring(0,6)
if sha_str == ''
    message('Generating sha_str')
    sha_str = run_command('git', 'rev-parse', '--short', 'HEAD', check: true).stdout().strip()

    is_dirty = run_command('bash', '-c', 'git diff --quiet || echo \'dirty\'', check: true).stdout().strip()
    if is_dirty == 'dirty'
        sha_str +=  '-dirty'
    endif
endif

datetime_str = get_option('datetime')
if datetime_str == ''
    message('Generating datetime_str')
    datetime_str = run_command('date', check: true).stdout().strip()
endif

message('Building dcloadip Version: ', repo_str + ' ' + branch_str + ' ' + sha_str)
message('At ' + datetime_str)

add_project_arguments([
    '-DDCLOAD_VERSION="' + meson.project_version() + '"',
    '-DDCLOAD_GITREPO="' + repo_str + '"',
    '-DDCLOAD_GITBRANCH="' + branch_str + '"',
    '-DDCLOAD_GITSHA="' + sha_str + '"',
    '-DDCLOAD_BUILDDATETIME="' + datetime_str + '"',
    # '-DDREAMCAST_IP="172.16.0.99"'
], language: 'c')

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


incdir = include_directories('include')

subdir('dcload')
subdir('loader')

