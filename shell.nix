with (import <nixpkgs> {});

clangStdenv.mkDerivation {
  name = "ichise-shell-dev";
  buildInputs = [
    alsaLib
    cairo
    doxygen
    freetype
    glm
    gobject-introspection
    libcap
    libdrm
    libevdev
    libexecinfo
    libGL
    libinput
    libpulseaudio
    libxkbcommon
    libxml2
    mesa
    meson
    ninja
    protobuf
    wayland
    wayland-protocols
    xorg.xcbutilerrors
    xorg.xcbutilwm
    xwayland
  ];
  nativeBuildInputs = [
    cmake
    pkg-config
  ];
  shellHook = ''
    export CC=clang
    export CXX=clang++
    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
  '';
}

