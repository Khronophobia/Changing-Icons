#pragma once
#include <settings/Enums.hpp>
#include <settings/PlayerColor.hpp>

namespace ci::utils {

IconType toIconType(CITab val);
UnlockType toUnlockType(CITab val);
cocos2d::ccColor3B getPlayerColor(PlayerColor const& var);
int getPlayerIcon(IconType type);
int getPlayerIcon(CITab type);

template<typename T>
bool verifyJson(matjson::Value const& value, std::string_view key) {
    return value.contains(key) && value[key].is<T>();
}

// Assign the JSON value to the referenced variable if it exists or is valid
template<typename T>
void tryAssignJson(T& value, matjson::Value const& json) {
    if (json.is<T>()) {
        if constexpr (std::is_move_assignable_v<T>) {
            value = std::move(json.as<T>());
        } else {
            value = json.as<T>();
        }
    }
}
// Assign the JSON value to the referenced variable if it exists or is valid
template<typename T>
void tryAssignJson(T& value, matjson::Value const& json, std::string_view key) {
    if (json.contains(key)) {
        tryAssignJson(value, json[key]);
    }
}

} // namespace ci::utils