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
        IconType currentTab = IconType::Cube;
        IconOrder order = IconOrder::Random;
    };

    struct IconConfigData {
        std::vector<IconProperties> iconSet;
        bool random;
        bool disabled;
    };

    namespace utils {
        template<typename T>
        T tryGetJsonValue(matjson::Value const& value, std::string_view key, T defaultVar) {
            return value.contains(key) ? value[key].as<T>() : defaultVar;
        }
        template<typename T, typename A>
        T tryGetJsonValue(matjson::Value const& value, std::string_view key, T defaultVar) {
            return value.contains(key) ? static_cast<T>(value[key].as<A>()) : defaultVar;
        }
    }
}

template<>
struct matjson::Serialize<changing_icons::IconProperties> {
    static changing_icons::IconProperties from_json(matjson::Value const& value) {
        return changing_icons::IconProperties {
            .iconID = changing_icons::utils::tryGetJsonValue<int>(
                value,
                "ID",
                changing_icons::IconProperties().iconID
            ),
            .color1 = changing_icons::utils::tryGetJsonValue<std::optional<cocos2d::ccColor3B>>(
                value,
                "color1",
                changing_icons::IconProperties().color1
            ),
            .color2 = changing_icons::utils::tryGetJsonValue<std::optional<cocos2d::ccColor3B>>(
                value,
                "color2",
                changing_icons::IconProperties().color2
            )
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
            .currentTab = changing_icons::utils::tryGetJsonValue<IconType, int>(
                value,
                "current-tab",
                changing_icons::GlobalConfigData().currentTab
            ),
            .order = changing_icons::utils::tryGetJsonValue<changing_icons::IconOrder, int>(
                value,
                "icon-order",
                changing_icons::GlobalConfigData().order
            )
        };
    }
    static matjson::Value to_json(changing_icons::GlobalConfigData const& value) {
        auto obj = matjson::Object();
        obj["current-tab"] = static_cast<int>(value.currentTab);
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
            .iconSet = changing_icons::utils::tryGetJsonValue<std::vector<changing_icons::IconProperties>>(
                value,
                "icon-set",
                changing_icons::IconConfigData().iconSet
            ),
            .random = changing_icons::utils::tryGetJsonValue<bool>(
                value,
                "random",
                changing_icons::IconConfigData().random
            ),
            .disabled = changing_icons::utils::tryGetJsonValue<bool>(
                value,
                "disabled",
                changing_icons::IconConfigData().disabled
            )
        };
    }
    static matjson::Value to_json(changing_icons::IconConfigData const& value) {
        auto obj = matjson::Object();
        obj["icon-set"] = value.iconSet;
        obj["random"] = value.random;
        obj["disabled"] = value.disabled;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};