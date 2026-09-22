#pragma once

#include "mods/service.hpp"
#include "mods/svc/config.h"
#include "mods/svc/ui.h"

#define QOL_TRY(call)                \
    do {                             \
        const ModResult r_ = (call); \
        if (r_ != MOD_OK) {          \
            return r_;               \
        }                            \
    } while (false)

namespace qol {

struct Feature {
    ModResult (*initialize)();
    ModResult (*build_panel)(ModContext* ctx, UiElementHandle pane);
};

bool enabled(ConfigVarHandle var);

ModResult register_toggle(const char* name, ConfigVarHandle* out);

ModResult add_toggle(ModContext* ctx, UiElementHandle pane, const char* label, ConfigVarHandle var);

}
