project(
  'ichise-shell',
  'c',
  'cpp',
  version: '0.0.0',
  license: 'AGPL-3.0-or-later',
  meson_version: '>=0.56.0',
  default_options: [
    'cpp_std=c++17',
    'c_std=c11',
    'warning_level=2',
    'werror=false',
  ],
)

wayfire = dependency('wayfire', required: true)

add_project_arguments(['-DWLR_USE_UNSTABLE'], language: ['c', 'cpp'])
add_project_arguments(['-DWAYFIRE_PLUGIN'], language: ['c', 'cpp'])

add_project_link_arguments(['-rdynamic'], language: 'cpp')

subdir('plugins')

message('\n'.join([
  '',
  '#----------------#',
  'Ichise Shell -> Wayfire Plugins @0@'.format(meson.project_version()),
  '#----------------#',
  '',
]))

