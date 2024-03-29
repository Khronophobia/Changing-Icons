#pragma once
#include <Geode/modify/PlayerObject.hpp>

namespace changing_icons {
    struct CITempProperties;
}

class $modify(CIPlayerObject, PlayerObject) {
    bool m_ciHasInit;
    bool m_levelStarted;

    int m_ogColor1 = GameManager::get()->getPlayerColor();
    int m_ogColor2 = GameManager::get()->getPlayerColor2();

    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor);
    void setVehicleColor(cocos2d::ccColor3B const& color);
    void setVehicleSecondColor(cocos2d::ccColor3B const& color);
    void setColorsCI(IconType type, cocos2d::ccColor3B const& color1, cocos2d::ccColor3B const& color2);
    void setColorsCI(IconType type, int color1, int color2);
    void switchedToMode(GameObjectType p0);
    void updatePlayerFrame(int frame);
    void updatePlayerShipFrame(int frame);
    void updatePlayerJetpackFrame(int frame);
    void updatePlayerRollFrame(int frame);
    void updatePlayerBirdFrame(int frame);
    void updatePlayerDartFrame(int frame);
    void updatePlayerSwingFrame(int frame);
    void resetObject();
    IconType getGamemode();
    int getNextIconCI(IconType type, int originalFrame);
    void refreshColorsCI();
    changing_icons::CITempProperties& getActiveProperties(IconType type);
};