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

wayfire = dependeny('wayfire', required: true)

subdir('plugins')

message('\n'.join([
  '',
  '#----------------#',
  'Ichise Shell -> Wayfire Plugins @0@'.format(meson.project_version()),
  '',
  'system fmt: @0@'.format(use_system_fmt_flag),
  'system nlohmann_json: @0@'.format(use_system_nlohmann_json_flag),
  'system wayfire: @0@ {'.format(use_system_wayfire_flag),
  '  enable_gles32=@0@'.format(get_option('enable_gles32')),
  '  use_system_wfconfig=@0@'.format(use_system_wfconfig_flag),
  '  use_system_wlroots=@0@'.format(use_system_wlroots_flag),
  '  xwayland=@0@'.format(xwayland_flag),
  '}',
  '#----------------#',
  '',
]))

