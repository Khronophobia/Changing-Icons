#pragma once

// Idk if there's a function to get the color count
namespace changing_icons::constants {
    inline float constexpr ICONCELL_WIDTH = 160.f;
    inline float constexpr ICONCELL_HEIGHT = 30.f;
    inline int constexpr COLOR_COUNT = 106;
    inline int constexpr ICONS_PER_PAGE = 55;
    inline std::array<std::string, 17> constexpr ILLEGAL_CHARS{
        "/", "\\", ":", "*", "?", "\"", "<", ">", "|", "@",
        "$", "%",  "^", "&", ".", ";",  "="
    };
}