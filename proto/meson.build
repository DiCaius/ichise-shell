hello_lib = static_library(
  'hello',
  generate_protobuf.process(
    'hello.proto',
    preserve_path_from: meson.current_source_dir(),
  ),
  dependencies: [protobuf],
  install: true,
  install_dir: messagesdir,
)

hello_message = declare_dependency(
  dependencies: [protobuf],
  include_directories: hello_lib.private_dir_include(),
  link_with: hello_lib
)

