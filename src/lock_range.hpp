#pragma once

#include "mods/service.hpp"
#include "mods/svc/ui.h"
#include "lock_sounds.hpp"
#include "lock_range.hpp"
#include "settings_window.hpp"


namespace qol::lock_range {

ModResult initialize();
ModResult build_panel(ModContext* ctx, UiElementHandle pane);

}
