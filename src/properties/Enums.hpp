#pragma once

namespace changing_icons {
    enum class IconOrder {
        Random,
        Forward,
        Backward,
    };
}

template<>
struct matjson::Serialize<IconType> {
    static IconType from_json(matjson::Value const& value);
    static matjson::Value to_json(IconType const& value);
    static bool is_json(matjson::Value const& value);
};

template<>
struct matjson::Serialize<changing_icons::IconOrder> {
    static changing_icons::IconOrder from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::IconOrder const& value);
    static bool is_json(matjson::Value const& value);
};