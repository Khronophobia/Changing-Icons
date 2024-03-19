#pragma once
#include <Geode/ui/Popup.hpp>
#include "../IconConfigProperties.hpp"
#include "../class/CIConfigManager.hpp"

namespace changing_icons {
    class IconConfigLayer : public geode::Popup<> {
    private:
        static inline IconConfigLayer* m_instance = nullptr;
    protected:
        IconType m_currentTab;
        cocos2d::CCMenu* m_gamemodeBar;
        geode::ScrollLayer* m_iconList;
        geode::Scrollbar* m_iconListScrollbar;
        CCMenuItemToggler* m_randomBtn;
        CCMenuItemToggler* m_disableBtn;
        CCMenuItemToggler* m_mirrorEndBtn;
        CIConfigManager* m_configManager;
        std::vector<std::string> const m_iconOrderList{"Random", "Down", "Up"};
        cocos2d::CCLabelBMFont* m_iconOrderLabel;
        bool setup();
        void refreshTab();
        void setOrderChoice(IconOrder choice);
        void setOrderChoice(int choice);
        void refreshIconList(IconType currentTab, bool toTop = false);
        ~IconConfigLayer();
    public:
        static IconConfigLayer* getInstance();
        static IconConfigLayer* create();
        IconConfigData& getCurrentConfig();
        void onSwitchTab(CCObject*);
        void onVarInfo(CCObject*);
        void onVarToggle(CCObject*);
        void onOrderArrow(CCObject*);
        void onAddIcon(CCObject*);
        void onClearList(CCObject*);
        void addIcon(IconProperties properties);
        void swapIcons(int icon1, int icon2);
        void deleteIcon(int index);
    };
}