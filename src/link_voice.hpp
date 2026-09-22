#pragma once

#include "mods/service.hpp"
#include "mods/svc/ui.h"

namespace qol::link_voice {

ModResult initialize();
ModResult build_submenus(ModContext* ctx, UiElementHandle group_pane, UiElementHandle target_pane);

}
