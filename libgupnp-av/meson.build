marshall_sources = gnome.genmarshal('gupnp-av-marshal', prefix : 'gupnp_av_marshal', sources : 'gupnp-av-marshal.list')

introspection_sources = [
    'gupnp-av-error.c',
    'gupnp-cds-last-change-parser.c',
    'gupnp-didl-lite-container.c',
    'gupnp-didl-lite-contributor.c',
    'gupnp-didl-lite-createclass.c',
    'gupnp-didl-lite-descriptor.c',
    'gupnp-didl-lite-item.c',
    'gupnp-didl-lite-object.c',
    'gupnp-didl-lite-parser.c',
    'gupnp-didl-lite-resource.c',
    'gupnp-didl-lite-writer.c',
    'gupnp-dlna.c',
    'gupnp-feature.c',
    'gupnp-feature-list-parser.c',
    'gupnp-last-change-parser.c',
    'gupnp-media-collection.c',
    'gupnp-protocol-info.c',
    'gupnp-search-criteria-parser.c'
]


gupnp_av_lib = library('gupnp-av-1.0',
        [
            introspection_sources,
            'fragment-util.c',
            'gvalue-util.c',
            'time-utils.c',
            'xml-util.c',
            'xsd-data.c',
            marshall_sources
        ],
        install: true,
        version : '2.0.0',
        c_args : common_cflags + ['-DDATADIR="@0@"'.format(join_paths(get_option('prefix'), get_option('datadir'), 'gupnp-av'))],
        dependencies : [gobject, libxml]
)
gupnp_av = declare_dependency(link_with : gupnp_av_lib, include_directories : include_directories('..'))

meson.override_dependency('gupnp-av-1.0', gupnp_av)

public_headers = [
        'gupnp-av-enums.h',
        'gupnp-av-error.h',
        'gupnp-av.h',
        'gupnp-cds-last-change-parser.h',
        'gupnp-didl-lite-container.h',
        'gupnp-didl-lite-contributor.h',
        'gupnp-didl-lite-createclass.h',
        'gupnp-didl-lite-descriptor.h',
        'gupnp-didl-lite-item.h',
        'gupnp-didl-lite-object.h',
        'gupnp-didl-lite-parser.h',
        'gupnp-didl-lite-resource.h',
        'gupnp-didl-lite-writer.h',
        'gupnp-dlna.h',
        'gupnp-feature.h',
        'gupnp-feature-list-parser.h',
        'gupnp-last-change-parser.h',
        'gupnp-media-collection.h',
        'gupnp-protocol-info.h',
        'gupnp-search-criteria-parser.h',
]

install_headers(
    public_headers,
    subdir : 'gupnp-av-1.0/libgupnp-av'
)

if get_option('introspection')
    gupnp_av_gir = gnome.generate_gir(
        gupnp_av_lib,
        sources : [public_headers, introspection_sources],
        namespace : 'GUPnPAV',
        nsversion : '1.0',
        identifier_prefix : 'GUPnP',
        symbol_prefix : 'gupnp',
        export_packages : 'gupnp-av-1.0',
        includes : ['GObject-2.0', 'libxml2-2.0'],
        install : true
    )
endif
