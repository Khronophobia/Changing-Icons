#pragma once
#include "../IconConfigProperties.hpp"

namespace changing_icons {
    class CIConfigManager : public cocos2d::CCObject {
    protected:
        inline static CIConfigManager* m_sharedInstance = nullptr;
        GlobalConfigData m_globalConfig;
        IconConfigData m_cubeConfig;
        IconConfigData m_shipConfig;
        IconConfigData m_ballConfig;
        IconConfigData m_birdConfig;
        IconConfigData m_dartConfig;
        IconConfigData m_robotConfig;
        IconConfigData m_spiderConfig;
        IconConfigData m_swingConfig;
        IconConfigData m_jetpackConfig;
        bool init();
    public:
        // Get the shared instance of CIConfigManager
        static CIConfigManager* get();
        GlobalConfigData& getGlobalConfig();
        IconConfigData& getConfig(IconType type);
    };
}