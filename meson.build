project(
  'ichise-shell',
  'c',
  'cpp',
  default_options: [
    'cpp_std=c++17',
    'c_std=c11',
    'warning_level=2',
    'werror=false',
  ],
  license: 'AGPL-3.0-or-later',
  meson_version: '>=0.56.0',
  version: '0.0.0',
)

protobuf = dependency('protobuf', required: true)
wayfire = dependency('wayfire', required: true)
wfconfig = dependency('wf-config', required: true)
wlroots = dependency('wlroots', required: true)

add_project_arguments(['-DWLR_USE_UNSTABLE'], language: ['c', 'cpp'])
add_project_arguments(['-DWAYFIRE_PLUGIN'], language: ['c', 'cpp'])

add_project_link_arguments(['-rdynamic'], language: 'cpp')

protoc = find_program('protoc', required: true)

generate_protobuf = generator(
  protoc,
  arguments: ['--proto_path=@CURRENT_SOURCE_DIR@', '--cpp_out=@BUILD_DIR@', '@INPUT@'],
  output: ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
)

plugindir = wayfire.get_variable(pkgconfig: 'plugindir')
messagesdir = plugindir + '/ichise/messages'
metadatadir = wayfire.get_variable(pkgconfig: 'metadatadir')

pkgconfig = import('pkgconfig')
pkgconfig.generate(
  description: 'A Wayfire Shell',
  filebase: meson.project_name(),
  name: meson.project_name(),
  variables: [
    'plugindir=${plugindir}',
    'messagesdir=${messagesdir}',
    'metadatadir=${metadatadir}',
  ],
  version: meson.project_version(),
)

subdir('proto')
subdir('plugins')

message('\n'.join([
  '',
  '#----------------#',
  'Ichise Shell -> Wayfire Plugins @0@'.format(meson.project_version()),
  '',
  'with plugin - hello: @0@'.format(get_option('with_plugin_hello')),
  'with plugin - trigger: @0@'.format(get_option('with_plugin_trigger')),
  '#----------------#',
  '',
]))

