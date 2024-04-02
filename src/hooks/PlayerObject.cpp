#include "PlayerObject.hpp"
#include <class/CIConfigManager.hpp>
#include <CIUtilities.hpp>
#include <Random.hpp>

using namespace geode::prelude;
using namespace changing_icons;

$override
bool CIPlayerObject::init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4) {
    if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

    m_fields->m_ogHasGlow = m_hasGlow;
    // Not sure if these are needed but just in case
    m_fields->m_currentColor = GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor());
    m_fields->m_currentVehicleColor = GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor());
    m_fields->m_vehicleGlowColor = GameManager::get()->colorForIdx(GameManager::get()->getPlayerGlowColor());
    m_fields->m_hasVehicleGlow = m_hasGlow;
    m_fields->m_hasCustomVehicleGlowColor = m_hasCustomGlowColor;

    return true;
}

$override
void CIPlayerObject::updateGlowColor() {
    PlayerObject::updateGlowColor();
    m_vehicleGlow->setColor(m_fields->m_vehicleGlowColor);
}

$override
void CIPlayerObject::updatePlayerGlow() {
    PlayerObject::updatePlayerGlow();
    if (m_isShip || m_isBird)
        m_vehicleGlow->setVisible(m_fields->m_hasVehicleGlow);
}

$override
void CIPlayerObject::flashPlayer(float p0, float p1, ccColor3B mainColor, ccColor3B secondColor) {
    // Make this do nothing because it resets the colors
}

$override
void CIPlayerObject::updatePlayerFrame(int frame) {
    PlayerObject::updatePlayerFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerFrame(getNextIconCI(IconType::Cube, frame));
}

$override
void CIPlayerObject::updatePlayerShipFrame(int frame) {
    PlayerObject::updatePlayerShipFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerShipFrame(getNextIconCI(IconType::Ship, frame));
}

$override
void CIPlayerObject::updatePlayerJetpackFrame(int frame) {
    PlayerObject::updatePlayerJetpackFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerJetpackFrame(getNextIconCI(IconType::Jetpack, frame));
}

$override
void CIPlayerObject::updatePlayerRollFrame(int frame) {
    PlayerObject::updatePlayerRollFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerRollFrame(getNextIconCI(IconType::Ball, frame));
}

$override
void CIPlayerObject::updatePlayerBirdFrame(int frame) {
    PlayerObject::updatePlayerBirdFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerBirdFrame(getNextIconCI(IconType::Ufo, frame));
}

$override
void CIPlayerObject::updatePlayerDartFrame(int frame) {
    PlayerObject::updatePlayerDartFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerDartFrame(getNextIconCI(IconType::Wave, frame));
}

$override
void CIPlayerObject::updatePlayerSwingFrame(int frame) {
    PlayerObject::updatePlayerSwingFrame(frame);
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    PlayerObject::updatePlayerSwingFrame(getNextIconCI(IconType::Swing, frame));
}

$override
void CIPlayerObject::switchedToMode(GameObjectType p0) { // Need to do this because updatePlayerRobotFrame
    PlayerObject::switchedToMode(p0);                    // and updatePlayerSpiderFrame are inlined
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    switch (p0) {
    default: return;
    case GameObjectType::RobotPortal:
        PlayerObject::updatePlayerRobotFrame(getNextIconCI(IconType::Robot, GameManager::get()->getPlayerRobot()));
        break;
    case GameObjectType::SpiderPortal:
        PlayerObject::updatePlayerSpiderFrame(getNextIconCI(IconType::Spider, GameManager::get()->getPlayerSpider()));
        break;
    }
    PlayerObject::updateGlowColor();
}

$override
void CIPlayerObject::resetObject() { // I need something that gets called after init,
    PlayerObject::resetObject();     // And this seems the closest to what I want
    if (!PlayLayer::get()) return;
    if (PlayLayer::get()->m_player1 != this && PlayLayer::get()->m_player2 != this)
        return;

    if (!m_fields->m_ciHasInit) {
        m_fields->m_ciProperties = {
            setupCIValues(IconType::Cube),
            setupCIValues(IconType::Ship),
            setupCIValues(IconType::Ball),
            setupCIValues(IconType::Ufo),
            setupCIValues(IconType::Wave),
            setupCIValues(IconType::Robot),
            setupCIValues(IconType::Spider),
            setupCIValues(IconType::Swing),
            setupCIValues(IconType::Jetpack)
        };

        log::info("CI values initialized");
        m_fields->m_ciHasInit = true;
    }

    if (m_fields->m_levelStarted) refreshColorsCI();
}

void CIPlayerObject::setVehicleColor(ccColor3B const& color) {
    m_vehicleSprite->setColor(color);
}

