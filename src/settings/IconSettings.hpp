#pragma once
#include "PlayerColor.hpp"
#include "variant_serialization.hpp"
#include <CIUtilities.hpp>

namespace ci {

struct IconSettings {
    int iconID{1};
    bool isPlayerIcon;
    std::optional<PlayerColor> primaryColor;
    std::optional<PlayerColor> secondaryColor;
    bool overrideGlow;
    std::optional<PlayerColor> glowColor;
};

} // namespace ci

namespace matjson {

template<>
struct Serialize<ci::IconSettings> {
    static ci::IconSettings from_json(Value const& value) {
        ci::IconSettings ret{};
        ci::utils::tryAssignJson(ret.iconID, value, "ID");
        ci::utils::tryAssignJson(ret.isPlayerIcon, "player-icon");
        ci::utils::tryAssignJson(ret.primaryColor, value, "color1");
        ci::utils::tryAssignJson(ret.secondaryColor, value, "color2");
        ci::utils::tryAssignJson(ret.overrideGlow, value, "override-glow");
        ci::utils::tryAssignJson(ret.glowColor, value, "glow-color");

        return ret;
    }
    static Value to_json(ci::IconSettings const& value) {
        return Object{
            {"ID", value.iconID},
            {"player-icon", value.isPlayerIcon},
            {"color1", value.primaryColor},
            {"color2", value.secondaryColor},
            {"override-glow", value.overrideGlow},
            {"glow-color", value.glowColor}
        };
    }
    static bool is_json(Value const& value) {
        return ci::utils::verifyJson<int>(value, "ID") &&
            ci::utils::verifyJson<bool>(value, "player-icon") &&
            ci::utils::verifyJson<std::optional<ci::PlayerColor>>(value, "color1") &&
            ci::utils::verifyJson<std::optional<ci::PlayerColor>>(value, "color2") &&
            ci::utils::verifyJson<bool>(value, "override-glow") &&
            ci::utils::verifyJson<std::optional<ci::PlayerColor>>(value, "glow-color");
    }
};

} // namespace matjson