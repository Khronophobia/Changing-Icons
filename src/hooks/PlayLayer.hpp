#include <Geode/modify/PlayLayer.hpp>
#include <properties/TempProperties.hpp>

class $modify(CIPlayLayer, PlayLayer) {
    $override
    void setupHasCompleted();

    std::pair<IconType, changing_icons::CITempProperties> setupCIValues(IconType type);
};