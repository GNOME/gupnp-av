gnome.generate_vapi(
    'gupnp-av-1.0',
    metadata_dirs : meson.current_source_dir(),
    sources : [
        gupnp_av_gir.get(0),
        'gupnp-av-1.0-custom.vala'
    ],
    packages : ['gobject-2.0', 'libxml-2.0'],
    install : true
)
