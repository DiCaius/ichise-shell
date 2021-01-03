fmt = dependency('fmt', required: true)
nlohmann_json = dependency('nlohmann_json', required: true)
wfconfig = dependency('wf-config', required: true)
wlroots = dependency('wlroots', required: true)

trigger = shared_module(
  'trigger',
  'plugin.cpp',
  dependencies: [fmt, nlohmann_json, wfconfig, wlroots],
  install: true,
  install_dir: wayfire.get_variable(pkgconfig: 'plugindir'),
)

trigger_metadata = install_data(
  'plugin.xml',
  rename: ['trigger.xml'],
  install_dir: wayfire.get_variable(pkgconfig: 'metadatadir'),
)
