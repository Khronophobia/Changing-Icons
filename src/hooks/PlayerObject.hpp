#pragma once
#include <Geode/modify/PlayerObject.hpp>
#include <properties/TempProperties.hpp>

class $modify(CIPlayerObject, PlayerObject) {
    struct Fields {
        bool m_levelStarted;
        std::unordered_map<IconType, changing_icons::CITempProperties> m_ciProperties;
        inline static changing_icons::CITempProperties s_emptyCIProperty = changing_icons::CITempProperties();
        int m_ogColor1 = GameManager::get()->getPlayerColor();
        int m_ogColor2 = GameManager::get()->getPlayerColor2();
        int m_ogGlowColor = GameManager::get()->getPlayerGlowColor();
        bool m_ogHasGlow;
        cocos2d::ccColor3B m_currentColor;
        cocos2d::ccColor3B m_currentVehicleColor;
        cocos2d::ccColor3B m_vehicleGlowColor;
        bool m_hasVehicleGlow;
        bool m_hasCustomVehicleGlowColor;
    };

    $override
    bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4);
    void setColor(cocos2d::ccColor3B const& color);
    void updateGlowColor();
    void updatePlayerGlow();
    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor);
    void updatePlayerFrame(int frame);
    void updatePlayerShipFrame(int frame);
    void updatePlayerJetpackFrame(int frame);
    void updatePlayerRollFrame(int frame);
    void updatePlayerBirdFrame(int frame);
    void updatePlayerDartFrame(int frame);
    void updatePlayerSwingFrame(int frame);
    void switchedToMode(GameObjectType p0);
    void spawnFromPlayer(PlayerObject* p0, bool p1);
    void resetObject();

    void setVehicleColor(cocos2d::ccColor3B const& color);
    void setVehicleSecondColor(cocos2d::ccColor3B const& color);
    void enableCustomVehicleGlowColor(cocos2d::ccColor3B const& color);
    void setGlowColorCI(bool isVehicle, bool enable, cocos2d::ccColor3B const& color);
    void setGlowColorCI(bool isVehicle, bool enable, int color);
    void setColorsCI(bool isVehicle, cocos2d::ccColor3B const& color1, cocos2d::ccColor3B const& color2);
    void setColorsCI(bool isVehicle, int color1, int color2);
    changing_icons::CITempProperties& getActiveProperties(IconType type);
    int getNextIconCI(IconType type, int originalFrame);
    void refreshColorsCI();
};