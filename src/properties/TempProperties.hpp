#pragma once
#include "IconProperties.hpp"
#include "Enums.hpp"

namespace changing_icons {
    struct CITempProperties {
        int current;
        int index;
        std::vector<IconProperties> iconSet;
        IconOrder order;
        bool useAll;
        bool mirrorEnd;
        bool disabled;
    };
}