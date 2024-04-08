#include "CIConfigManager.hpp"

using namespace geode::prelude;
using namespace changing_icons;

CIManager* CIManager::get() {
    if (!s_sharedInstance) {
        s_sharedInstance = new CIManager();
        s_sharedInstance->init();
    }

    return s_sharedInstance;
}

bool CIManager::init() {
    m_globalConfig = Mod::get()->getSavedValue<CIGlobalProperties>("global");
    m_iconConfigs = {
        {IconType::Cube, Mod::get()->getSavedValue<CITabProperties>("cube")},
        {IconType::Ship, Mod::get()->getSavedValue<CITabProperties>("ship")},
        {IconType::Ball, Mod::get()->getSavedValue<CITabProperties>("ball")},
        {IconType::Ufo, Mod::get()->getSavedValue<CITabProperties>("bird")},
        {IconType::Wave, Mod::get()->getSavedValue<CITabProperties>("dart")},
        {IconType::Robot, Mod::get()->getSavedValue<CITabProperties>("robot")},
        {IconType::Spider, Mod::get()->getSavedValue<CITabProperties>("spider")},
        {IconType::Swing, Mod::get()->getSavedValue<CITabProperties>("swing")},
        {IconType::Jetpack, Mod::get()->getSavedValue<CITabProperties>("jetpack")}
    };
    /* m_cubeConfig = Mod::get()->getSavedValue<CITabProperties>("cube");
    m_shipConfig = Mod::get()->getSavedValue<CITabProperties>("ship");
    m_ballConfig = Mod::get()->getSavedValue<CITabProperties>("ball");
    m_birdConfig = Mod::get()->getSavedValue<CITabProperties>("bird");
    m_dartConfig = Mod::get()->getSavedValue<CITabProperties>("dart");
    m_robotConfig = Mod::get()->getSavedValue<CITabProperties>("robot");
    m_spiderConfig = Mod::get()->getSavedValue<CITabProperties>("spider");
    m_swingConfig = Mod::get()->getSavedValue<CITabProperties>("swing");
    m_jetpackConfig = Mod::get()->getSavedValue<CITabProperties>("jetpack"); */

    return true;
}

CIGlobalProperties& CIManager::getGlobalConfig() {
    return m_globalConfig;
}

CITabProperties& CIManager::getConfig(IconType type) {
    /* switch (type) {
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
    } */
    return m_iconConfigs.at(type);
}

std::unordered_map<IconType, CITabProperties>& CIManager::getConfigMap() {
    return m_iconConfigs;
}

void CIManager::refreshUnlockedIcons(IconType type) {
    std::vector<int> unlockedIcons;
    for (int id = 1; id <= GameManager::get()->countForType(type); id++) {
        if (GameManager::get()->isIconUnlocked(id, type))
            unlockedIcons.push_back(id);
    }
    if (m_unlockedIcons.contains(type))
        m_unlockedIcons[type] = unlockedIcons;
    else
        m_unlockedIcons.insert({type, unlockedIcons});
}

std::vector<int> CIManager::getUnlockedIcons(IconType type) {
    if (m_unlockedIcons.contains(type)) return m_unlockedIcons[type];

    return {};
}

ghc::filesystem::path CIManager::getPresetDir(IconType type) {
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