void CIPlayerObject::setVehicleSecondColor(ccColor3B const& color) {
    m_vehicleSpriteSecondary->setColor(color);
}

void CIPlayerObject::enableCustomVehicleGlowColor(ccColor3B const& color) {
    m_fields->m_hasCustomVehicleGlowColor = true;
    m_fields->m_vehicleGlowColor = color;
}

void CIPlayerObject::setColorsCI(IconType type, ccColor3B const& color1, ccColor3B const& color2) {
    if (type == IconType::Ship || type == IconType::Ufo || type == IconType::Jetpack) {
        setVehicleColor(color1);
        setVehicleSecondColor(color2);
        m_fields->m_currentVehicleColor = color1;
    } else {
        PlayerObject::setColor(color1);
        PlayerObject::setSecondColor(color2);
        m_fields->m_currentColor = color1;
    }
}

void CIPlayerObject::setColorsCI(IconType type, int color1, int color2) {
    setColorsCI(type, GameManager::get()->colorForIdx(color1), GameManager::get()->colorForIdx(color2));
}

void CIPlayerObject::setGlowColorCI(IconType type, bool enable, ccColor3B const& color) {
    if (type == IconType::Ship || type == IconType::Ufo || type == IconType::Jetpack) {
        m_fields->m_hasVehicleGlow = enable || (m_fields->m_currentVehicleColor == cc3x(0x0));
        enableCustomVehicleGlowColor(color);
    } else {
        m_hasGlow = enable || (m_fields->m_currentColor == cc3x(0x0));
        PlayerObject::enableCustomGlowColor(color);
    }
    PlayerObject::updateGlowColor();
    PlayerObject::updatePlayerGlow();
}

void CIPlayerObject::setGlowColorCI(IconType type, bool enable, int color) {
    setGlowColorCI(type, enable, GameManager::get()->colorForIdx(color));
}

IconType CIPlayerObject::getGamemode() {
    if (m_isShip) {
        if (m_isPlatformer) return IconType::Jetpack;
        return IconType::Ship;
    }
    if (m_isBall) return IconType::Ball;
    if (m_isBird) return IconType::Ufo;
    if (m_isDart) return IconType::Wave;
    if (m_isRobot) return IconType::Robot;
    if (m_isSpider) return IconType::Spider;
    if (m_isSwing) return IconType::Swing;

    return IconType::Cube;
}

CITempProperties& CIPlayerObject::getActiveProperties(IconType type) {
    if (m_fields->m_ciProperties.contains(type))
        return m_fields->m_ciProperties.at(type);

    return m_fields->s_emptyCIProperty;
}

std::pair<IconType, CITempProperties> CIPlayerObject::setupCIValues(IconType type) {
    auto gm = GameManager::get();
    auto const& config = CIManager::get()->getConfig(type);
    auto const& globalConfig = CIManager::get()->getGlobalConfig();

    auto order = config.order;
    auto disabled = config.disabled;
    auto useAll = config.useAll;
    auto includePlayerIcon = config.includePlayerIcon;
    auto mirrorEnd = config.mirrorEnd;
    if (globalConfig.globalOverrides.find(type) != globalConfig.globalOverrides.end()) {
        log::info("Gamemode {} listed in global overrides", static_cast<int>(type));
        if (globalConfig.override.order) order = globalConfig.override.order.value();
        if (globalConfig.override.disabled) disabled = globalConfig.override.disabled.value();
        if (globalConfig.override.useAll) useAll = globalConfig.override.useAll.value();
        if (globalConfig.override.includePlayerIcon) includePlayerIcon = globalConfig.override.includePlayerIcon.value();
    }

    auto iconSet = config.iconSet;
    if (includePlayerIcon && !iconSet.empty()) {
        int playerIconID;
        switch (type) {
            default:
            case IconType::Cube: playerIconID = gm->getPlayerFrame(); break;
            case IconType::Ship: playerIconID = gm->getPlayerShip(); break;
            case IconType::Ball: playerIconID = gm->getPlayerBall(); break;
            case IconType::Ufo: playerIconID = gm->getPlayerBird(); break;
            case IconType::Wave: playerIconID = gm->getPlayerDart(); break;
            case IconType::Robot: playerIconID = gm->getPlayerRobot(); break;
            case IconType::Spider: playerIconID = gm->getPlayerSpider(); break;
            case IconType::Swing: playerIconID = gm->getPlayerSwing(); break;
            case IconType::Jetpack: playerIconID = gm->getPlayerJetpack(); break;
        }
        iconSet.push_back(IconProperties{ .iconID = playerIconID });
    }
    
    int index;
    if (useAll) {
        switch (order) {
            case IconOrder::Random: [[fallthrough]];
            case IconOrder::Forward: index = 1; break;
            case IconOrder::Backward: index = gm->countForType(type); break;
        }
    } else {
        switch (order) {
            case IconOrder::Random: [[fallthrough]];
            case IconOrder::Forward: index = 0; break;
            case IconOrder::Backward: index = iconSet.size() - 1; break;
        }
    }

    auto properties = CITempProperties{
        .current = index,
        .index = index,
        .iconSet = iconSet,
        .order = order,
        .useAll = useAll,
        .mirrorEnd = mirrorEnd,
        .disabled = disabled
    };

    return {type, properties};
}

