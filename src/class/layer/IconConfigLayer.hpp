#pragma once
#include <Geode/ui/Popup.hpp>

namespace changing_icons {
    enum class IconOrder;
    struct CITabProperties;
    struct IconProperties;
    class CIManager;

    class IconConfigLayer : public geode::Popup<> {
    protected:
        IconType m_currentTab;
        cocos2d::CCMenu* m_gamemodeBar;
        geode::ScrollLayer* m_iconList;
        geode::Scrollbar* m_iconListScrollbar;
        CCMenuItemToggler* m_disableBtn;
        CCMenuItemToggler* m_useAllBtn;
        CCMenuItemToggler* m_includePlayerBtn;
        CCMenuItemToggler* m_mirrorEndBtn;
        CIManager* m_configManager;
        cocos2d::CCMenu* m_iconOrderMenu;
        
        bool setup() override;
        void refreshTab();
        void setOrderChoice(IconOrder choice);
        void setOrderChoice(int choice);
        void refreshIconList(IconType currentTab, bool toTop = false);
        ~IconConfigLayer();
    public:
        static IconConfigLayer* create();
        CITabProperties& getCurrentConfig();

        void onGlobalConfig(CCObject*);
        void onSwitchTab(CCObject*);
        void onVarInfo(CCObject*);
        void onVarToggle(CCObject*);
        void onOrderButton(CCObject*);
        void onAddIcon(CCObject*);
        void onClearList(CCObject*);
        void onSaveList(CCObject*);
        void onLoadList(CCObject*);

        void editIconAtIndex(int index);
        void addIcon(IconProperties properties);
        void swapIcons(int icon1, int icon2);
        void replaceIcon(IconProperties properties, int index);
        void deleteIcon(int index);
        void replaceList(std::vector<IconProperties> const& list);
    };
}