#pragma once
#include <settings/Enums.hpp>
#include <settings/TabSettings.hpp>
#include "IconListView.hpp"
#include "CCMenuItemTogglerEx.hpp"

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
    khronos::CCMenuItemTogglerEx* addCheckbox(bool TabSettings::* memberPtr, char const* name, cocos2d::Anchor anchor, cocos2d::CCPoint const& offset = {}, char const* info = nullptr);
    void onGamemodeTab(cocos2d::CCObject*);
    void onSettingCheckbox(cocos2d::CCObject*);
    ~CIConfigPopup();
protected:
    static inline CIConfigPopup* s_instance{};
    CITab& m_currentTab;
    cocos2d::CCMenu* m_tabNavMenu{};
    geode::cocos::CCArrayExt<khronos::CCMenuItemTogglerEx*> m_checkboxList;
    IconListView* m_iconListView;
};

} // namespace ci