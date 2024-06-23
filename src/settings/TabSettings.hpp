#pragma once
#include "Enums.hpp"
#include "IconSettings.hpp"
#include <matjson/stl_serialize.hpp>

namespace ci {

struct TabSettings {
    IconOrder iconOrder;
    bool useAllIcons;
    bool disabled;
    std::vector<IconSettings> iconSet;
};

} // namespace ci

namespace matjson {

template<>
struct Serialize<ci::TabSettings> {
    static ci::TabSettings from_json(Value const& value) {
        ci::TabSettings ret{};
        ci::utils::tryAssignJson(ret.iconOrder, value, "order");
        ci::utils::tryAssignJson(ret.useAllIcons, value, "use-all");
        ci::utils::tryAssignJson(ret.disabled, value, "disabled");
        ci::utils::tryAssignJson(ret.iconSet, value, "icon-set");

        return ret;
    }
    static Value to_json(ci::TabSettings const& value) {
        return Object{
            {"order", value.iconOrder},
            {"use-all", value.useAllIcons},
            {"disabled", value.disabled},
            {"icon-set", value.iconSet}
        };
    }
    static bool is_json(Value const& value) {
        return ci::utils::verifyJson<ci::IconOrder>(value, "order") &&
            ci::utils::verifyJson<bool>(value, "use-all") &&
            ci::utils::verifyJson<bool>(value, "disabled") &&
            ci::utils::verifyJson<std::vector<ci::IconSettings>>(value, "icon-set");
    }
};

} // namespace matjson