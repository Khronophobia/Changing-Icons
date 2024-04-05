#include "Enums.hpp"
#include "ConfigProperties.hpp"
#include "Preset.hpp"
#include "CIUtilities.hpp"
#include <matjson/stl_serialize.hpp>

using namespace changing_icons;

IconType matjson::Serialize<IconType>::from_json(matjson::Value const& value) {
    return static_cast<IconType>(value.as_int());
}

matjson::Value matjson::Serialize<IconType>::to_json(IconType const& value) {
    auto json = matjson::Value();
    json = static_cast<int>(value);
    return json;
}

bool matjson::Serialize<IconType>::is_json(matjson::Value const& value) {
    return value.is_number();
}

IconOrder matjson::Serialize<IconOrder>::from_json(matjson::Value const& value) {
    return static_cast<IconOrder>(value.as_int());
}

matjson::Value matjson::Serialize<IconOrder>::to_json(IconOrder const& value) {
    auto json = matjson::Value();
    json = static_cast<int>(value);
    return json;
}

bool matjson::Serialize<IconOrder>::is_json(matjson::Value const& value) {
    return value.is_number();
}

CIGlobalOverride matjson::Serialize<CIGlobalOverride>::from_json(matjson::Value const& value) {
    return CIGlobalOverride {
        .useAll = utils::tryGetJsonValue<std::optional<bool>>(
            value,
            "use-all",
            CIGlobalOverride().useAll
        ),
        .includePlayerIcon = utils::tryGetJsonValue<std::optional<bool>>(
            value,
            "include-player-icon",
            CIGlobalOverride().includePlayerIcon
        ),
        .order = utils::tryGetJsonValue<std::optional<IconOrder>>(
            value,
            "order",
            CIGlobalOverride().order
        ),
        .mirrorEnd = utils::tryGetJsonValue<std::optional<bool>>(
            value,
            "mirror-end",
            CIGlobalOverride().mirrorEnd
        ),
        .disabled = utils::tryGetJsonValue<std::optional<bool>>(
            value,
            "disabled",
            CIGlobalOverride().disabled
        )
    };
}

matjson::Value matjson::Serialize<CIGlobalOverride>::to_json(CIGlobalOverride const& value) {
    auto obj = matjson::Object();
    obj["use-all"] = value.useAll;
    obj["order"] = value.order;
    obj["include-player-icon"] = value.includePlayerIcon;
    obj["mirror-end"] = value.mirrorEnd;
    obj["disabled"] = value.disabled;
    return obj;
}

bool matjson::Serialize<CIGlobalOverride>::is_json(matjson::Value const& value) {
    return value.is_object();
}

CIGlobalProperties matjson::Serialize<CIGlobalProperties>::from_json(matjson::Value const& value) {
    return CIGlobalProperties {
        .currentTab = utils::tryGetJsonValue<IconType>(
            value,
            "current-tab",
            CIGlobalProperties().currentTab
        ),
        .globalOverrides = utils::tryGetJsonValue<std::unordered_set<IconType>>(
            value,
            "global-overrides",
            CIGlobalProperties().globalOverrides
        ),
        .override = utils::tryGetJsonValue<CIGlobalOverride>(
            value,
            "override",
            CIGlobalProperties().override
        )
    };
}

matjson::Value matjson::Serialize<CIGlobalProperties>::to_json(CIGlobalProperties const& value) {
    auto obj = matjson::Object();
    obj["current-tab"] = static_cast<int>(value.currentTab);
    obj["global-overrides"] = value.globalOverrides;
    obj["override"] = value.override;
    return obj;
}

bool matjson::Serialize<CIGlobalProperties>::is_json(matjson::Value const& value) {
    return value.is_object();
}

IconProperties matjson::Serialize<IconProperties>::from_json(matjson::Value const& value) {
    return IconProperties {
        .iconID = utils::tryGetJsonValue<int>(
            value,
            "ID",
            IconProperties().iconID
        ),
        .color1 = utils::tryGetJsonValue<std::optional<int>>(
            value,
            "color1",
            IconProperties().color1
        ),
        .color2 = utils::tryGetJsonValue<std::optional<int>>(
            value,
            "color2",
            IconProperties().color2
        ),
        .overrideGlow = utils::tryGetJsonValue<bool>(
            value,
            "override-glow",
            IconProperties().overrideGlow
        ),
        .glowColor = utils::tryGetJsonValue<std::optional<int>>(
            value,
            "glow-color",
            IconProperties().color2
        )
    };
}

matjson::Value matjson::Serialize<IconProperties>::to_json(IconProperties const& value) {
    auto obj = matjson::Object();
    obj["ID"] = value.iconID;
    obj["color1"] = value.color1;
    obj["color2"] = value.color2;
    obj["override-glow"] = value.overrideGlow;
    obj["glow-color"] = value.glowColor;
    return obj;
}

bool matjson::Serialize<IconProperties>::is_json(matjson::Value const& value) {
    return value.is_object();
}

CITabProperties matjson::Serialize<CITabProperties>::from_json(matjson::Value const& value) {
    return CITabProperties {
        .iconSet = utils::tryGetJsonValue<std::vector<IconProperties>>(
            value,
            "icon-set",
            CITabProperties().iconSet
        ),
        .order = utils::tryGetJsonValue<IconOrder>(
            value,
            "order",
            CITabProperties().order
        ),
        .useAll = utils::tryGetJsonValue<bool>(
            value,
            "use-all",
            CITabProperties().useAll
        ),
        .includePlayerIcon = utils::tryGetJsonValue<bool>(
            value,
            "include-player-icon",
            CITabProperties().includePlayerIcon
        ),
        .mirrorEnd = utils::tryGetJsonValue(
            value,
            "mirror-end",
            CITabProperties().mirrorEnd
        ),
        .disabled = utils::tryGetJsonValue<bool>(
            value,
            "disabled",
            CITabProperties().disabled
        )
    };
}

matjson::Value matjson::Serialize<CITabProperties>::to_json(CITabProperties const& value) {
    auto obj = matjson::Object();
    obj["icon-set"] = value.iconSet;
    obj["order"] = static_cast<int>(value.order);
    obj["use-all"] = value.useAll;
    obj["include-player-icon"] = value.includePlayerIcon;
    obj["mirror-end"] = value.mirrorEnd;
    obj["disabled"] = value.disabled;
    return obj;
}

bool matjson::Serialize<CITabProperties>::is_json(matjson::Value const& value) {
    return value.is_object();
}

CIPreset matjson::Serialize<CIPreset>::from_json(matjson::Value const& value) {
    return CIPreset{
        .name = utils::tryGetJsonValue(
            value,
            "name",
            CIPreset().name
        ),
        .iconSet = utils::tryGetJsonValue(
            value,
            "icon-set",
            CIPreset().iconSet
        ),
        .formatVersion = utils::tryGetJsonValue(
            value,
            "version",
            CIPreset().formatVersion
        )
    };
}

matjson::Value matjson::Serialize<CIPreset>::to_json(CIPreset const& value) {
    auto obj = matjson::Object();
    obj["name"] = value.name;
    obj["icon-set"] = value.iconSet;
    obj["version"] = value.formatVersion;
    return obj;
}

bool matjson::Serialize<CIPreset>::is_json(matjson::Value const& value) {
    return value.is_object();
}