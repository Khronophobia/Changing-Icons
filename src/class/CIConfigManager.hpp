#pragma once
#include <CIConfigProperties.hpp>

namespace changing_icons {
    class CIConfigManager : public cocos2d::CCObject {
    protected:
        inline static CIConfigManager* m_sharedInstance = nullptr;
        CIGlobalProperties m_globalConfig;
        CITabProperties m_cubeConfig;
        CITabProperties m_shipConfig;
        CITabProperties m_ballConfig;
        CITabProperties m_birdConfig;
        CITabProperties m_dartConfig;
        CITabProperties m_robotConfig;
        CITabProperties m_spiderConfig;
        CITabProperties m_swingConfig;
        CITabProperties m_jetpackConfig;
        bool init();
    public:
        // Get the shared instance of CIConfigManager
        static CIConfigManager* get();
        CIGlobalProperties& getGlobalConfig();
        CITabProperties& getConfig(IconType type);
        static ghc::filesystem::path getPresetDir(IconType type);
    };
}