#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "class/CIConfigManager.hpp"
#include "CIUtilities.hpp"

using namespace geode::prelude;
namespace ci = changing_icons;

class $modify(CIPlayerObject, PlayerObject) {
    int m_cubeIndex;
    int m_shipIndex;
    int m_ballIndex;

    void toggleFlyMode(bool p0, bool p1) {
        PlayerObject::toggleFlyMode(p0, p1);
        CIPlayerObject::updateIconsCI(IconType::Cube);
        if (m_isPlatformer) CIPlayerObject::updateIconsCI(IconType::Jetpack);
        else CIPlayerObject::updateIconsCI(IconType::Ship);
    }
    void toggleRollMode(bool p0, bool p1) {
        PlayerObject::toggleRollMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Ball);
    }
    void toggleBirdMode(bool p0, bool p1) {
        PlayerObject::toggleBirdMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Ufo);
    }
    void toggleDartMode(bool p0, bool p1) {
        PlayerObject::toggleDartMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Wave);
    }
    void toggleRobotMode(bool p0, bool p1) {
        PlayerObject::toggleRobotMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Robot);
    }
    void toggleSpiderMode(bool p0, bool p1) {
        PlayerObject::toggleSpiderMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Spider);
    }
    void toggleSwingMode(bool p0, bool p1) {
        PlayerObject::toggleSwingMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Swing);
    }

    void setVehicleColor(ccColor3B color) {
        this->m_vehicleSprite->setColor(color);
    }
    void setVehicleSecondColor(ccColor3B color) {
        this->m_vehicleSpriteSecondary->setColor(color);
    }

    void updateIconsCI(IconType type) {
        auto const& config = ci::CIConfigManager::get()->getConfig(type);

        if (config.disabled || (config.iconSet.empty() && !config.useAll)) return;

        int newIcon = 1;
        if (config.order == ci::IconOrder::Random) {
            if (config.useAll) newIcon = ci::utils::randomIntInRange(1, GameManager::get()->countForType(type));
            else {
                auto result = ci::utils::randomIntInRange(0, config.iconSet.size() - 1);
                newIcon = config.iconSet.at(result).iconID;
            }
        }
        switch (type) {
        default: break;
        case IconType::Cube:
            PlayerObject::updatePlayerFrame(newIcon);
            break;
        case IconType::Ship:
            PlayerObject::updatePlayerShipFrame(newIcon);
            break;
        case IconType::Ball:
            PlayerObject::updatePlayerRollFrame(newIcon);
            break;
        case IconType::Ufo:
            PlayerObject::updatePlayerBirdFrame(newIcon);
            break;
        case IconType::Wave:
            PlayerObject::updatePlayerDartFrame(newIcon);
            break;
        case IconType::Robot:
            PlayerObject::updatePlayerRobotFrame(newIcon);
            break;
        case IconType::Spider:
            PlayerObject::updatePlayerSpiderFrame(newIcon);
            break;
        case IconType::Swing:
            PlayerObject::updatePlayerSwingFrame(newIcon);
            break;
        case IconType::Jetpack:
            PlayerObject::updatePlayerJetpackFrame(newIcon);
            break;
        }
    }
};