#pragma once
#include <Geode/ui/Popup.hpp>
#include "../IconConfigProperties.hpp"

namespace changing_icons {
    class IconConfigLayer : public geode::Popup<> {
    protected:
        IconType m_currentTab;
        cocos2d::CCMenu* m_gamemodeBar;
        geode::ScrollLayer* m_iconList;
        CCMenuItemToggler* m_randomBtn;
        CCMenuItemToggler* m_disableBtn;
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
        std::vector<std::string> const m_iconOrderList{"Random", "Down", "Up"};
        cocos2d::CCLabelBMFont* m_iconOrderLabel;
        bool setup();
        void refreshTab();
        void setOrderChoice(IconOrder choice);
        void setOrderChoice(int choice);
        ~IconConfigLayer();
        static inline IconConfigLayer* m_instance = nullptr;
    public:
        static IconConfigLayer* getInstance();
        static IconConfigLayer* create();
        IconConfigData& getCurrentConfig(IconType currentTab);
        void onSwitchTab(CCObject*);
        void onVarInfo(CCObject*);
        void onVarToggle(CCObject*);
        void onOrderArrow(CCObject*);
    };
}