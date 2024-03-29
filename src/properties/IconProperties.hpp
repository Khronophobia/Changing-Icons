#pragma once

namespace changing_icons {
    struct IconProperties {
        int iconID;
        std::optional<int> color1 = std::nullopt;
        std::optional<int> color2 = std::nullopt;
    };
}

template<>
struct matjson::Serialize<changing_icons::IconProperties> {
    static changing_icons::IconProperties from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::IconProperties const& value);
    static bool is_json(matjson::Value const& value);
};