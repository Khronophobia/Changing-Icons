#pragma once

namespace khronos {

struct tribool {
    enum class state : uint8_t {
        false_t = false,
        true_t = true,
        null_t = 0b10
    };

    constexpr tribool() noexcept
        : _value(state::false_t){}
    constexpr tribool(bool value) noexcept
        : _value(static_cast<state>(value)) {}
    constexpr tribool(std::nullptr_t) noexcept
        : _value(state::null_t) {}
    constexpr tribool(std::nullopt_t) noexcept
        : _value(state::null_t) {}
    
    constexpr bool is_null() const noexcept;
    constexpr bool to_bool() const noexcept;
    constexpr bool to_nbool() const noexcept;

    tribool& operator=(bool value) noexcept;
    tribool& operator=(std::nullptr_t) noexcept;
    tribool& operator=(std::nullopt_t) noexcept;

    constexpr explicit operator bool() const noexcept;
    constexpr bool operator!() const noexcept;

    state _value;
};

constexpr bool tribool::is_null() const noexcept {
    return _value == state::null_t;
}
constexpr bool tribool::to_bool() const noexcept {
    return is_null() ? false : static_cast<bool>(_value);
}
constexpr bool tribool::to_nbool() const noexcept {
    return is_null() ? false : !static_cast<bool>(_value);
}

inline tribool& tribool::operator=(bool value) noexcept {
    _value = static_cast<state>(value);
    return *this;
}
inline tribool& tribool::operator=(std::nullptr_t) noexcept {
    _value = state::null_t;
    return *this;
}
inline tribool& tribool::operator=(std::nullopt_t) noexcept {
    _value = state::null_t;
    return *this;
}

constexpr tribool::operator bool() const noexcept {
    return to_bool();
}
constexpr bool tribool::operator!() const noexcept {
    return to_nbool();
}

} // namespace khronos

namespace matjson {

template<>
struct Serialize<khronos::tribool> {
    static khronos::tribool from_json(Value const& value) {
        if (value.is_null()) return nullptr;
        
        return value.as_bool();
    }
    static matjson::Value to_json(khronos::tribool const& value) {
        if (value.is_null()) return nullptr;

        return value.to_bool();
    }
    static bool is_json(Value const& value) {
        return value.is_bool() || value.is_null();
    }
};

} // namespace matjson

namespace fmt {
    template<>
    struct formatter<khronos::tribool> : formatter<char const*> {
        auto format(khronos::tribool const& value, format_context& ctx) -> decltype(ctx.out()) {
            switch (value._value) {
            case khronos::tribool::state::null_t:
                return formatter<char const*>::format("null", ctx);
            case khronos::tribool::state::false_t:
                return formatter<char const*>::format("false", ctx);
            case khronos::tribool::state::true_t:
                return formatter<char const*>::format("true", ctx);
            }
        }
    };
} // namespace fmt