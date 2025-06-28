project('gupnp-av', 'c', version : '0.14.4', default_options: ['c_std=c11'], meson_version:  '>= 0.58.0')

gnome = import('gnome')

glib_version = '2.58'
gobject = dependency('gobject-2.0', version : '>= ' + glib_version)
glib = dependency('glib-2.0', version : '>= ' + glib_version)
libxml = dependency('libxml-2.0')

GUPNP_AV_API_NAME='gupnp-av-1.0'

cc = meson.get_compiler('c')

# Compiler flags, taken from the Xorg macros
if cc.get_id() == 'msvc'
  # Compiler options taken from msvc_recommended_pragmas.h
  # in GLib, based on _Win32_Programming_ by Rector and Newcomer
  test_cflags = [
    '-we4002', # too many actual parameters for macro
    '-we4003', # not enough actual parameters for macro
    '-w14010', # single-line comment contains line-continuation character
    '-we4013', # 'function' undefined; assuming extern returning int
    '-w14016', # no function return type; using int as default
    '-we4020', # too many actual parameters
    '-we4021', # too few actual parameters
    '-we4027', # function declared without formal parameter list
    '-we4029', # declared formal parameter list different from definition
    '-we4033', # 'function' must return a value
    '-we4035', # 'function' : no return value
    '-we4045', # array bounds overflow
    '-we4047', # different levels of indirection
    '-we4049', # terminating line number emission
    '-we4053', # an expression of type void was used as an operand
    '-we4071', # no function prototype given
    '-we4819', # the file contains a character that cannot be represented in the current code page
  ]
elif cc.get_id() == 'gcc' or cc.get_id() == 'clang'
  test_cflags = [
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wformat=2',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wnested-externs',
    '-Wbad-function-cast',
    '-Wold-style-definition',
    '-Wunused',
    '-Wuninitialized',
    '-Wshadow',
    '-Wmissing-noreturn',
    '-Wmissing-format-attribute',
    '-Wredundant-decls',
    '-Wlogical-op',
    '-Werror=implicit',
    '-Werror=nonnull',
    '-Werror=init-self',
    '-Werror=main',
    '-Werror=missing-braces',
    '-Werror=sequence-point',
    '-Werror=return-type',
    '-Werror=trigraphs',
    '-Werror=array-bounds',
    '-Werror=write-strings',
    '-Werror=address',
    '-Werror=int-to-pointer-cast',
    '-Werror=pointer-to-int-cast',
    '-fno-strict-aliasing',
    '-Wno-int-conversion',
  ]
else
  test_cflags = []
endif

common_cflags = cc.get_supported_arguments(test_cflags)

#add_project_arguments(cc.get_supported_arguments('-Werror=deprecated-declarations'), language: 'c')
conf = configuration_data()
conf.set_quoted('DATADIR', join_paths(get_option('prefix'), get_option('datadir'), 'gupnp-av'))
conf.set_quoted('VERSION', meson.project_version())
conf.set('GLIB_VERSION_MIN_REQUIRED', 'GLIB_VERSION_' + glib_version.underscorify())
conf.set('GLIB_VERSION_MAX_ALLOWED', 'GLIB_VERSION_' + glib_version.underscorify())
subdir('internal')

subdir('libgupnp-av')
subdir('tests')
subdir('data')


pkg = import('pkgconfig')
pkg.generate(
    gupnp_av_lib,
    requires : [
      'glib-2.0',
      'gobject-2.0',
      'gio-2.0',
      'libxml-2.0'
    ],
    subdirs : 'gupnp-av-1.0',
    description : 'GObject-based AV specific UPnP library'
)

gidocgen_dep = dependency('gi-docgen', version: '>= 2021.1',
                          fallback: ['gi-docgen', 'dummy_dep'],
                          native: true,
                          required: get_option('gtk_doc') and get_option('introspection')
                      )

if get_option('gtk_doc')

gidocgen_dep = dependency('gi-docgen', version: '>= 2021.1',
                          fallback: ['gi-docgen', 'dummy_dep'],
                          required: get_option('gtk_doc') and get_option('introspection')
                      )

    subdir('doc')
endif

if get_option('introspection') and get_option('vapi')
    subdir('vala')
endif

if not meson.is_subproject()
    meson.add_dist_script('build-aux/dist-docs.py')
endif


