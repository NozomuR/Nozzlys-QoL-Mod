#include "mods/service.hpp"
#include "mods/svc/config.h"
#include "mods/svc/hook.hpp"
#include "mods/svc/log.h"
#include "mods/svc/ui.h"

#include "action_prompt.hpp"
#include "common.hpp"
#include "link_voice.hpp"
#include "lock_sounds.hpp"
#include "lock_range.hpp"
#include "settings_window.hpp"


DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(ConfigService, svc_config);
IMPORT_SERVICE(UiService, svc_ui);

namespace {
constexpr qol::Feature kFeatures[] = {
    {qol::action_prompt::initialize, qol::action_prompt::build_panel},
    {qol::lock_sounds::initialize, qol::lock_sounds::build_panel},
    {qol::link_voice::initialize, qol::settings_window::add_open_button},
    {qol::lock_range::initialize, qol::lock_range::build_panel},
};

ModResult build_panel(ModContext* ctx, UiElementHandle pane, void*, ModError*) {
    for (const auto& feature : kFeatures) {
        QOL_TRY(feature.build_panel(ctx, pane));
    }
    return MOD_OK;
}
}

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError*) {
    for (const auto& feature : kFeatures) {
        QOL_TRY(feature.initialize());
    }

    UiModsPanelDesc panel = UI_MODS_PANEL_DESC_INIT;
    panel.build = build_panel;
    QOL_TRY(svc_ui->register_mods_panel(mod_ctx, &panel));

    svc_log->info(mod_ctx, "Nozzlys random QoL initialized successfully");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
