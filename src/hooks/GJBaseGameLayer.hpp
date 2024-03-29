#pragma once
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <CIConfigProperties.hpp>

class $modify(CIBaseGameLayer, GJBaseGameLayer) {
    bool m_ciHasInit;
    std::array<changing_icons::CITempProperties, 8> m_player1CIProperties;
    std::array<changing_icons::CITempProperties, 8> m_player2CIProperties;

    void setupLevelStart(LevelSettingsObject* p0);
    changing_icons::CITempProperties setupCIValues(IconType type);
};