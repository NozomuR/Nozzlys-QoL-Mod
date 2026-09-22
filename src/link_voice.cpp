#include "link_voice.hpp"

#include "common.hpp"

#include "mods/svc/hook.hpp"

#include <span>

#include "Z2AudioLib/Z2LinkMgr.h"
#include "Z2AudioLib/Z2SeMgr.h"
#include "Z2AudioLib/Z2SoundHandles.h"

DEFINE_HOOK_SYMBOL("Z2CreatureLink::startLinkVoice",
                   Z2SoundHandlePool*(Z2CreatureLink*, JAISoundID, s8), LinkVoiceStart);

namespace qol::link_voice {
namespace {

struct VoiceToggle {
    u32 sound_id;
    const char* var_name;
    const char* label;
    ConfigVarHandle var;
};

VoiceToggle movement[] = {
    {Z2SE_AL_V_BACKTEN, "voice_roll", "Roll and backflip", 0},
    {Z2SE_AL_V_SOTOMO_ROLL, "voice_guard_roll", "Shield roll", 0},
    {Z2SE_AL_V_JUMP_S, "voice_hop", "Side hop and short jump", 0},
};

VoiceToggle attacks[] = {
    {Z2SE_AL_V_ATTACK_S, "voice_swing_light", "Light swing", 0},
    {Z2SE_AL_V_ATTACK_M, "voice_swing_medium", "Medium swing", 0},
    {Z2SE_AL_V_ATTACK_L, "voice_swing_heavy", "Heavy swing", 0},
    {Z2SE_AL_V_ATTACK_RUN, "voice_swing_run", "Running slash", 0},
    {Z2SE_AL_V_KAITEN, "voice_swing_spin", "Spin attack", 0},
};

const std::span<VoiceToggle> groups[] = {movement, attacks};

const VoiceToggle* find(u32 sound_id) {
    for (const auto& group : groups) {
        for (const auto& toggle : group) {
            if (toggle.sound_id == sound_id) {
                return &toggle;
            }
        }
    }
    return nullptr;
}

HookAction on_link_voice(ModContext*, void* args, void* result, void*) {
    const VoiceToggle* toggle = find(mods::arg<JAISoundID>(args, 1));
    if (toggle == nullptr || !enabled(toggle->var)) {
        return HOOK_CONTINUE;
    }
    *static_cast<Z2SoundHandlePool**>(result) = nullptr;
    return HOOK_SKIP_ORIGINAL;
}

template <std::size_t N>
ModResult build_group(ModContext* ctx, UiElementHandle pane, VoiceToggle (&group)[N]) {
    for (const auto& toggle : group) {
        QOL_TRY(add_toggle(ctx, pane, toggle.label, toggle.var));
    }
    return MOD_OK;
}

ModResult build_movement(ModContext* ctx, UiElementHandle pane, void*, ModError*) {
    return build_group(ctx, pane, movement);
}

ModResult build_attacks(ModContext* ctx, UiElementHandle pane, void*, ModError*) {
    return build_group(ctx, pane, attacks);
}

}

ModResult initialize() {
    for (const auto& group : groups) {
        for (auto& toggle : group) {
            QOL_TRY(register_toggle(toggle.var_name, &toggle.var));
        }
    }
    return mods::hook::add_pre<LinkVoiceStart>(on_link_voice);
}

ModResult build_submenus(ModContext* ctx, UiElementHandle group_pane, UiElementHandle target_pane) {
    UiGroupDesc desc = UI_GROUP_DESC_INIT;
    desc.label = "Movement voice clips";
    desc.build = build_movement;
    QOL_TRY(svc_ui->pane_add_group(ctx, group_pane, target_pane, &desc, nullptr));
    desc.label = "Sword swing voice clips";
    desc.build = build_attacks;
    return svc_ui->pane_add_group(ctx, group_pane, target_pane, &desc, nullptr);
}

}
