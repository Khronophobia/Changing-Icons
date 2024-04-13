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
void CIPlayerObject::setColor(ccColor3B const& color) {
    PlayerObject::setColor(color);
    m_fields->m_currentColor = color;
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
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerFrame(getNextIconCI(IconType::Cube, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::updatePlayerShipFrame(int frame) {
    PlayerObject::updatePlayerShipFrame(frame);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerShipFrame(getNextIconCI(IconType::Ship, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::updatePlayerJetpackFrame(int frame) {
    PlayerObject::updatePlayerJetpackFrame(frame);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerJetpackFrame(getNextIconCI(IconType::Jetpack, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::updatePlayerRollFrame(int frame) {
    PlayerObject::updatePlayerRollFrame(frame);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerRollFrame(getNextIconCI(IconType::Ball, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::updatePlayerBirdFrame(int frame) {
    PlayerObject::updatePlayerBirdFrame(frame);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerBirdFrame(getNextIconCI(IconType::Ufo, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::updatePlayerDartFrame(int frame) {
    PlayerObject::updatePlayerDartFrame(frame);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerDartFrame(getNextIconCI(IconType::Wave, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::updatePlayerSwingFrame(int frame) {
    PlayerObject::updatePlayerSwingFrame(frame);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    PlayerObject::updatePlayerSwingFrame(getNextIconCI(IconType::Swing, frame));
    refreshColorsCI();
}

$override
void CIPlayerObject::switchedToMode(GameObjectType p0) { // Need to do this because updatePlayerRobotFrame
    PlayerObject::switchedToMode(p0);                    // and updatePlayerSpiderFrame are inlined
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
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
    refreshColorsCI();
}

$override
void CIPlayerObject::spawnFromPlayer(PlayerObject* p0, bool p1) {
    PlayerObject::spawnFromPlayer(p0, p1);
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player2 != this) return;
    auto gm = GameManager::get();
    if (m_isBall)
        PlayerObject::updatePlayerRollFrame(gm->getPlayerBall());
    else if (m_isDart)
        PlayerObject::updatePlayerDartFrame(gm->getPlayerDart());
    else if (m_isSwing)
        PlayerObject::updatePlayerSwingFrame(gm->getPlayerSwing());
    else if (!m_isRobot && !m_isSpider)
        PlayerObject::updatePlayerFrame(gm->getPlayerFrame());
}

$override
void CIPlayerObject::resetObject() { // I need something that gets called after init,
    PlayerObject::resetObject();     // And this seems the closest to what I want
    if (!PlayLayer::get()) return;
    if (m_gameLayer->m_player1 != this && m_gameLayer->m_player2 != this)
        return;

    if (m_fields->m_levelStarted) refreshColorsCI();
}

void CIPlayerObject::setVehicleColor(ccColor3B const& color) {
    m_vehicleSprite->setColor(color);
    m_fields->m_currentVehicleColor = color;
}

void CIPlayerObject::setVehicleSecondColor(ccColor3B const& color) {
    m_vehicleSpriteSecondary->setColor(color);
}

void CIPlayerObject::enableCustomVehicleGlowColor(ccColor3B const& color) {
    m_fields->m_hasCustomVehicleGlowColor = true;
    m_fields->m_vehicleGlowColor = color;
}

void CIPlayerObject::setColorsCI(bool isVehicle, ccColor3B const& color1, ccColor3B const& color2) {
    if (isVehicle) {
        setVehicleColor(color1);
        setVehicleSecondColor(color2);
    } else {
        PlayerObject::setColor(color1);
        PlayerObject::setSecondColor(color2);
    }
}

void CIPlayerObject::setColorsCI(bool isVehicle, int color1, int color2) {
    setColorsCI(isVehicle, GameManager::get()->colorForIdx(color1), GameManager::get()->colorForIdx(color2));
}

void CIPlayerObject::setGlowColorCI(bool isVehicle, bool enable, ccColor3B const& color) {
    if (isVehicle) {
        m_fields->m_hasVehicleGlow = enable || (m_fields->m_currentVehicleColor == cc3x(0x0));
        enableCustomVehicleGlowColor(color);
    } else {
        m_hasGlow = enable || (m_fields->m_currentColor == cc3x(0x0));
        PlayerObject::enableCustomGlowColor(color);
    }
    PlayerObject::updateGlowColor();
    PlayerObject::updatePlayerGlow();
}

void CIPlayerObject::setGlowColorCI(bool isVehicle, bool enable, int color) {
    setGlowColorCI(isVehicle, enable, GameManager::get()->colorForIdx(color));
}

CITempProperties& CIPlayerObject::getActiveProperties(IconType type) {
    if (m_fields->m_ciProperties.contains(type))
        return m_fields->m_ciProperties[type];

    return m_fields->s_emptyCIProperty;
}

int CIPlayerObject::getNextIconCI(IconType type, int originalFrame) {
    if (!m_fields->m_levelStarted) return originalFrame;

    auto gm = GameManager::get();
    auto& config = getActiveProperties(type);
    auto isVehicle = type == IconType::Ship || type == IconType::Ufo || type == IconType::Jetpack;

    ccColor3B color1 = gm->colorForIdx(m_fields->m_ogColor1);
    ccColor3B color2 = gm->colorForIdx(m_fields->m_ogColor2);
    ccColor3B glowColor = gm->colorForIdx(m_fields->m_ogGlowColor);
    bool enableGlow = m_fields->m_ogHasGlow;

    if (config.disabled || config.iconSet.empty()) {
        return originalFrame;
    }

    if (config.iconSet.size() == 1) {
        auto const& iconProps = config.iconSet[0];
        if (iconProps.color1)
            color1 = changing_icons::utils::getColorFromVariant(iconProps.color1.value());
        if (iconProps.color2)
            color2 = changing_icons::utils::getColorFromVariant(iconProps.color2.value());
        if (iconProps.overrideGlow) {
            enableGlow = iconProps.glowColor.has_value();
            if (iconProps.glowColor) {
                glowColor = changing_icons::utils::getColorFromVariant(iconProps.glowColor.value());
            }
        }
        return iconProps.iconID;
    }

    int start = 0;
    int end = config.iconSet.size() - 1;
    int result;
    switch (config.order) {
    case IconOrder::Random:
        // https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/
        result = Random::get(start, end);
        config.current = result;
        break;
    case IconOrder::Shuffle: [[fallthrough]];
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
    auto const& iconProps = config.iconSet.at(result);
    newIcon = iconProps.iconID;
    if (iconProps.color1) {
        color1 = changing_icons::utils::getColorFromVariant(iconProps.color1.value());
    }
    if (iconProps.color2) {
        color2 = changing_icons::utils::getColorFromVariant(iconProps.color2.value());
    }
    if (iconProps.overrideGlow) {
        enableGlow = iconProps.glowColor.has_value();
        if (iconProps.glowColor) {
            glowColor = changing_icons::utils::getColorFromVariant(iconProps.glowColor.value());
        }
    }

    std::string_view playerName = "P1";
    if (this == m_gameLayer->m_player2) playerName = "P2";

    log::info("{}: Changed {} icon to ID {} (Index: {}, Order: {})", playerName, type, newIcon, config.current, config.order);
    return newIcon;
}

void CIPlayerObject::refreshColorsCI() {
    if (!m_fields->m_levelStarted) return;
    auto isPlayer2 = this == m_gameLayer->m_player2;
    auto const& config = getActiveProperties(
        changing_icons::utils::getIconTypeFromGamemode(this, false)
    );

    auto enableGlow = m_fields->m_ogHasGlow;
    auto color1 = GameManager::get()->colorForIdx(m_fields->m_ogColor1);
    auto color2 = GameManager::get()->colorForIdx(m_fields->m_ogColor2);
    auto glowColor = GameManager::get()->colorForIdx(m_fields->m_ogGlowColor);

    if (!config.disabled && !config.iconSet.empty()) {
        auto const& icon = config.iconSet.at(config.current);
        if (icon.overrideGlow) enableGlow = icon.glowColor.has_value();
        if (icon.color1)
            color1 = changing_icons::utils::getColorFromVariant(icon.color1.value());
        if (icon.color2)
            color2 = changing_icons::utils::getColorFromVariant(icon.color2.value());
        if (icon.glowColor)
            glowColor = changing_icons::utils::getColorFromVariant(icon.glowColor.value());
    }
    if ((CIManager::get()->getGlobalConfig().flipP2Colors || config.disabled) && isPlayer2) {
        setColorsCI(false, color2, color1);
    } else {
        setColorsCI(false, color1, color2);
    }
    setGlowColorCI(false, enableGlow, glowColor);

    auto hasVehicle = m_isShip || m_isBird;
    if (hasVehicle) {
        auto const& vehConfig = getActiveProperties(
            changing_icons::utils::getIconTypeFromGamemode(this, true)
        );
        auto enableGlow = m_fields->m_ogHasGlow;
        auto color1 = GameManager::get()->colorForIdx(m_fields->m_ogColor1);
        auto color2 = GameManager::get()->colorForIdx(m_fields->m_ogColor2);
        auto glowColor = GameManager::get()->colorForIdx(m_fields->m_ogGlowColor);

        if (!vehConfig.disabled && !vehConfig.iconSet.empty()) {
            auto const& icon = vehConfig.iconSet[vehConfig.current];
            if (icon.overrideGlow) enableGlow = icon.glowColor.has_value();
            if (icon.color1)
                color1 = changing_icons::utils::getColorFromVariant(icon.color1.value());
            if (icon.color2)
                color2 = changing_icons::utils::getColorFromVariant(icon.color2.value());
            if (icon.glowColor)
                glowColor = changing_icons::utils::getColorFromVariant(icon.glowColor.value());
        }
        if ((CIManager::get()->getGlobalConfig().flipP2Colors || vehConfig.disabled) && isPlayer2) {
            setColorsCI(true, color2, color1);
        } else {
            setColorsCI(true, color1, color2);
        }
        setGlowColorCI(true, enableGlow, glowColor);
    }
}