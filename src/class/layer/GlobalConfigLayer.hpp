#pragma once
#include <Geode/ui/Popup.hpp>

namespace changing_icons {
    enum class IconOrder;
    class CIManager;
    class DropdownMenu;

    class GlobalConfigLayer : public geode::Popup<bool> {
    protected:
        CIManager* m_configManager;
        cocos2d::CCLabelBMFont* m_iconOrderLabel;
        DropdownMenu* m_iconOrderDropdown;
        cocos2d::CCMenu* m_gamemodeBar;

        bool setup(bool fromModSettings) override;
    public:
        static GlobalConfigLayer* create(bool fromModSettings);
        void onTriTogglerInfo(CCObject* sender);
        void onVarTriToggle(CCObject* sender);
        void onVarToggle(CCObject* sender);
        void onOrderDropdown(CCObject* sender);
        void onAddToOverride(CCObject* sender);
        void onModSettings(CCObject* sender);
        ~GlobalConfigLayer();
    };
}