#pragma once
#include <settings/Enums.hpp>
#include <settings/TabSettings.hpp>
#include "IconListView.hpp"

namespace ci {

class CIConfigPopup : public geode::Popup<> {
public:
    static CIConfigPopup* create(bool show = true);
    static CIConfigPopup* instance();
protected:
    CIConfigPopup(CITab& tab);
    bool setup() override;
    void refreshTab();
    TabSettings& currentSetting();
    void onGamemodeTab(cocos2d::CCObject*);
    void onSettingCheckbox(cocos2d::CCObject*);
    ~CIConfigPopup();
protected:
    static inline CIConfigPopup* s_instance{};
    CITab& m_currentTab;
    cocos2d::CCMenu* m_tabNavMenu{};
    CCMenuItemToggler* m_useAllCheckbox{};
    IconListView* m_iconListView;
};

} // namespace ci