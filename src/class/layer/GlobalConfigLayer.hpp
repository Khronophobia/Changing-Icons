#pragma once
#include <Geode/ui/Popup.hpp>
#include <CIConfigProperties.hpp>

namespace changing_icons {
    class CIConfigManager;

    class GlobalConfigLayer : public geode::Popup<> {
    protected:
        CIConfigManager* m_configManager;
        cocos2d::CCLabelBMFont* m_iconOrderLabel;
        bool m_iconOrderEnabled;
        std::array<std::string, 3> const m_iconOrderList{"Random", "Forward", "Backward"};
        cocos2d::CCMenu* m_gamemodeBar;

        bool setup();
        void setOrderChoice(IconOrder choice);
        void setOrderChoice(int choice);
        void disableOrder();
    public:
        static GlobalConfigLayer* create();
        void onVarTriToggle(CCObject* sender);
        void onOrderArrow(CCObject* sender);
        void onDisableOrder(CCObject* sender);
        void onAddToOverride(CCObject* sender);
        ~GlobalConfigLayer();
    };
}