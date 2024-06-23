#pragma once
#include "Enums.hpp"
#include <CIUtilities.hpp>
#include <class/tribool.hpp>

namespace ci {

struct GlobalOverrides {
    std::optional<IconOrder> iconOrder;
    khronos::tribool useAllIcons;
    khronos::tribool disabled;
};

struct GlobalSettings {
    CITab currentTab;
    SwitchType switchType;
    bool flipP2Colors;
    std::unordered_set<CITab> globalOverrides;
    GlobalOverrides overrideSettings;
};

} // namespace ci

namespace matjson {

template<>
struct Serialize<ci::GlobalOverrides> {
    static ci::GlobalOverrides from_json(Value const& value) {
        ci::GlobalOverrides ret{};
        ci::utils::tryAssignJson(ret.iconOrder, value, "order");
        ci::utils::tryAssignJson(ret.useAllIcons, value, "use-all");
        ci::utils::tryAssignJson(ret.disabled, value, "disabled");

        return ret;
    }
    static Value to_json(ci::GlobalOverrides const& value) {
        return Object{
            {"order", value.iconOrder},
            {"use-all", value.useAllIcons},
            {"disabled", value.disabled}
        };
    }
    static bool is_json(Value const& value) {
        return ci::utils::verifyJson<std::optional<ci::IconOrder>>(value, "order") &&
            ci::utils::verifyJson<khronos::tribool>(value, "use-all") &&
            ci::utils::verifyJson<khronos::tribool>(value, "disabled");
    }
};

template<>
struct Serialize<ci::GlobalSettings> {
    static ci::GlobalSettings from_json(Value const& value) {
        ci::GlobalSettings ret{};
        ci::utils::tryAssignJson(ret.currentTab, value, "current-tab");
        ci::utils::tryAssignJson(ret.switchType, value, "switch-type");
        ci::utils::tryAssignJson(ret.flipP2Colors, value, "flip-p2-colors");
        ci::utils::tryAssignJson(ret.globalOverrides, value, "global-overrides");
        ci::utils::tryAssignJson(ret.overrideSettings, value, "override");

        return ret;
    }
    static Value to_json(ci::GlobalSettings const& value) {
        return Object{
            {"current-tab", value.currentTab},
            {"switch-type", value.switchType},
            {"flip-p2-colors", value.flipP2Colors},
            {"global-overrides", value.globalOverrides},
            {"override", value.overrideSettings}
        };
    }
    static bool is_json(Value const& value) {
        return ci::utils::verifyJson<ci::CITab>(value, "current-tab") &&
            ci::utils::verifyJson<ci::SwitchType>(value, "switch-type") &&
            ci::utils::verifyJson<bool>(value, "flip-p2-colors") &&
            ci::utils::verifyJson<std::unordered_set<ci::CITab>>(value, "global-overrides") &&
            ci::utils::verifyJson<ci::GlobalOverrides>(value, "override");
    }
};

} // namespace matjson