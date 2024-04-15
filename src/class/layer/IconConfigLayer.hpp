#pragma once
#include <class/IconCellDelegate.hpp>

namespace changing_icons {
    enum class IconOrder;
    struct CITabProperties;
    struct IconProperties;
    class CIManager;
    class DropdownMenu;

    class IconConfigLayer : public geode::Popup<bool>, IconCellDelegate {
    private:
        enum MoveTo {
            Ignore = -1,
            ToBottom = 0,
            ToTop = 1,
        };
    protected:
        bool m_fromModSettings;
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
        
        bool setup(bool fromModSettings) override;
        void refreshTab();
        void refreshIconList(IconType currentTab, int moveTo = Ignore, bool isRemove = false);
        ~IconConfigLayer();
    public:
        static IconConfigLayer* create(bool fromModSettings = false);
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

        void editIcon(int index) override;
        void swapIcons(int icon1, int icon2) override;
        void deleteIcon(int index) override;
        void addIcon(IconProperties properties);
        void replaceIcon(IconProperties properties, int index);
        void replaceList(std::vector<IconProperties> const& list);
    };
}