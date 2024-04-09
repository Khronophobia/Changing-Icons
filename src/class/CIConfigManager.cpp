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

    return true;
}

CIGlobalProperties& CIManager::getGlobalConfig() {
    return m_globalConfig;
}

CITabProperties& CIManager::getConfig(IconType type) {
    return m_iconConfigs[type];
}

std::unordered_map<IconType, CITabProperties>& CIManager::getConfigMap() {
    return m_iconConfigs;
}

std::vector<IconProperties> CIManager::generateIconKitList(IconType type) {
    auto disableLockedIcons = Mod::get()->getSettingValue<bool>("disable-locked-icons");
    std::vector<IconProperties> iconKitList;
    for (int id = 1; id <= GameManager::get()->countForType(type); id++) {
        if (GameManager::get()->isIconUnlocked(id, type) || !disableLockedIcons)
            iconKitList.push_back(IconProperties{.iconID = id});
    }

    return iconKitList;
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