#include "CIUtilities.hpp"

fmt::appender fmt::formatter<IconType>::format(IconType type, format_context& ctx) const {
    switch(type) {
        default: return formatter<string_view>::format("Unknown", ctx);
        case IconType::Cube: return formatter<string_view>::format("Cube", ctx);
        case IconType::Ship: return formatter<string_view>::format("Ship", ctx);
        case IconType::Ball: return formatter<string_view>::format("Ball", ctx);
        case IconType::Ufo: return formatter<string_view>::format("UFO", ctx);
        case IconType::Wave: return formatter<string_view>::format("Wave", ctx);
        case IconType::Robot: return formatter<string_view>::format("Robot", ctx);
        case IconType::Spider: return formatter<string_view>::format("Spider", ctx);
        case IconType::Swing: return formatter<string_view>::format("Swing", ctx);
        case IconType::Jetpack: return formatter<string_view>::format("Jetpack", ctx);
    }
}