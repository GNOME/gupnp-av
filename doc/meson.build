conf = configuration_data()
conf.set('VERSION', meson.project_version())

version_xml = configure_file(
    input : 'version.xml.in',
    output : 'version.xml',
    configuration: conf
)

gnome.gtkdoc(
    'gupnp-av',
    src_dir : [
        join_paths(meson.source_root(), 'libgupnp-av'),
        join_paths(meson.build_root(), 'libgupnp-av'),
    ],
    main_sgml : 'gupnp-av-docs.sgml',
    content_files : version_xml,
    scan_args : ['--deprecated-guards=GUPNP_DISABLE_DEPRECATED'],
    mkdb_args : ['--sgml-mode', '--output-format=xml'],
    dependencies : gupnp_av,
    ignore_headers : [
        'xml-util.h',
        'xsd-data.h',
        'fragment-util.h',
        'gvalue-util.h',
        'gupnp-av-marshal.h',
        'gunpp-didl-lite-contributor-private.h',
        'gupnp-didl-lite-descriptior-private.h',
        'gupnp-didl-lite-object-private.h',
        'gupnp-didl-lite-resource-private.h'
    ],
    install : true
)
