#include "CIUtilities.hpp"

namespace ci::utils {

IconType toIconType(CITab val) {
    switch (val) {
        case CITab::Cube: return IconType::Cube;
        case CITab::Ship: return IconType::Ship;
        case CITab::Ball: return IconType::Ball;
        case CITab::UFO: return IconType::Ufo;
        case CITab::Wave: return IconType::Wave;
        case CITab::Robot: return IconType::Robot;
        case CITab::Spider: return IconType::Spider;
        case CITab::Swing: return IconType::Swing;
        case CITab::Jetpack: return IconType::Jetpack;
    }
}
UnlockType toUnlockType(CITab val) {
    switch (val) {
        case CITab::Cube: return UnlockType::Cube;
        case CITab::Ship: return UnlockType::Ship;
        case CITab::Ball: return UnlockType::Ball;
        case CITab::UFO: return UnlockType::Bird;
        case CITab::Wave: return UnlockType::Dart;
        case CITab::Robot: return UnlockType::Robot;
        case CITab::Spider: return UnlockType::Spider;
        case CITab::Swing: return UnlockType::Swing;
        case CITab::Jetpack: return UnlockType::Jetpack;
    }
}
cocos2d::ccColor3B getPlayerColor(PlayerColor const& var) {
    if (int const* colorID = std::get_if<int>(&var)) {
        return GameManager::get()->colorForIdx(*colorID);
    } else if (cocos2d::ccColor3B const* color = std::get_if<cocos2d::ccColor3B>(&var)) {
        return *color;
    } else {
        return {};
    }
}
int getPlayerIcon(IconType type) {
    switch (type) {
        case IconType::Cube: return GameManager::get()->getPlayerFrame();
        case IconType::Ship: return GameManager::get()->getPlayerShip();
        case IconType::Ball: return GameManager::get()->getPlayerBall();
        case IconType::Ufo: return GameManager::get()->getPlayerBird();
        case IconType::Wave: return GameManager::get()->getPlayerDart();
        case IconType::Robot: return GameManager::get()->getPlayerRobot();
        case IconType::Spider: return GameManager::get()->getPlayerSpider();
        case IconType::Swing: return GameManager::get()->getPlayerSwing();
        case IconType::Jetpack: return GameManager::get()->getPlayerJetpack();
        default: return 0;
    }
}
int getPlayerIcon(CITab type) {
    return getPlayerIcon(toIconType(type));
}

} // namespace ci::utils