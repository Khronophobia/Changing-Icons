#pragma once
#include <properties/ConfigProperties.hpp>

namespace changing_icons {
    class CIManager : public cocos2d::CCObject {
    protected:
        inline static CIManager* s_sharedInstance = nullptr;
        CIGlobalProperties m_globalConfig;
        std::unordered_map<IconType, CITabProperties> m_iconConfigs;
        bool init();
    public:
        // Get the shared instance of CIConfigManager
        static CIManager* get();
        CIGlobalProperties& getGlobalConfig();
        CITabProperties& getConfig(IconType type);
        std::unordered_map<IconType, CITabProperties>& getConfigMap();
        std::vector<IconProperties> generateIconKitList(IconType type);
        static ghc::filesystem::path getPresetDir(IconType type);
    };
}