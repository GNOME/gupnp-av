conf = configuration_data()
conf.set('VERSION', meson.project_version())

if get_option('gtk_doc')
  gidocgen = find_program('gi-docgen', required: true)

  gupnp_av_toml = configure_file (
      input: 'gupnp-av.toml.in',
      output: 'gupnp-av.toml',
      configuration: conf
  )

  docs_dir = join_paths(get_option('prefix'), get_option('datadir')) / 'doc'

  custom_target(
      'gupnp-av-doc',
      input: [ gupnp_av_toml, gupnp_av_gir[0] ],
      output: GUPNP_AV_API_NAME,
      command : [
          gidocgen,
          'generate',
          '--quiet',
          '--add-include-path=@0@'.format(meson.current_build_dir() / '../libgupnp-av'),
          '--config', gupnp_av_toml,
          '--output-dir=@OUTPUT@',
          '--no-namespace-dir',
          '--content-dir=@0@'.format(meson.current_source_dir()),
          '@INPUT1@',
      ],
      depend_files : [
          gupnp_av_toml,
      ],
      build_by_default: true,
      install: true,
      install_dir : docs_dir,
  )
endif
