#pragma once
#include <Geode/ui/Popup.hpp>

namespace changing_icons {
    enum class IconOrder;
    class CIManager;

    class GlobalConfigLayer : public geode::Popup<> {
    protected:
        CIManager* m_configManager;
        cocos2d::CCLabelBMFont* m_iconOrderLabel;
        cocos2d::CCMenu* m_iconOrderMenu;
        cocos2d::CCMenu* m_gamemodeBar;

        bool setup() override;
        void setOrderChoice(IconOrder choice);
        void setOrderChoice(int choice);
    public:
        static GlobalConfigLayer* create();
        void onTriTogglerInfo(CCObject* sender);
        void onVarTriToggle(CCObject* sender);
        void onOrderButton(CCObject* sender);
        void onAddToOverride(CCObject* sender);
        void onModSettings(CCObject* sender);
        ~GlobalConfigLayer();
    };
}