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
    int m_birdIndex;
    int m_dartIndex;
    int m_robotIndex;
    int m_spiderIndex;
    int m_swingIndex;

    bool init(int p0, int p1, GJBaseGameLayer* p2, CCLayer* p3, bool p4) {
        if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

        CIPlayerObject::initCIValues(IconType::Cube);
        if (m_isPlatformer) CIPlayerObject::initCIValues(IconType::Jetpack);
        else CIPlayerObject::initCIValues(IconType::Ship);
        CIPlayerObject::initCIValues(IconType::Ball);
        CIPlayerObject::initCIValues(IconType::Ufo);
        CIPlayerObject::initCIValues(IconType::Wave);
        CIPlayerObject::initCIValues(IconType::Robot);
        CIPlayerObject::initCIValues(IconType::Spider);
        CIPlayerObject::initCIValues(IconType::Swing);

        return true;
    }

    void initCIValues(IconType type) {
        auto gm = GameManager::get();
        auto& config = ci::CIConfigManager::get()->getConfig(type);
        auto& index = CIPlayerObject::getActiveIndex(type);
        if (config.order == ci::IconOrder::Backward) { // Start from the end
            if (config.useAll) index = gm->countForType(type);
            else index = config.iconSet.size() - 1;
        } else {
            if (config.useAll) index = 1;
        }
        config.tempOrder = config.order;
    }

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
        m_vehicleSprite->setColor(color);
    }
    void setVehicleSecondColor(ccColor3B color) {
        m_vehicleSpriteSecondary->setColor(color);
    }

    void updateIconsCI(IconType type) {
        auto& config = ci::CIConfigManager::get()->getConfig(type);
        auto gm = GameManager::get();

        int newIcon = 1;
        int newColor1 = gm->getPlayerColor();
        int newColor2 = gm->getPlayerColor2();

        if (config.disabled || (config.iconSet.empty() && !config.useAll)) {
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            return;
        };

        int start = 0;
        int end = config.iconSet.size() - 1;
        int result;
        auto& index = CIPlayerObject::getActiveIndex(type);
        if (config.useAll) {
            start = 1;
            end = gm->countForType(type);
        }
        switch (config.tempOrder) {
        case ci::IconOrder::Random:
            result = ci::utils::randomIntInRange(start, end);
            break;
        case ci::IconOrder::Backward:
            result = index;
            index--;
            if (index < start) {
                if (config.mirrorEnd) {
                    index += 2;
                    config.tempOrder = ci::IconOrder::Forward;
                } else index = end;
            }
            break;
        case ci::IconOrder::Forward:
            result = index;
            index++;
            if (index > end) {
                if (config.mirrorEnd) {
                    index -= 2;
                    config.tempOrder = ci::IconOrder::Backward;
                } else index = start;
            }
        }

        if (config.useAll) {
            newIcon = result;
        } else {
            auto const& iconProps = config.iconSet.at(result);
            newIcon = iconProps.iconID;
            if (iconProps.color1) newColor1 = iconProps.color1.value();
            if (iconProps.color2) newColor2 = iconProps.color2.value();
        }
        log::debug("Icon changed to ID {}", newIcon);

        switch (type) {
        default: break;
        case IconType::Cube:
            PlayerObject::updatePlayerFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Ship:
            PlayerObject::updatePlayerShipFrame(newIcon);
            CIPlayerObject::setVehicleColor(gm->colorForIdx(newColor1));
            CIPlayerObject::setVehicleSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Ball:
            PlayerObject::updatePlayerRollFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Ufo:
            PlayerObject::updatePlayerBirdFrame(newIcon);
            CIPlayerObject::setVehicleColor(gm->colorForIdx(newColor1));
            CIPlayerObject::setVehicleSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Wave:
            PlayerObject::updatePlayerDartFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Robot:
            PlayerObject::updatePlayerRobotFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Spider:
            PlayerObject::updatePlayerSpiderFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Swing:
            PlayerObject::updatePlayerSwingFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            break;
        case IconType::Jetpack:
            PlayerObject::updatePlayerJetpackFrame(newIcon);
            CIPlayerObject::setVehicleColor(gm->colorForIdx(newColor1));
            CIPlayerObject::setVehicleSecondColor(gm->colorForIdx(newColor2));
            break;
        }
    }

    int& getActiveIndex(IconType type) {
        switch (type) {
            default:
            case IconType::Cube: return m_fields->m_cubeIndex;
            case IconType::Ship: // Share with jetpack since there can only be one in a given level
            case IconType::Jetpack: return m_fields->m_shipIndex;
            case IconType::Ball: return m_fields->m_ballIndex;
            case IconType::Ufo: return m_fields->m_birdIndex;
            case IconType::Wave: return m_fields->m_dartIndex;
            case IconType::Robot: return m_fields->m_robotIndex;
            case IconType::Spider: return m_fields->m_spiderIndex;
            case IconType::Swing: return m_fields->m_swingIndex;
        }
    }
};