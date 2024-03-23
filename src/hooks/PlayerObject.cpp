#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <class/CIConfigManager.hpp>
#include <CIUtilities.hpp>
#include <CIConfigProperties.hpp>

using namespace geode::prelude;
namespace ci = changing_icons;

class $modify(CIPlayerObject, PlayerObject) {
    ci::CITempProperties m_cubeProperties;
    ci::CITempProperties m_shipProperties;
    ci::CITempProperties m_ballProperties;
    ci::CITempProperties m_birdProperties;
    ci::CITempProperties m_dartProperties;
    ci::CITempProperties m_robotProperties;
    ci::CITempProperties m_spiderProperties;
    ci::CITempProperties m_swingProperties;

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
        auto const& config = ci::CIConfigManager::get()->getConfig(type);
        auto const& globalConfig = ci::CIConfigManager::get()->getGlobalConfig();

        auto order = config.order;
        auto disabled = config.disabled;
        auto useAll = config.useAll;
        auto mirrorEnd = config.mirrorEnd;
        if (globalConfig.globalOverrides.find(type) != globalConfig.globalOverrides.end()) {
            log::info("Gamemode {} listed in global overrides", static_cast<int>(type));
            if (globalConfig.override.order) order = globalConfig.override.order.value();
            if (globalConfig.override.disabled) disabled = globalConfig.override.disabled.value();
            if (globalConfig.override.useAll) useAll = globalConfig.override.useAll.value();
        }
        int index;
        if (useAll) {
            switch (order) {
                case ci::IconOrder::Random: [[fallthrough]];
                case ci::IconOrder::Forward: index = 1; break;
                case ci::IconOrder::Backward: index = gm->countForType(type); break;
            }
        } else {
            switch (order) {
                case ci::IconOrder::Random: [[fallthrough]];
                case ci::IconOrder::Forward: index = 0; break;
                case ci::IconOrder::Backward: index = config.iconSet.size() - 1; break;
            }
        }
        CIPlayerObject::getActiveProperties(type) = {
            .index = index,
            .order = order,
            .useAll = useAll,
            .mirrorEnd = mirrorEnd,
            .disabled = disabled
        };
    }

    void toggleFlyMode(bool p0, bool p1) {
        PlayerObject::toggleFlyMode(p0, p1);
        CIPlayerObject::updateIconsCI(IconType::Cube);
        if (p0) {
            if (m_isPlatformer) CIPlayerObject::updateIconsCI(IconType::Jetpack);
            else CIPlayerObject::updateIconsCI(IconType::Ship);
        }
    }
    void toggleRollMode(bool p0, bool p1) {
        PlayerObject::toggleRollMode(p0, p1);
        if (p0) CIPlayerObject::updateIconsCI(IconType::Ball);
    }
    void toggleBirdMode(bool p0, bool p1) {
        PlayerObject::toggleBirdMode(p0, p1);
        if (p0) {
            CIPlayerObject::updateIconsCI(IconType::Ufo);
        }
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
    void flashPlayer(float p0, float p1, ccColor3B mainColor, ccColor3B secondColor) {
        // Make this do nothing because it resets the colors
    }

    void setVehicleColor(ccColor3B color) {
        m_vehicleSprite->setColor(color);
    }
    void setVehicleSecondColor(ccColor3B color) {
        m_vehicleSpriteSecondary->setColor(color);
    }

    void updateIconsCI(IconType type) {
        auto gm = GameManager::get();
        if (!gm->getPlayLayer() && !gm->getEditorLayer()) return;
        auto& config = CIPlayerObject::getActiveProperties(type);

        auto const& iconSet = ci::CIConfigManager::get()->getConfig(type).iconSet;
        int newIcon = 1;
        int newColor1 = gm->getPlayerColor();
        int newColor2 = gm->getPlayerColor2();

        if (config.disabled || (iconSet.empty() && !config.useAll)) {
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            return;
        };

        int start = 0;
        int end = iconSet.size() - 1;
        int result;
        auto& index = config.index;
        if (config.useAll) {
            start = 1;
            end = gm->countForType(type);
        }
        switch (config.order) {
        case ci::IconOrder::Random:
            result = ci::utils::randomIntInRange(start, end);
            break;
        case ci::IconOrder::Backward:
            result = index;
            --index;
            if (index < start) {
                if (config.mirrorEnd) {
                    index += 2;
                    config.order = ci::IconOrder::Forward;
                } else index = end;
            }
            break;
        case ci::IconOrder::Forward:
            result = index;
            ++index;
            if (index > end) {
                if (config.mirrorEnd) {
                    index -= 2;
                    config.order = ci::IconOrder::Backward;
                } else index = start;
            }
        }

        if (config.useAll) {
            newIcon = result;
        } else {
            auto const& iconProps = iconSet.at(result);
            newIcon = iconProps.iconID;
            if (iconProps.color1) newColor1 = iconProps.color1.value();
            if (iconProps.color2) newColor2 = iconProps.color2.value();
        }

        switch (type) {
        default: break;
        case IconType::Cube:
            PlayerObject::updatePlayerFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            log::debug("Cube icon changed to ID {}", newIcon);
            break;
        case IconType::Ship:
            PlayerObject::updatePlayerShipFrame(newIcon);
            CIPlayerObject::setVehicleColor(gm->colorForIdx(newColor1));
            CIPlayerObject::setVehicleSecondColor(gm->colorForIdx(newColor2));
            log::debug("Ship icon changed to ID {}", newIcon);
            break;
        case IconType::Ball:
            PlayerObject::updatePlayerRollFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            log::debug("Ball icon changed to ID {}", newIcon);
            break;
        case IconType::Ufo:
            PlayerObject::updatePlayerBirdFrame(newIcon);
            CIPlayerObject::setVehicleColor(gm->colorForIdx(newColor1));
            CIPlayerObject::setVehicleSecondColor(gm->colorForIdx(newColor2));
            log::debug("UFO icon changed to ID {}", newIcon);
            break;
        case IconType::Wave:
            PlayerObject::updatePlayerDartFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            log::debug("Wave icon changed to ID {}", newIcon);
            break;
        case IconType::Robot:
            PlayerObject::updatePlayerRobotFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            log::debug("Robot icon changed to ID {}", newIcon);
            break;
        case IconType::Spider:
            PlayerObject::updatePlayerSpiderFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            log::debug("Spider icon changed to ID {}", newIcon);
            break;
        case IconType::Swing:
            PlayerObject::updatePlayerSwingFrame(newIcon);
            PlayerObject::setColor(gm->colorForIdx(newColor1));
            PlayerObject::setSecondColor(gm->colorForIdx(newColor2));
            log::debug("Swing icon changed to ID {}", newIcon);
            break;
        case IconType::Jetpack:
            PlayerObject::updatePlayerJetpackFrame(newIcon);
            CIPlayerObject::setVehicleColor(gm->colorForIdx(newColor1));
            CIPlayerObject::setVehicleSecondColor(gm->colorForIdx(newColor2));
            log::debug("Jetpack icon changed to ID {}", newIcon);
            break;
        }
    }

    ci::CITempProperties& getActiveProperties(IconType type) {
        switch (type) {
            default:
            case IconType::Cube: return m_fields->m_cubeProperties;
            case IconType::Ship: [[fallthrough]];
            case IconType::Jetpack: return m_fields->m_shipProperties;
            case IconType::Ball: return m_fields->m_ballProperties;
            case IconType::Ufo: return m_fields->m_birdProperties;
            case IconType::Wave: return m_fields->m_dartProperties;
            case IconType::Robot: return m_fields->m_robotProperties;
            case IconType::Spider: return m_fields->m_spiderProperties;
            case IconType::Swing: return m_fields->m_swingProperties;
        }
    }
};