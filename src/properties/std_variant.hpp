#pragma once

template<typename T>
concept ValidJson = requires(matjson::Value const& value) {
    value.as<T>();
};

template<ValidJson T, ValidJson R>
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

template<ValidJson... Types>
struct matjson::Serialize<std::variant<Types...>> {
private:
    using Variant = std::variant<Types...>;

    template<size_t N>
    using Type = std::variant_alternative_t<N, Variant>;
    
    static auto constexpr lastIndex = std::variant_size_v<Variant> - 1;

    template<size_t N>
    static std::variant<Types...> __from_json(matjson::Value const& value) {
        using CurrentType = Type<N>;
        
        if (value.is<CurrentType>()) {
            return value.as<CurrentType>();
        }
        
        return __from_json<N + 1>(value);
    }
    template<>
    static std::variant<Types...> __from_json<lastIndex>(matjson::Value const& value) {
        using CurrentType = Type<lastIndex>;

        return value.as<CurrentType>();
    }

    template<size_t N>
    static matjson::Value __to_json(Variant const& value) {
        using CurrentType = Type<N>;

        if (std::holds_alternative<CurrentType>(value)) {
            return std::get<CurrentType>(value);
        }
        
        return __to_json<N + 1>(value);
    }
    template<>
    static matjson::Value __to_json<lastIndex>(Variant const& value) {
        using CurrentType = Type<lastIndex>;

        return std::get<CurrentType>(value);
    }
public:
    static std::variant<Types...> from_json(matjson::Value const& value) {
        return __from_json<0>(value);
    }
    static matjson::Value to_json(std::variant<Types...> const& value) {
        return __to_json<0>(value);
    }
    static bool is_json(matjson::Value const& value) {
        return (value.is<Types> || ...);
    }
};