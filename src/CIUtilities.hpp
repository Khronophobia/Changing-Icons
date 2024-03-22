#pragma once

namespace changing_icons::utils {
    int randomIntInRange(int min, int max);
    
    template<class T>
    T tryGetJsonValue(matjson::Value const& value, std::string_view key, T defaultVar) {
        return value.contains(key) ? value[key].as<T>() : defaultVar;
    }
}