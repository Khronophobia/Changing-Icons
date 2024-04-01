#pragma once
#include <Geode/modify/PlayerObject.hpp>
#include <properties/TempProperties.hpp>

namespace changing_icons {
    struct CITempProperties;
}

class $modify(CIPlayerObject, PlayerObject) {
    bool m_levelStarted;
    bool m_ciHasInit;
    std::unordered_map<IconType, changing_icons::CITempProperties> m_ciProperties;
    inline static changing_icons::CITempProperties m_emptyCIProperty = changing_icons::CITempProperties();
    int m_ogColor1 = GameManager::get()->getPlayerColor();
    int m_ogColor2 = GameManager::get()->getPlayerColor2();
    int m_ogGlowColor = GameManager::get()->getPlayerGlowColor();
    bool m_ogHasGlow;
    cocos2d::ccColor3B m_currentColor;
    cocos2d::ccColor3B m_currentVehicleColor;
    cocos2d::ccColor3B m_vehicleGlowColor;
    bool m_hasVehicleGlow;
    bool m_hasCustomVehicleGlowColor;

    bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4);
    void updateGlowColor();
    void updatePlayerGlow();
    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor);
    void switchedToMode(GameObjectType p0);
    void updatePlayerFrame(int frame);
    void updatePlayerShipFrame(int frame);
    void updatePlayerJetpackFrame(int frame);
    void updatePlayerRollFrame(int frame);
    void updatePlayerBirdFrame(int frame);
    void updatePlayerDartFrame(int frame);
    void updatePlayerSwingFrame(int frame);
    void resetObject();

    void setVehicleColor(cocos2d::ccColor3B const& color);
    void setVehicleSecondColor(cocos2d::ccColor3B const& color);
    void enableCustomVehicleGlowColor(cocos2d::ccColor3B const& color);
    void setGlowColorCI(IconType type, bool enable, cocos2d::ccColor3B const& color);
    void setGlowColorCI(IconType type, bool enable, int color);
    void setColorsCI(IconType type, cocos2d::ccColor3B const& color1, cocos2d::ccColor3B const& color2);
    void setColorsCI(IconType type, int color1, int color2);
    IconType getGamemode();
    int getNextIconCI(IconType type, int originalFrame);
    void refreshColorsCI();
    changing_icons::CITempProperties& getActiveProperties(IconType type);
    std::pair<IconType, changing_icons::CITempProperties> setupCIValues(IconType type);
};