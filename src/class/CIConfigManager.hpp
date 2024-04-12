#pragma once
#include <properties/ConfigProperties.hpp>

namespace changing_icons {
    class IconConfigLayer;

    class CIManager : public cocos2d::CCObject {
    protected:
        inline static CIManager* s_sharedInstance = nullptr;
        IconConfigLayer* m_configLayer;
        CIGlobalProperties m_globalConfig;
        std::unordered_map<IconType, CITabProperties> m_iconConfigs;
        bool init();
        void setConfigLayer(IconConfigLayer* configLayer);
    public:
        // Get the shared instance of CIConfigManager
        static CIManager* get();
        IconConfigLayer* getConfigLayer();
        CIGlobalProperties& getGlobalConfig();
        CITabProperties& getConfig(IconType type);
        std::unordered_map<IconType, CITabProperties>& getConfigMap();
        std::vector<IconProperties> generateIconKitList(IconType type);
        static ghc::filesystem::path getPresetDir(IconType type);

        friend class IconConfigLayer;
    };
}