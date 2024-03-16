#pragma once
#include <Geode/ui/Popup.hpp>
#include "../IconConfigProperties.hpp"

namespace changing_icons {
    class IconConfigLayer : public geode::Popup<> {
    protected:
        IconType m_activeTab;
        cocos2d::CCMenu* m_gamemodeBar;
        cocos2d::CCLayerColor* m_iconList;
        CCMenuItemToggler* m_randomBtn;
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
        bool setup();
    public:
        static IconConfigLayer* create();
        IconConfigData& getCurrentConfig(IconType activeTab);
        void onSwitchTab(CCObject*);
        void onVarToggle(CCObject*);
        void onClose(CCObject*);
    };
}