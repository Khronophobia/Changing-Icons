#include "GJBaseGameLayer.hpp"
#include <class/CIConfigManager.hpp>

using namespace geode::prelude;
using namespace changing_icons;

bool CIBaseGameLayer::init() {
    if (!GJBaseGameLayer::init()) return false;

    if (!Mod::get()->getSettingValue<bool>("disable-locked-icons")) return true;

    for (auto const& pair : CIManager::get()->getConfigMap()) {
        CIManager::get()->refreshUnlockedIcons(pair.first);
    }
    log::info("Unlocked icons refreshed");

    return true;
}