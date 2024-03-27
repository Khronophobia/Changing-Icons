#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <class/CIConfigManager.hpp>
#include <Random.hpp>
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

    int m_ogColor1 = GameManager::get()->getPlayerColor();
    int m_ogColor2 = GameManager::get()->getPlayerColor2();

    bool init(int p0, int p1, GJBaseGameLayer* p2, CCLayer* p3, bool p4) {
        if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;
        if (!PlayLayer::get() && !LevelEditorLayer::get()) return true;

        initCIValues(IconType::Cube);
        if (m_isPlatformer) initCIValues(IconType::Jetpack);
        else initCIValues(IconType::Ship);
        initCIValues(IconType::Ball);
        initCIValues(IconType::Ufo);
        initCIValues(IconType::Wave);
        initCIValues(IconType::Robot);
        initCIValues(IconType::Spider);
        initCIValues(IconType::Swing);

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

    void flashPlayer(float p0, float p1, ccColor3B mainColor, ccColor3B secondColor) {
        // Make this do nothing because it resets the colors
    }

    void setVehicleColor(ccColor3B color) {
        m_vehicleSprite->setColor(color);
    }
    void setVehicleSecondColor(ccColor3B color) {
        m_vehicleSpriteSecondary->setColor(color);
    }

    void updateColorsCI(int const color1, int const color2) {
        if (m_isShip || m_isBird) {
            setVehicleColor(GameManager::get()->colorForIdx(color1));
            setVehicleSecondColor(GameManager::get()->colorForIdx(color2));
        } else {
            PlayerObject::setColor(GameManager::get()->colorForIdx(color1));
            PlayerObject::setSecondColor(GameManager::get()->colorForIdx(color2));
        }
    }

    void switchedToMode(GameObjectType p0) { // Need to do this because updatePlayerRobotFrame
        PlayerObject::switchedToMode(p0);    // and updatePlayerSpiderFrame are inlined
        if (!PlayLayer::get() && !LevelEditorLayer::get()) return;
        switch (p0) {
        default: return;
        case GameObjectType::RobotPortal:
            PlayerObject::updatePlayerRobotFrame(getNextIconCI(IconType::Robot, GameManager::get()->getPlayerRobot()));
            break;
        case GameObjectType::SpiderPortal:
            PlayerObject::updatePlayerSpiderFrame(getNextIconCI(IconType::Spider, GameManager::get()->getPlayerSpider()));
            break;
        }
    }

    void updatePlayerFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerFrame(frame);
            return;
        }
        PlayerObject::updatePlayerFrame(getNextIconCI(IconType::Cube, frame));
    }

    void updatePlayerShipFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerShipFrame(frame);
            return;
        }
        PlayerObject::updatePlayerShipFrame(getNextIconCI(IconType::Ship, frame));
    }

    void updatePlayerJetpackFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerJetpackFrame(frame);
            return;
        }
        PlayerObject::updatePlayerJetpackFrame(getNextIconCI(IconType::Jetpack, frame));
    }

    void updatePlayerRollFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerRollFrame(frame);
            return;
        }
        PlayerObject::updatePlayerRollFrame(getNextIconCI(IconType::Ball, frame));
    }

    void updatePlayerBirdFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerBirdFrame(frame);
            return;
        }
        PlayerObject::updatePlayerBirdFrame(getNextIconCI(IconType::Ufo, frame));
    }

    void updatePlayerDartFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerDartFrame(frame);
            return;
        }
        PlayerObject::updatePlayerDartFrame(getNextIconCI(IconType::Wave, frame));
    }

    void updatePlayerSwingFrame(int frame) {
        if (!PlayLayer::get() && !LevelEditorLayer::get()) {
            PlayerObject::updatePlayerSwingFrame(frame);
            return;
        }
        PlayerObject::updatePlayerSwingFrame(getNextIconCI(IconType::Swing, frame));
    }

    int getNextIconCI(IconType type, int originalFrame, bool updateIndex = true) {
        auto gm = GameManager::get();
        auto& config = getActiveProperties(type);
        auto const& iconSet = ci::CIConfigManager::get()->getConfig(type).iconSet;

        int color1 = m_fields->m_ogColor1;
        int color2 = m_fields->m_ogColor2;

        if (config.disabled || (!config.useAll && iconSet.empty())) {
            updateColorsCI(color1, color2);
            return originalFrame;
        }

        if (!config.useAll && iconSet.size() == 1) {
            auto const& iconProps = iconSet.at(0);
            if (iconProps.color1) color1 = iconProps.color1.value();
            if (iconProps.color2) color2 = iconProps.color2.value();
            updateColorsCI(color1, color2);
            return iconProps.iconID;
        }

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
            // https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/
            result = Random::get(start, end);
            break;
        case ci::IconOrder::Forward:
            result = index;
            if (updateIndex) ++index;
            if (index > end) {
                if (config.mirrorEnd) {
                    index -= 2;
                    config.order = ci::IconOrder::Backward;
                } else index = start;
            }
            break;
        case ci::IconOrder::Backward:
            result = index;
            if (updateIndex) --index;
            if (index < start) {
                if (config.mirrorEnd) {
                    index += 2;
                    config.order = ci::IconOrder::Forward;
                } else index = end;
            }
            break;
        }

        int newIcon;
        if (config.useAll) newIcon = result;
        else {
            auto const& iconProps = iconSet.at(result);
            newIcon = iconProps.iconID;
            if (iconProps.color1) color1 = iconProps.color1.value();
            if (iconProps.color2) color2 = iconProps.color2.value();
        }
        updateColorsCI(color1, color2);
        log::info("Changed icon to ID {}", newIcon);
        return newIcon;
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