#pragma once

template<class T, class R>
struct matjson::Serialize<std::variant<T, R>> {
    static std::variant<T, R> from_json(matjson::Value const& value) {
        if (value.is<T>()) return value.as<T>();

        return value.as<R>();
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

template<class... Types>
struct matjson::Serialize<std::variant<Types...>> {
private:
    using Variant = std::variant<Types...>;

    template<size_t N>
    static std::variant<Types...> to_json_loop(matjson::Value const& value) {
        using CurrentType = std::variant_alternative_t<N, Variant>;
        
        if (value.is<CurrentType>()) {
            return value.as<CurrentType>();
        }
        
        return to_json_loop<N + 1>(value);
    }
    template<>
    static std::variant<Types...> to_json_loop<std::variant_size_v<Variant> - 1>(matjson::Value const& value) {
        using CurrentType = std::variant_alternative_t<std::variant_size_v<Variant> - 1, Variant>;

        return value.as<CurrentType>();
    }

    template<size_t N>
    static matjson::Value to_json_loop(Variant const& value) {
        using CurrentType = std::variant_alternative_t<N, Variant>;

        if (std::holds_alternative<CurrentType>(value)) {
            return std::get<CurrentType>(value);
        }
        
        return to_json_loop<N + 1>(value);
    }
    template<>
    static matjson::Value to_json_loop<std::variant_size_v<Variant> - 1>(Variant const& value) {
        using CurrentType = std::variant_alternative_t<std::variant_size_v<Variant> - 1, Variant>;

        return std::get<CurrentType>(value);
    }

    template<size_t N>
    static bool is_json_loop(matjson::Value const& value) {
        using CurrentType = std::variant_alternative_t<N, Variant>;

        return (value.is<CurrentType>()) || is_json_loop<N + 1>(value);
    }
    template<>
    static bool is_json_loop<std::variant_size_v<Variant> - 1>(matjson::Value const& value) {
        using CurrentType = std::variant_alternative_t<std::variant_size_v<Variant> - 1, Variant>;

        return (value.is<CurrentType>());
    }
public:
    static std::variant<Types...> from_json(matjson::Value const& value) {
        return to_json_loop<0>(value);
    }
    static matjson::Value to_json(std::variant<Types...> const& value) {
        return to_json_loop<0>(value);
    }
    static bool is_json(matjson::Value const& value) {
        return is_json_loop<0>(value);
    }
};