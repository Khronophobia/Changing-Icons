#include "CIConfigManager.hpp"

using namespace geode::prelude;
using namespace changing_icons;

CIConfigManager* CIConfigManager::get() {
    if (!m_sharedInstance) {
        m_sharedInstance = new CIConfigManager();
        m_sharedInstance->init();
    }

    return m_sharedInstance;
}

bool CIConfigManager::init() {
    m_globalConfig = Mod::get()->getSavedValue<CIGlobalProperties>("global");
    m_cubeConfig = Mod::get()->getSavedValue<CITabProperties>("cube");
    m_shipConfig = Mod::get()->getSavedValue<CITabProperties>("ship");
    m_ballConfig = Mod::get()->getSavedValue<CITabProperties>("ball");
    m_birdConfig = Mod::get()->getSavedValue<CITabProperties>("bird");
    m_dartConfig = Mod::get()->getSavedValue<CITabProperties>("dart");
    m_robotConfig = Mod::get()->getSavedValue<CITabProperties>("robot");
    m_spiderConfig = Mod::get()->getSavedValue<CITabProperties>("spider");
    m_swingConfig = Mod::get()->getSavedValue<CITabProperties>("swing");
    m_jetpackConfig = Mod::get()->getSavedValue<CITabProperties>("jetpack");

    return true;
}

CIGlobalProperties& CIConfigManager::getGlobalConfig() {
    return m_globalConfig;
}

CITabProperties& CIConfigManager::getConfig(IconType type) {
    switch (type) {
        default:
        case IconType::Cube: return m_cubeConfig;
        case IconType::Ship: return m_shipConfig;
        case IconType::Ball: return m_ballConfig;
        case IconType::Ufo: return m_birdConfig;
        case IconType::Wave: return m_dartConfig;
        case IconType::Robot: return m_robotConfig;
        case IconType::Spider: return m_spiderConfig;
        case IconType::Swing: return m_swingConfig;
        case IconType::Jetpack: return m_jetpackConfig;
    }
}

ghc::filesystem::path CIConfigManager::getPresetDir(IconType type) {
    auto presetDir = Mod::get()->getConfigDir() / "icon-sets";
    switch (type) {
        default:
        case IconType::Cube: return presetDir / "cube";
        case IconType::Ship: return presetDir / "ship";
        case IconType::Ball: return presetDir / "ball";
        case IconType::Ufo: return presetDir / "ufo";
        case IconType::Wave: return presetDir / "wave";
        case IconType::Robot: return presetDir / "robot";
        case IconType::Spider: return presetDir / "spider";
        case IconType::Swing: return presetDir / "swing";
        case IconType::Jetpack: return presetDir / "jetpack";
    }
}