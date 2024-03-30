#pragma once
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <properties/TempProperties.hpp>

class $modify(CIBaseGameLayer, GJBaseGameLayer) {
    bool m_ciHasInit;
    std::unordered_map<IconType, changing_icons::CITempProperties> m_player1CIProperties;
    std::unordered_map<IconType, changing_icons::CITempProperties> m_player2CIProperties;

    void setupLevelStart(LevelSettingsObject* p0);
    changing_icons::CITempProperties setupCIValues(IconType type);
};