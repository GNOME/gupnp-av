executable(
    'test-search-criteria-parser',
    [
        'test-search-criteria-parser.c'
    ],
    c_args : common_cflags,
    include_directories: config_h_inc,
    dependencies : [gobject, libxml, gupnp_av]
)

check_search = executable(
    'check-search',
    'check-search.c',
    c_args : common_cflags,
    include_directories: config_h_inc,
    dependencies : [gobject, libxml, gupnp_av]
)

check_feature_list_parser = executable(
    'check-feature-list-parser',
    'check-feature-list-parser.c',
    c_args : common_cflags,
    include_directories: config_h_inc,
    dependencies : [gobject, libxml, gupnp_av]
)

fragments = executable(
    'fragments',
    'fragments.c',
    c_args : [common_cflags, '-DABS_TOP_SRCDIR="@0@"'.format(meson.project_source_root())],
    include_directories: config_h_inc,
    dependencies : [gobject, libxml, gupnp_av]
)

test('check-search', check_search)
test('check-feature-list-parser', check_feature_list_parser)
test('fragments', fragments)

subdir('gtest')
