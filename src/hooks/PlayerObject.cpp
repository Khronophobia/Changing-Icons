#include "PlayerObject.hpp"
#include "GJBaseGameLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <CIUtilities.hpp>
#include <Random.hpp>

using namespace geode::prelude;
using namespace changing_icons;

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

void CIPlayerObject::updateGlowColor() {
    PlayerObject::updateGlowColor();
    m_vehicleGlow->setColor(m_fields->m_vehicleGlowColor);
}

void CIPlayerObject::updatePlayerGlow() {
    PlayerObject::updatePlayerGlow();
    if (m_isShip || m_isBird)
        m_vehicleGlow->setVisible(m_fields->m_hasVehicleGlow);
}

void CIPlayerObject::flashPlayer(float p0, float p1, ccColor3B mainColor, ccColor3B secondColor) {
    // Make this do nothing because it resets the colors
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

void CIPlayerObject::switchedToMode(GameObjectType p0) { // Need to do this because updatePlayerRobotFrame
    PlayerObject::switchedToMode(p0);                    // and updatePlayerSpiderFrame are inlined
    if (!PlayLayer::get()) return;
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

void CIPlayerObject::updatePlayerFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerFrame(frame);
        return;
    }
    PlayerObject::updatePlayerFrame(getNextIconCI(IconType::Cube, frame));
}

void CIPlayerObject::updatePlayerShipFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerShipFrame(frame);
        return;
    }
    PlayerObject::updatePlayerShipFrame(getNextIconCI(IconType::Ship, frame));
}

void CIPlayerObject::updatePlayerJetpackFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerJetpackFrame(frame);
        return;
    }
    PlayerObject::updatePlayerJetpackFrame(getNextIconCI(IconType::Jetpack, frame));
}

void CIPlayerObject::updatePlayerRollFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerRollFrame(frame);
        return;
    }
    PlayerObject::updatePlayerRollFrame(getNextIconCI(IconType::Ball, frame));
}

void CIPlayerObject::updatePlayerBirdFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerBirdFrame(frame);
        return;
    }
    PlayerObject::updatePlayerBirdFrame(getNextIconCI(IconType::Ufo, frame));
}

void CIPlayerObject::updatePlayerDartFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerDartFrame(frame);
        return;
    }
    PlayerObject::updatePlayerDartFrame(getNextIconCI(IconType::Wave, frame));
}

void CIPlayerObject::updatePlayerSwingFrame(int frame) {
    if (!PlayLayer::get()) {
        PlayerObject::updatePlayerSwingFrame(frame);
        return;
    }
    PlayerObject::updatePlayerSwingFrame(getNextIconCI(IconType::Swing, frame));
}

void CIPlayerObject::resetObject() { // I need something that gets called after init,
    PlayerObject::resetObject();     // And this seems the closest to what I want
    if (m_fields->m_levelStarted) refreshColorsCI();
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

int CIPlayerObject::getNextIconCI(IconType type, int originalFrame) {
    if (!m_fields->m_levelStarted) return originalFrame;

    auto gm = GameManager::get();
    auto& config = getActiveProperties(type);

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

    int start = 0;
    int end = config.iconSet.size() - 1;
    int result;
    if (config.useAll) {
        start = 1;
        end = gm->countForType(type);
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
    if (config.useAll) newIcon = result;
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
    if (config.disabled) return;
    bool enableGlow = m_fields->m_ogHasGlow;
    if (config.useAll) {
        setColorsCI(getGamemode(), m_fields->m_ogColor1, m_fields->m_ogColor2);
        setGlowColorCI(getGamemode(), enableGlow, m_fields->m_ogGlowColor);
        return;
    }
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

CITempProperties& CIPlayerObject::getActiveProperties(IconType type) {
    auto ciGameLayer = static_cast<CIBaseGameLayer*>(m_gameLayer);
    if (this == PlayLayer::get()->m_player1)
        return ciGameLayer->m_fields->m_player1CIProperties.at(type);
    else
        return ciGameLayer->m_fields->m_player2CIProperties.at(type);
}