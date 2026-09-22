#include "action_prompt.hpp"

#include "common.hpp"

#include "mods/svc/hook.hpp"

#include "d/d_com_inf_game.h"
#include "d/d_meter2_draw.h"

DEFINE_HOOK(&dMeter2Draw_c::drawButtonA, DrawButtonA);
DEFINE_HOOK(&dMeter2Draw_c::getActionString, GetActionString);

namespace qol::action_prompt {
namespace {

// The draw type the HUD uses for the emphasised prompt; 1 is the plain one.
constexpr u8 kEmphasisDrawType = 7;
constexpr u8 kPlainDrawType = 1;

ConfigVarHandle no_prompt, no_flash;

HookAction on_draw_button_a(ModContext*, void* args, void*, void*) {
    const u8 action = mods::arg<u8>(args, 1);
    bool& changed = mods::arg_ref<bool>(args, 7);

    if (enabled(no_prompt) && dComIfGp_isDoSetFlag(BUTTON_STATUS_FLAG_EMPHASIS)) {
        dComIfGp_setDoStatus(action, BUTTON_STATUS_FLAG_NONE);
    }
    if (enabled(no_flash)) {
        changed = false;
    }
    return HOOK_CONTINUE;
}

void on_get_action_string(ModContext*, void* args, void*, void*) {
    u8* draw_type = mods::arg<u8*>(args, 3);
    if (enabled(no_prompt) && draw_type != nullptr && *draw_type == kEmphasisDrawType) {
        *draw_type = kPlainDrawType;
    }
}

}

ModResult initialize() {
    QOL_TRY(register_toggle("no_action_prompt", &no_prompt));
    QOL_TRY(register_toggle("no_action_flash", &no_flash));
    QOL_TRY(mods::hook::add_pre<DrawButtonA>(on_draw_button_a));
    return mods::hook::add_post<GetActionString>(on_get_action_string);
}

ModResult build_panel(ModContext* ctx, UiElementHandle pane) {
    QOL_TRY(add_toggle(ctx, pane, "Remove bottom action prompt", no_prompt));
    return add_toggle(ctx, pane, "Remove A button flash", no_flash);
}

}
