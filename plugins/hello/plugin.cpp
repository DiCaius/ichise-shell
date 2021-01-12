#include "../signals.hpp"

#include <string>
#include <wayfire/core.hpp>
#include <wayfire/plugin.hpp>
#include <wayfire/signal-definitions.hpp>
#include <wayfire/util/log.hpp>

namespace ichise::plugin::hello {
  class Plugin : public wf::plugin_interface_t {
  private:
    wf::signal_callback_t hello = [&](wf::signal_data_t* data) {
      LOGD("hello -> Casting signal");
      HelloSignal* signal = static_cast<HelloSignal*>(data);

      LOGD("hello -> ", signal->message->greeting(), ", ", signal->message->recipient());
      LOGD("hello -> Done");
    };

  public:
    void init() override {
      LOGD("init -> Connecting: `hello`");
      wf::get_core().connect_signal("hello", &hello);

      LOGD("init -> Done");
    }

    void fini() override { LOGD("fini -> Done"); }
  };
} // namespace ichise::plugin::hello

DECLARE_WAYFIRE_PLUGIN(ichise::plugin::hello::Plugin);

