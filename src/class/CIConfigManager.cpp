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
    m_globalConfig = Mod::get()->getSavedValue<GlobalConfigData>("global");
    m_cubeConfig = Mod::get()->getSavedValue<IconConfigData>("cube");
    m_shipConfig = Mod::get()->getSavedValue<IconConfigData>("ship");
    m_ballConfig = Mod::get()->getSavedValue<IconConfigData>("ball");
    m_birdConfig = Mod::get()->getSavedValue<IconConfigData>("bird");
    m_dartConfig = Mod::get()->getSavedValue<IconConfigData>("dart");
    m_robotConfig = Mod::get()->getSavedValue<IconConfigData>("robot");
    m_spiderConfig = Mod::get()->getSavedValue<IconConfigData>("spider");
    m_swingConfig = Mod::get()->getSavedValue<IconConfigData>("swing");
    m_jetpackConfig = Mod::get()->getSavedValue<IconConfigData>("jetpack");

    return true;
}

GlobalConfigData& CIConfigManager::getGlobalConfig() {
    return m_globalConfig;
}

IconConfigData& CIConfigManager::getConfig(IconType type) {
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