int CIPlayerObject::getNextIconCI(IconType type, int originalFrame) {
    if (!m_fields->m_levelStarted) return originalFrame;

    auto gm = GameManager::get();
    auto& config = getActiveProperties(type);

    auto disableLockedIcons = Mod::get()->getSettingValue<bool>("disable-locked-icons");
    auto const& unlockedIcons = CIManager::get()->getUnlockedIcons(type);

    int color1 = m_fields->m_ogColor1;
    int color2 = m_fields->m_ogColor2;
    int glowColor = m_fields->m_ogGlowColor;
    bool enableGlow = m_fields->m_ogHasGlow;

    if (config.disabled || (!config.useAll && config.iconSet.empty())) {
        setColorsCI(type, color1, color2);
        setGlowColorCI(type, enableGlow, glowColor);
        return originalFrame;
    }

    if (!config.useAll && config.iconSet.size() == 1) {
        auto const& iconProps = config.iconSet.at(0);
        if (iconProps.color1) color1 = iconProps.color1.value();
        if (iconProps.color2) color2 = iconProps.color2.value();
        if (iconProps.overrideGlow) {
            enableGlow = iconProps.glowColor.has_value();
            if (iconProps.glowColor) glowColor = iconProps.glowColor.value();
        }
        setColorsCI(type, color1, color2);
        setGlowColorCI(type, enableGlow, glowColor);
        return iconProps.iconID;
    }

    if (config.useAll && disableLockedIcons && unlockedIcons.size() == 1) {
        setColorsCI(type, color1, color2);
        setGlowColorCI(type, enableGlow, glowColor);
        return unlockedIcons.at(0);
    }

    int start = 0;
    int end = config.iconSet.size() - 1;
    int result;
    if (config.useAll) {
        if (disableLockedIcons) {
            end = unlockedIcons.size() - 1;
        } else {
            start = 1;
            end = gm->countForType(type);
        }
    }
    switch (config.order) {
    case IconOrder::Random:
        // https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/
        result = Random::get(start, end);
        config.current = result;
        break;
    case IconOrder::Forward:
        result = config.index;
        config.current = result;
        ++config.index;
        if (config.index > end) {
            if (config.mirrorEnd) {
                config.index -= 2;
                config.order = IconOrder::Backward;
            } else config.index = start;
        }
        break;
    case IconOrder::Backward:
        result = config.index;
        config.current = result;
        --config.index;
        if (config.index < start) {
            if (config.mirrorEnd) {
                config.index += 2;
                config.order = IconOrder::Forward;
            } else config.index = end;
        }
        break;
    }

    int newIcon;
    if (config.useAll) {
        if (disableLockedIcons) newIcon = unlockedIcons.at(result);
        else newIcon = result;
    }
    else {
        auto const& iconProps = config.iconSet.at(result);
        newIcon = iconProps.iconID;
        if (iconProps.color1) color1 = iconProps.color1.value();
        if (iconProps.color2) color2 = iconProps.color2.value();
        if (iconProps.overrideGlow) {
            enableGlow = iconProps.glowColor.has_value();
            if (iconProps.glowColor) glowColor = iconProps.glowColor.value();
        }
    }
    setColorsCI(type, color1, color2);
    setGlowColorCI(type, enableGlow, glowColor);

    std::string_view playerName = "P1";
    if (this == PlayLayer::get()->m_player2) playerName = "P2";

    log::info("{}: Changed {} icon to ID {} (Index: {}, Order: {})", playerName, type, newIcon, config.current, config.order);
    return newIcon;
}

void CIPlayerObject::refreshColorsCI() {
    auto const& config = getActiveProperties(getGamemode());
    if (config.disabled || config.useAll || config.iconSet.empty()) return;
    bool enableGlow = m_fields->m_ogHasGlow;
    auto const& icon = config.iconSet.at(config.current);
    if (icon.overrideGlow) enableGlow = icon.glowColor.has_value();
    setColorsCI(
        getGamemode(),
        icon.color1.value_or(m_fields->m_ogColor1),
        icon.color2.value_or(m_fields->m_ogColor2)
    );
    setGlowColorCI(
        getGamemode(),
        enableGlow,
        icon.glowColor.value_or(m_fields->m_ogGlowColor)
    );
}