#pragma once
#include "IconProperties.hpp"

namespace changing_icons {
    struct CIPreset {
        std::string name;
        std::vector<IconProperties> iconSet;
        int formatVersion;
    };
}

template<>
struct matjson::Serialize<changing_icons::CIPreset> {
    static changing_icons::CIPreset from_json(matjson::Value const& value);
    static matjson::Value to_json(changing_icons::CIPreset const& value);
    static bool is_json(matjson::Value const& value);
};