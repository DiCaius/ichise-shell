with (import <nixpkgs> {});

mkShell {
  buildInputs = [
    cairo
    glm
    freetype
    libevdev
    libdrm
    libexecinfo
    libinput
    libjpeg
    libpng
    libGL
    mesa
    libcap
    xorg.xcbutilerrors
    xorg.xcbutilwm
    libxml2
    libxkbcommon
    xwayland
    wayland
    wayland-protocols
    meson
    ninja
    doxygen
    gtk3
    gtkmm3
    gobject-introspection
    libpulseaudio
    alsaLib
  ];
  nativeBuildInputs = [
    pkg-config
    clang
    cmake
  ];
  shellHook = ''
    export CC=clang
    export CXX=clang++
    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
  '';
}

