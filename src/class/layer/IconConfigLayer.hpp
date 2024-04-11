#pragma once
#include <Geode/ui/Popup.hpp>

namespace changing_icons {
    enum class IconOrder;
    struct CITabProperties;
    struct IconProperties;
    class CIManager;
    class DropdownMenu;

    class IconConfigLayer : public geode::Popup<> {
    private:
        enum MoveTo {
            Ignore = -1,
            ToBottom = 0,
            ToTop = 1,
        };
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
        DropdownMenu* m_iconOrderDropdown;
        
        bool setup() override;
        void refreshTab();
        void refreshIconList(IconType currentTab, int moveTo = Ignore, bool isRemove = false);
        ~IconConfigLayer();
    public:
        static IconConfigLayer* create();
        CITabProperties& getCurrentConfig();

        void onGlobalConfig(CCObject*);
        void onSwitchTab(CCObject*);
        void onVarInfo(CCObject*);
        void onVarToggle(CCObject*);
        void onOrderDropdown(CCObject*);
        void onOrderInfo(CCObject*);
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