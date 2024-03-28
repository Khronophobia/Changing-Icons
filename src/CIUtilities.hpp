#pragma once

namespace changing_icons::utils {
    template<class T>
    T tryGetJsonValue(matjson::Value const& value, std::string_view key, T defaultVar) {
        return value.contains(key) ? value[key].as<T>() : defaultVar;
    }
}

template<>
struct fmt::formatter<IconType> : formatter<string_view> {
    fmt::appender format(IconType type, format_context& ctx) const;
};