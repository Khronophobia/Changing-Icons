#pragma once
#include <matjson/stl_serialize.hpp>

namespace changing_icons {
    enum class IconOrder {
        Random,
        Down,
        Up,
    };

    struct IconProperties {
        int iconID;
        std::optional<cocos2d::ccColor3B> color1 = std::nullopt;
        std::optional<cocos2d::ccColor3B> color2 = std::nullopt;
    };

    struct GlobalConfigData {
        IconOrder order = IconOrder::Random;
    };

    struct IconConfigData {
        std::vector<IconProperties> iconSet;
        bool random;
    };
}

template<>
struct matjson::Serialize<changing_icons::IconProperties> {
    static changing_icons::IconProperties from_json(matjson::Value const& value) {
        return changing_icons::IconProperties {
            .iconID = value["ID"].as_int(),
            .color1 = value["color1"].as<std::optional<cocos2d::ccColor3B>>(),
            .color2 = value["color2"].as<std::optional<cocos2d::ccColor3B>>()
        };
    }
    static matjson::Value to_json(changing_icons::IconProperties const& value) {
        auto obj = matjson::Object();
        obj["ID"] = value.iconID;
        obj["color1"] = value.color1;
        obj["color2"] = value.color2;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};

template<>
struct matjson::Serialize<changing_icons::GlobalConfigData> {
    static changing_icons::GlobalConfigData from_json(matjson::Value const& value) {
        return changing_icons::GlobalConfigData {
            .order = static_cast<changing_icons::IconOrder>(value["icon-order"].as_int())
        };
    }
    static matjson::Value to_json(changing_icons::GlobalConfigData const& value) {
        auto obj = matjson::Object();
        obj["icon-order"] = static_cast<int>(value.order);
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};

template<>
struct matjson::Serialize<changing_icons::IconConfigData> {
    static changing_icons::IconConfigData from_json(matjson::Value const& value) {
        return changing_icons::IconConfigData {
            .iconSet = value["icon-set"].as<std::vector<changing_icons::IconProperties>>(),
            .random = value["random"].as_bool()
        };
    }
    static matjson::Value to_json(changing_icons::IconConfigData const& value) {
        auto obj = matjson::Object();
        obj["icon-set"] = value.iconSet;
        obj["random"] = value.random;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};