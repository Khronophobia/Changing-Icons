#pragma once
#include "Enums.hpp"
#include "IconProperties.hpp"

namespace changing_icons {
    enum class IconOrder;

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
        IconOrder order = IconOrder::Shuffle;
        bool useAll;
        bool includePlayerIcon;
        bool mirrorEnd;
        bool disabled;
    };
}

template<>
struct matjson::Serialize<changing_icons::CIGlobalOverride> {
    static changing_icons::CIGlobalOverride from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::CIGlobalOverride const& value);
    static bool is_json(matjson::Value const& value);
};

template<>
struct matjson::Serialize<changing_icons::CIGlobalProperties> {
    static changing_icons::CIGlobalProperties from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::CIGlobalProperties const& value);
    static bool is_json(matjson::Value const& value);
};

template<>
struct matjson::Serialize<changing_icons::CITabProperties> {
    static changing_icons::CITabProperties from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::CITabProperties const& value);
    static bool is_json(matjson::Value const& value);
};