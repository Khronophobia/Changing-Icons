#pragma once

namespace changing_icons {
    struct IconProperties {
        int iconID;
        std::optional<std::variant<int, cocos2d::ccColor3B>> color1 = std::nullopt;
        std::optional<std::variant<int, cocos2d::ccColor3B>> color2 = std::nullopt;
        bool overrideGlow;
        std::optional<std::variant<int, cocos2d::ccColor3B>> glowColor = std::nullopt;
    };
}

template<>
struct matjson::Serialize<changing_icons::IconProperties> {
    static changing_icons::IconProperties from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::IconProperties const& value);
    static bool is_json(matjson::Value const& value);
};