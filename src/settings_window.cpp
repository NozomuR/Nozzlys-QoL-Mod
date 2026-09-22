#include "settings_window.hpp"

#include "common.hpp"
#include "link_voice.hpp"

namespace qol::settings_window {
namespace {

UiWindowHandle window = 0;

ModResult build_voices_tab(ModContext* ctx, UiWindowHandle, UiElementHandle left,
                           UiElementHandle right, void*, ModError*) {
    return link_voice::build_submenus(ctx, left, right);
}

void on_closed(ModContext*, UiWindowHandle, void*) {
    window = 0;
}

void open(ModContext* ctx, void*) {
    if (window != 0) {
        return;
    }
    UiTabDesc tabs[1] = {UI_TAB_DESC_INIT};
    tabs[0].title = "Link voices";
    tabs[0].build = build_voices_tab;

    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 1;
    desc.on_closed = on_closed;
    if (svc_ui->window_push(ctx, &desc, &window) != MOD_OK) {
        window = 0;
    }
}

}

ModResult add_open_button(ModContext* ctx, UiElementHandle pane) {
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_BUTTON;
    desc.label = "Voice clip settings";
    desc.on_pressed = open;
    return svc_ui->pane_add_control(ctx, pane, &desc, nullptr);
}

}
