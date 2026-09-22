#pragma once

#include "mods/service.hpp"
#include "mods/svc/ui.h"

// Mutes the letterbox (L) and lock-on sound effects.
namespace qol::lock_sounds {

ModResult initialize();
ModResult build_panel(ModContext* ctx, UiElementHandle pane);

}
