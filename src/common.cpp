#include "common.hpp"

namespace qol {

bool enabled(ConfigVarHandle var) {
    bool value = false;
    svc_config->get_bool(mod_ctx, var, &value);
    return value;
}

ModResult register_toggle(const char* name, ConfigVarHandle* out) {
    ConfigVarDesc desc = CONFIG_VAR_DESC_INIT;
    desc.name = name;
    desc.default_bool = true;
    return svc_config->register_var(mod_ctx, &desc, out);
}

ModResult add_toggle(ModContext* ctx, UiElementHandle pane, const char* label,
                     ConfigVarHandle var) {
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_TOGGLE;
    desc.label = label;
    desc.binding = UI_BINDING_CONFIG_VAR;
    desc.config_var = var;
    return svc_ui->pane_add_control(ctx, pane, &desc, nullptr);
}

}
