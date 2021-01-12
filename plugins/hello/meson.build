hello = shared_module(
  'ichise-hello',
  ['plugin.cpp'],
  dependencies: [
    hello_message,
    wfconfig,
    wlroots,
  ],
  install: true,
  install_dir: plugindir,
)

hello_metadata = install_data(
  'plugin.xml',
  install_dir: metadatadir,
  rename: ['ichise-hello.xml'],
)

