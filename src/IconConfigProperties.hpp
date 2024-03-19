#pragma once
#include <matjson/stl_serialize.hpp>

namespace changing_icons {
    enum class IconOrder {
        Random,
        Forward,
        Backward,
    };

    struct IconProperties {
        int iconID;
        std::optional<cocos2d::ccColor3B> color1 = std::nullopt;
        std::optional<cocos2d::ccColor3B> color2 = std::nullopt;
    };

    struct GlobalOverrideData {
        std::optional<bool> useAll = std::nullopt;
        std::optional<IconOrder> order = std::nullopt;
        std::optional<bool> disable = std::nullopt;
    };

    struct GlobalConfigData {
        IconType currentTab = IconType::Cube;
        std::unordered_set<IconType> globalOverrides;
        GlobalOverrideData override;
    };

    struct IconConfigData {
        std::vector<IconProperties> iconSet;
        IconOrder order = IconOrder::Random;
        IconOrder tempOrder = IconOrder::Random;
        bool useAll;
        bool mirrorEnd;
        bool disabled;
    };

    namespace utils {
        template<typename T>
        T tryGetJsonValue(matjson::Value const& value, std::string_view key, T defaultVar) {
            return value.contains(key) ? value[key].as<T>() : defaultVar;
        }
    }
}

template<>
struct matjson::Serialize<IconType> {
    static IconType from_json(matjson::Value const& value) {
        return static_cast<IconType>(value.as_int());
    }
    static matjson::Value to_json(IconType const& value) {
        auto json = matjson::Value();
        json = static_cast<int>(value);
        return json;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_number();
    }
};

template<>
struct matjson::Serialize<changing_icons::IconOrder> {
    static changing_icons::IconOrder from_json(matjson::Value const& value) {
        return static_cast<changing_icons::IconOrder>(value.as_int());
    }
    static matjson::Value to_json(changing_icons::IconOrder const& value) {
        auto json = matjson::Value();
        json = static_cast<int>(value);
        return json;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_number();
    }
};

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
struct matjson::Serialize<changing_icons::GlobalOverrideData> {
    static changing_icons::GlobalOverrideData from_json(matjson::Value const& value) {
        return changing_icons::GlobalOverrideData {
            .useAll = changing_icons::utils::tryGetJsonValue<std::optional<bool>>(
                value,
                "use-all",
                changing_icons::GlobalOverrideData().useAll
            ),
            .order = changing_icons::utils::tryGetJsonValue<std::optional<changing_icons::IconOrder>>(
                value,
                "order",
                changing_icons::GlobalOverrideData().order
            ),
            .disable = changing_icons::utils::tryGetJsonValue<std::optional<bool>>(
                value,
                "disable",
                changing_icons::GlobalOverrideData().disable
            )
        };
    }
    static matjson::Value to_json(changing_icons::GlobalOverrideData const& value) {
        auto obj = matjson::Object();
        obj["use-all"] = value.useAll;
        obj["order"] = value.order;
        obj["disable"] = value.disable;
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
            .currentTab = changing_icons::utils::tryGetJsonValue<IconType>(
                value,
                "current-tab",
                changing_icons::GlobalConfigData().currentTab
            ),
            .globalOverrides = changing_icons::utils::tryGetJsonValue<std::unordered_set<IconType>>(
                value,
                "global-overrides",
                changing_icons::GlobalConfigData().globalOverrides
            ),
            .override = changing_icons::utils::tryGetJsonValue<changing_icons::GlobalOverrideData>(
                value,
                "override",
                changing_icons::GlobalConfigData().override
            )
        };
    }
    static matjson::Value to_json(changing_icons::GlobalConfigData const& value) {
        auto obj = matjson::Object();
        obj["current-tab"] = static_cast<int>(value.currentTab);
        obj["global-overrides"] = value.globalOverrides;
        obj["override"] = value.override;
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
            .order = changing_icons::utils::tryGetJsonValue<changing_icons::IconOrder>(
                value,
                "order",
                changing_icons::IconConfigData().order
            ),
            .useAll = changing_icons::utils::tryGetJsonValue<bool>(
                value,
                "use-all",
                changing_icons::IconConfigData().useAll
            ),
            .mirrorEnd = changing_icons::utils::tryGetJsonValue(
                value,
                "mirror-end",
                changing_icons::IconConfigData().mirrorEnd
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
        obj["order"] = static_cast<int>(value.order);
        obj["use-all"] = value.useAll;
        obj["mirror-end"] = value.mirrorEnd;
        obj["disabled"] = value.disabled;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};