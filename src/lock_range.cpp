#include "lock_range.hpp"

#include "common.hpp"

namespace qol::lock_range {
namespace {

constexpr int64_t kDefaultRangePercent = 100;
constexpr int64_t kMinRangePercent = 50;
constexpr int64_t kMaxRangePercent = 300;
constexpr int64_t kRangeStepPercent = 10;

ConfigVarHandle range_percent;

}

ModResult initialize() {
    ConfigVarDesc desc = CONFIG_VAR_DESC_INIT;
    desc.name = "range_percent";
    desc.type = CONFIG_VAR_INT;
    desc.default_int = kDefaultRangePercent;
    return svc_config->register_var(mod_ctx, &desc, &range_percent);
}

ModResult build_panel(ModContext* ctx, UiElementHandle pane) {
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_NUMBER;
    desc.label = "Lock-on range";
    desc.binding = UI_BINDING_CONFIG_VAR;
    desc.config_var = range_percent;
    desc.min = kMinRangePercent;
    desc.max = kMaxRangePercent;
    desc.step = kRangeStepPercent;
    desc.suffix = "%";
    return svc_ui->pane_add_control(ctx, pane, &desc, nullptr);
}

}
