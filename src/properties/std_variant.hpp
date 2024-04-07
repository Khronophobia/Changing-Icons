#pragma once

template<class T, class R>
struct matjson::Serialize<std::variant<T, R>> {
    static std::variant<T, R> from_json(matjson::Value const& value) {
        if (value.is<T>()) return value.as<T>();
        if (value.is<R>()) return value.as<R>();

        return std::variant<T, R>();
    }
    static matjson::Value to_json(std::variant<T, R> const& value) {
        if (std::holds_alternative<T>(value))
            return std::get<T>(value);
        
        return std::get<R>(value);
    }
    static bool is_json(matjson::Value const& value) {
        return value.is<T>() || value.is<R>();
    }
};