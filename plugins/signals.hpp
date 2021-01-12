#pragma once

#include "hello.pb.h"

#include <memory>
#include <wayfire/signal-definitions.hpp>

namespace ichise::plugin {
  template <typename TMessage> struct IchiseSignal : wf::signal_data_t {
    std::unique_ptr<TMessage> message;

    IchiseSignal() : message(std::make_unique<TMessage>()) {}
  };

  struct HelloSignal : IchiseSignal<Hello> {};
} // namespace ichise::plugin

