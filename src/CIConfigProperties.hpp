#pragma once
#include <matjson/stl_serialize.hpp>
#include "CIUtilities.hpp"

namespace changing_icons {
    enum class IconOrder {
        Random,
        Forward,
        Backward,
    };

    struct IconProperties {
        int iconID;
        std::optional<int> color1 = std::nullopt;
        std::optional<int> color2 = std::nullopt;
    };

    struct CIGlobalOverride {
        std::optional<bool> useAll = std::nullopt;
        std::optional<bool> includePlayerIcon = std::nullopt;
        std::optional<IconOrder> order = std::nullopt;
        std::optional<bool> mirrorEnd = std::nullopt;
        std::optional<bool> disabled = std::nullopt;
    };

    struct CIGlobalProperties {
        IconType currentTab = IconType::Cube;
        std::unordered_set<IconType> globalOverrides;
        CIGlobalOverride override;
    };

    struct CITabProperties {
        std::vector<IconProperties> iconSet;
        IconOrder order = IconOrder::Random;
        bool useAll;
        bool includePlayerIcon;
        bool mirrorEnd;
        bool disabled;
    };

    struct CITempProperties {
        int current;
        int index;
        std::vector<IconProperties> iconSet;
        IconOrder order;
        bool useAll;
        bool mirrorEnd;
        bool disabled;
    };

    struct CIPreset {
        std::string name;
        std::vector<IconProperties> iconSet;
        int formatVersion;
    };
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
            .color1 = changing_icons::utils::tryGetJsonValue<std::optional<int>>(
                value,
                "color1",
                changing_icons::IconProperties().color1
            ),
            .color2 = changing_icons::utils::tryGetJsonValue<std::optional<int>>(
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
struct matjson::Serialize<changing_icons::CIGlobalOverride> {
    static changing_icons::CIGlobalOverride from_json(matjson::Value const& value) {
        return changing_icons::CIGlobalOverride {
            .useAll = changing_icons::utils::tryGetJsonValue<std::optional<bool>>(
                value,
                "use-all",
                changing_icons::CIGlobalOverride().useAll
            ),
            .includePlayerIcon = changing_icons::utils::tryGetJsonValue<std::optional<bool>>(
                value,
                "include-player-icon",
                changing_icons::CIGlobalOverride().includePlayerIcon
            ),
            .order = changing_icons::utils::tryGetJsonValue<std::optional<changing_icons::IconOrder>>(
                value,
                "order",
                changing_icons::CIGlobalOverride().order
            ),
            .mirrorEnd = changing_icons::utils::tryGetJsonValue<std::optional<bool>>(
                value,
                "mirror-end",
                changing_icons::CIGlobalOverride().mirrorEnd
            ),
            .disabled = changing_icons::utils::tryGetJsonValue<std::optional<bool>>(
                value,
                "disabled",
                changing_icons::CIGlobalOverride().disabled
            )
        };
    }
    static matjson::Value to_json(changing_icons::CIGlobalOverride const& value) {
        auto obj = matjson::Object();
        obj["use-all"] = value.useAll;
        obj["include-player-icon"] = value.includePlayerIcon;
        obj["order"] = value.order;
        obj["mirror-end"] = value.mirrorEnd;
        obj["disabled"] = value.disabled;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};

template<>
struct matjson::Serialize<changing_icons::CIGlobalProperties> {
    static changing_icons::CIGlobalProperties from_json(matjson::Value const& value) {
        return changing_icons::CIGlobalProperties {
            .currentTab = changing_icons::utils::tryGetJsonValue<IconType>(
                value,
                "current-tab",
                changing_icons::CIGlobalProperties().currentTab
            ),
            .globalOverrides = changing_icons::utils::tryGetJsonValue<std::unordered_set<IconType>>(
                value,
                "global-overrides",
                changing_icons::CIGlobalProperties().globalOverrides
            ),
            .override = changing_icons::utils::tryGetJsonValue<changing_icons::CIGlobalOverride>(
                value,
                "override",
                changing_icons::CIGlobalProperties().override
            )
        };
    }
    static matjson::Value to_json(changing_icons::CIGlobalProperties const& value) {
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
struct matjson::Serialize<changing_icons::CITabProperties> {
    static changing_icons::CITabProperties from_json(matjson::Value const& value) {
        return changing_icons::CITabProperties {
            .iconSet = changing_icons::utils::tryGetJsonValue<std::vector<changing_icons::IconProperties>>(
                value,
                "icon-set",
                changing_icons::CITabProperties().iconSet
            ),
            .order = changing_icons::utils::tryGetJsonValue<changing_icons::IconOrder>(
                value,
                "order",
                changing_icons::CITabProperties().order
            ),
            .useAll = changing_icons::utils::tryGetJsonValue<bool>(
                value,
                "use-all",
                changing_icons::CITabProperties().useAll
            ),
            .includePlayerIcon = changing_icons::utils::tryGetJsonValue<bool>(
                value,
                "include-player-icon",
                changing_icons::CITabProperties().includePlayerIcon
            ),
            .mirrorEnd = changing_icons::utils::tryGetJsonValue(
                value,
                "mirror-end",
                changing_icons::CITabProperties().mirrorEnd
            ),
            .disabled = changing_icons::utils::tryGetJsonValue<bool>(
                value,
                "disabled",
                changing_icons::CITabProperties().disabled
            )
        };
    }
    static matjson::Value to_json(changing_icons::CITabProperties const& value) {
        auto obj = matjson::Object();
        obj["icon-set"] = value.iconSet;
        obj["order"] = static_cast<int>(value.order);
        obj["use-all"] = value.useAll;
        obj["include-player-icon"] = value.includePlayerIcon;
        obj["mirror-end"] = value.mirrorEnd;
        obj["disabled"] = value.disabled;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};

template<>
struct matjson::Serialize<changing_icons::CIPreset> {
    static changing_icons::CIPreset from_json(matjson::Value const& value) {
        return changing_icons::CIPreset{
            .name = changing_icons::utils::tryGetJsonValue(
                value,
                "name",
                changing_icons::CIPreset().name
            ),
            .iconSet = changing_icons::utils::tryGetJsonValue(
                value,
                "icon-set",
                changing_icons::CIPreset().iconSet
            ),
            .formatVersion = changing_icons::utils::tryGetJsonValue(
                value,
                "version",
                changing_icons::CIPreset().formatVersion
            )
        };
    }
    static matjson::Value to_json(changing_icons::CIPreset const& value) {
        auto obj = matjson::Object();
        obj["name"] = value.name;
        obj["icon-set"] = value.iconSet;
        obj["version"] = value.formatVersion;
        return obj;
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_object();
    }
};