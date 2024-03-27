#pragma once
#include <Geode/modify/PlayerObject.hpp>
#include <CIConfigProperties.hpp>

class $modify(CIPlayerObject, PlayerObject) {
    changing_icons::CITempProperties m_cubeProperties;
    changing_icons::CITempProperties m_shipProperties;
    changing_icons::CITempProperties m_ballProperties;
    changing_icons::CITempProperties m_birdProperties;
    changing_icons::CITempProperties m_dartProperties;
    changing_icons::CITempProperties m_robotProperties;
    changing_icons::CITempProperties m_spiderProperties;
    changing_icons::CITempProperties m_swingProperties;

    int m_ogColor1 = GameManager::get()->getPlayerColor();
    int m_ogColor2 = GameManager::get()->getPlayerColor2();

    bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4);
    void setupCIValues(IconType type);
    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor);
    void setVehicleColor(cocos2d::ccColor3B color);
    void setVehicleSecondColor(cocos2d::ccColor3B color);
    void updateColorsCI(cocos2d::ccColor3B const& color1, cocos2d::ccColor3B const& color2);
    void updateColorsCI(int color1, int color2);
    void switchedToMode(GameObjectType p0);
    void updatePlayerFrame(int frame);
    void updatePlayerShipFrame(int frame);
    void updatePlayerJetpackFrame(int frame);
    void updatePlayerRollFrame(int frame);
    void updatePlayerBirdFrame(int frame);
    void updatePlayerDartFrame(int frame);
    void updatePlayerSwingFrame(int frame);
    int getNextIconCI(IconType type, int originalFrame, bool updateIndex = true);
    changing_icons::CITempProperties& getActiveProperties(IconType type);
};