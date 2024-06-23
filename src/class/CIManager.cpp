#include "CIManager.hpp"

namespace ci {

CIManager::CIManager() {
    m_globalSettings = geode::Mod::get()->getSavedValue<GlobalSettings>("global");
    m_gamemodeSettings = {
        {CITab::Cube, geode::Mod::get()->getSavedValue<TabSettings>("cube")},
        {CITab::Ship, geode::Mod::get()->getSavedValue<TabSettings>("ship")},
        {CITab::Ball, geode::Mod::get()->getSavedValue<TabSettings>("ball")},
        {CITab::UFO, geode::Mod::get()->getSavedValue<TabSettings>("ufo")},
        {CITab::Wave, geode::Mod::get()->getSavedValue<TabSettings>("wave")},
        {CITab::Robot, geode::Mod::get()->getSavedValue<TabSettings>("robot")},
        {CITab::Spider, geode::Mod::get()->getSavedValue<TabSettings>("spider")},
        {CITab::Swing, geode::Mod::get()->getSavedValue<TabSettings>("swing")},
        {CITab::Jetpack, geode::Mod::get()->getSavedValue<TabSettings>("jetpack")}
    };
}

CIManager* CIManager::get() {
    if (!s_instance) {
        s_instance = new (std::nothrow) CIManager();
    }

    return s_instance;
}

GlobalSettings& CIManager::getGlobalSetting() {
    return m_globalSettings;
}

TabSettings& CIManager::getSetting(CITab type) {
    return m_gamemodeSettings[type];
}

std::unordered_map<CITab, TabSettings>& CIManager::getSettingsMap() {
    return m_gamemodeSettings;
}

void CIManager::saveData() const {
    geode::Mod::get()->setSavedValue("global", m_globalSettings);

    geode::Mod::get()->setSavedValue("cube", m_gamemodeSettings.at(CITab::Cube));
    geode::Mod::get()->setSavedValue("ship", m_gamemodeSettings.at(CITab::Ship));
    geode::Mod::get()->setSavedValue("ball", m_gamemodeSettings.at(CITab::Ball));
    geode::Mod::get()->setSavedValue("ufo", m_gamemodeSettings.at(CITab::UFO));
    geode::Mod::get()->setSavedValue("wave", m_gamemodeSettings.at(CITab::Wave));
    geode::Mod::get()->setSavedValue("robot", m_gamemodeSettings.at(CITab::Robot));
    geode::Mod::get()->setSavedValue("spider", m_gamemodeSettings.at(CITab::Spider));
    geode::Mod::get()->setSavedValue("swing", m_gamemodeSettings.at(CITab::Swing));
    geode::Mod::get()->setSavedValue("jetpack", m_gamemodeSettings.at(CITab::Jetpack));
}

} // namespace ci