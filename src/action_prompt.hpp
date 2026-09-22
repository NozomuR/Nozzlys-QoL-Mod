#pragma once

#include "mods/service.hpp"
#include "mods/svc/ui.h"

namespace qol::action_prompt {

ModResult initialize();
ModResult build_panel(ModContext* ctx, UiElementHandle pane);

}
