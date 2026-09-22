#include "lock_sounds.hpp"

#include "common.hpp"

#include "mods/svc/hook.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "d/d_attention.h"

DEFINE_HOOK(&dAttention_c::lockSoundStart, LockSoundStart);

namespace qol::lock_sounds {
namespace {

ConfigVarHandle no_letterbox, no_lock_on;

HookAction on_lock_sound_start(ModContext*, void* args, void*, void*) {
    switch (mods::arg<u32>(args, 1)) {
    case Z2SE_SY_CAMERA_L_MOVE:
    case Z2SE_SY_CAMERA_L_CANCEL:
        return enabled(no_letterbox) ? HOOK_SKIP_ORIGINAL : HOOK_CONTINUE;
    case Z2SE_SY_L_FOCUS_SET:
    case Z2SE_SY_L_FOCUS_RESET:
        return enabled(no_lock_on) ? HOOK_SKIP_ORIGINAL : HOOK_CONTINUE;
    default:
        return HOOK_CONTINUE;
    }
}

}

ModResult initialize() {
    QOL_TRY(register_toggle("no_lock_sound", &no_letterbox));
    QOL_TRY(register_toggle("no_focus_sound", &no_lock_on));
    return mods::hook::add_pre<LockSoundStart>(on_lock_sound_start);
}

ModResult build_panel(ModContext* ctx, UiElementHandle pane) {
    QOL_TRY(add_toggle(ctx, pane, "Mute letterbox (L) sound", no_letterbox));
    return add_toggle(ctx, pane, "Mute lock-on sounds", no_lock_on);
}

}
