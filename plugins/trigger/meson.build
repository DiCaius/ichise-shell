wfconfig = dependency('wf-config', required: true)
wlroots = dependency('wlroots', required: true)

trigger = shared_module(
  'trigger',
  ['plugin.cpp', 'trigger.cpp'],
  dependencies: [
    wfconfig,
    wlroots,
  ],
  install: true,
  install_dir: wayfire.get_variable(pkgconfig: 'plugindir'),
)

trigger_metadata = install_data(
  'plugin.xml',
  rename: ['trigger.xml'],
  install_dir: wayfire.get_variable(pkgconfig: 'metadatadir'),
)

