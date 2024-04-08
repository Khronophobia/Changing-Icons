#pragma once
#include <properties/ConfigProperties.hpp>

namespace changing_icons {
    class CIManager : public cocos2d::CCObject {
    protected:
        inline static CIManager* s_sharedInstance = nullptr;
        CIGlobalProperties m_globalConfig;
        std::unordered_map<IconType, CITabProperties> m_iconConfigs;
        /* CITabProperties m_cubeConfig;
        CITabProperties m_shipConfig;
        CITabProperties m_ballConfig;
        CITabProperties m_birdConfig;
        CITabProperties m_dartConfig;
        CITabProperties m_robotConfig;
        CITabProperties m_spiderConfig;
        CITabProperties m_swingConfig;
        CITabProperties m_jetpackConfig; */
        std::unordered_map<IconType, std::vector<int>> m_unlockedIcons;
        bool init();
    public:
        // Get the shared instance of CIConfigManager
        static CIManager* get();
        CIGlobalProperties& getGlobalConfig();
        CITabProperties& getConfig(IconType type);
        std::unordered_map<IconType, CITabProperties>& getConfigMap();
        void refreshUnlockedIcons(IconType type);
        std::vector<int> getUnlockedIcons(IconType type);
        static ghc::filesystem::path getPresetDir(IconType type);
    };
}