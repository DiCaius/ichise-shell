#include <wayfire/plugin.hpp>
#include <wayfire/util/log.hpp>

namespace ichise::plugin::trigger {
  class Plugin : public wf::plugin_interface_t {
  public:
    void init() override { LOGI("INIT"); }

    void fini() override { LOGI("FINI"); }
  };
} // namespace ichise::plugin::trigger

DECLARE_WAYFIRE_PLUGIN(ichise::plugin::trigger::Plugin);
