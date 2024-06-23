#pragma once

namespace matjson {

template<typename A, typename B>
requires requires(Value const& value) {
    value.as<A>();
    value.as<B>();
}
struct Serialize<std::variant<A, B>> {
private:
    using variant = std::variant<A, B>;
public:
    static variant from_json(Value const& value) {
        if (value.is<A>()) {
            return value.as<A>();
        }

        return value.as<B>();
    }
    static Value to_json(variant const& value) {
        /* if (std::holds_alternative<A>(value)) {
            return std::get<A>(value);
        }
        return std::get<B>(value); */

        return std::visit([](auto const& arg) -> Value {
            return arg;
        }, value);
    }
    static bool is_json(Value const& value) {
        return value.is<A>() || value.is<B>();
    }
};

} // namespace matjson