#include "trigger.hpp"

#include <wayfire/core.hpp>
#include <wayfire/output.hpp>
#include <wayfire/plugin.hpp>
#include <wayfire/signal-definitions.hpp>
#include <wayfire/util/log.hpp>

namespace ichise::plugin::trigger {
  class Plugin : public wf::plugin_interface_t {
  private:
    std::vector<wf::activator_callback> bindings;
    wf::signal_callback_t reload_config = [&](wf::signal_data_t*) {
      LOGD("reload_config -> Clearing bindings");
      clear_bindings();

      LOGD("reload_config -> Loading bindings");
      load_bindings();

      LOGD("reload_config -> Done");
    };

    void clear_bindings() {
      bindings.clear();

      LOGD("clear_bindings -> Done");
    }

    void load_bindings() {
      LOGD("load_bindings -> Getting trigger config");
      std::shared_ptr<wf::config::section_t> section = wf::get_core().config.get_section("trigger");
      wf::config::section_t::option_list_t config_options = section->get_registered_options();

      LOGD("load_bindings -> Loading triggers");
      bindings.resize(config_options.size());

      Parser parser;

      for (size_t index = 0; index < config_options.size(); index++) {
        std::shared_ptr<wf::config::option_base_t> config_option = config_options[index];
        std::string option_name = config_option->get_name();
        std::shared_ptr<wf::config::option_base_t> option = section->get_option_or(option_name);

        if (option) {
          try {
            LOGD("load_bindings -> Parsing trigger: ", option_name);
            std::unique_ptr<std::stringstream> input =
              std::make_unique<std::stringstream>(option->get_value_str());

            std::shared_ptr<const Trigger> trigger = parser(input);

            bindings[index] = std::bind(std::mem_fn(&Plugin::trigger_binding), this, trigger,
                                        std::placeholders::_1, std::placeholders::_2);

            output->add_activator(
              wf::create_option(
                wf::option_type::from_string<wf::activatorbinding_t>(trigger->binding).value()),
              &bindings[index]);
          } catch (const InvalidSyntaxException exception) {
            LOGE(option_name, "(InvalidSyntaxException): ", exception.what());
          } catch (const LexicalException exception) {
            LOGE(option_name, "(LexicalException): ", exception.what());
          }
        }
      }

      LOGD("load_bindings -> Done");
    }

    bool trigger_binding(std::shared_ptr<const Trigger>& trigger, wf::activator_source_t source,
                         uint32_t value) {
      LOGD("trigger_binding -> Matching binding type");

      if (trigger->signal == std::nullopt) {
        LOGD("trigger_binding -> Running command");
        wf::get_core().run(trigger->payload.c_str());

        LOGD("trigger_binding -> Deactivating plugin");
        output->deactivate_plugin(grab_interface);
      }

      LOGD("trigger_binding -> Done");
      return true;
    }

  public:
    void init() override {
      LOGD("init -> Defining interface");
      grab_interface->name = "trigger";
      grab_interface->capabilities = wf::CAPABILITY_GRAB_INPUT;

      LOGD("init -> Loading bindings");
      load_bindings();

      LOGD("init -> Connecting: `reload-config`");
      wf::get_core().connect_signal("reload-config", &reload_config);

      LOGD("init -> Done");
    }

    void fini() override {
      LOGD("fini -> Disconnecting: `reload-config`");
      wf::get_core().disconnect_signal("reload-config", &reload_config);

      LOGD("fini -> Clearing bindings");
      clear_bindings();

      LOGD("fini -> Done");
    }
  };
} // namespace ichise::plugin::trigger

DECLARE_WAYFIRE_PLUGIN(ichise::plugin::trigger::Plugin);

