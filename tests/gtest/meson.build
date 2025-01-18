test_env = [
    'GUPNP_AV_DATADIR="@0@"'.format(join_paths(meson.project_source_root(), 'data')),
    'MALLOC_PERTURB_=170',
    'MALLOC_CHECK_=2'
]

tests = [
    'regression',
    'didl-lite-object',
    'media-collection',
    'last-change-parser',
    'cds-last-change-parser'
]

foreach subtest : tests
    test_name = 'test-' + subtest
    test_source = test_name + '.c'
    test(
        test_name,
        executable(
            test_name,
            test_source,
            dependencies : [gupnp_av, gobject, libxml],
            include_directories: config_h_inc,
            c_args: [common_cflags, '-DDATA_PATH="@0@"'.format(meson.current_source_dir())]
        ),
        env : test_env
    )
endforeach

