#pragma once
#include <properties/Preset.hpp>

namespace changing_icons {
    class IconCellDelegate {
    public:
        virtual void editIcon(int index) = 0;
        virtual void swapIcons(int index1, int index2) = 0;
        virtual void deleteIcon(int index) = 0;
    };
}