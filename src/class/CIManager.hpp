#pragma once
#include <settings/TabSettings.hpp>
#include <settings/GlobalSettings.hpp>

namespace ci {

class CIManager : public cocos2d::CCObject {
public:
    static CIManager* get();
    GlobalSettings& getGlobalSetting();
    TabSettings& getSetting(CITab type);
    std::unordered_map<CITab, TabSettings>& getSettingsMap();
    void saveData() const;
protected:
    CIManager();
protected:
    inline static CIManager* s_instance = nullptr;
    GlobalSettings m_globalSettings;
    std::unordered_map<CITab, TabSettings> m_gamemodeSettings;
};

} // namespace ci