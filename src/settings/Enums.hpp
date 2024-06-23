#pragma once

namespace ci {

enum class CITab {
    Cube,
    Ship,
    Ball,
    UFO,
    Wave,
    Robot,
    Spider,
    Swing,
    Jetpack
};

enum class IconOrder {
    Shuffle,
    Forward,
    Backward,
    Random,
};

enum class SwitchType {
    EnterPortal,
    AttemptStart
};

} // namespace ci

namespace matjson {

template<typename T>
requires (std::is_enum_v<T>)
struct Serialize<T> {
    static T from_json(Value const& value) {
        return static_cast<T>(value.as_int());
    }
    static Value to_json(T const& value) {
        return static_cast<int>(value);
    }
    static bool is_json(Value const& value) {
        return value.is_number();
    }
};

} // namespace matjson