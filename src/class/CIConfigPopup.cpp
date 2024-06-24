#include "CIConfigPopup.hpp"
#include "CIManager.hpp"
#include "CCReference.hpp"
#include <CIConstants.hpp>

using namespace geode::prelude;

namespace ci {

template<typename T, typename Class>
using CCMemberPtr = ObjWrapper<T Class::*>;

CIConfigPopup::CIConfigPopup(CITab& tab)
    : m_currentTab(tab) {}

CIConfigPopup* CIConfigPopup::create(bool show) {
    auto ret = new (std::nothrow) CIConfigPopup(ci::CIManager::get()->getGlobalSetting().currentTab);
    if (ret && ret->initAnchored(350.f, 285.f, "GJ_square05.png")) {
        ret->autorelease();
        if (show) ret->show();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CIConfigPopup* CIConfigPopup::instance() {
    return s_instance;
}

bool CIConfigPopup::setup() {
    s_instance = this;
    m_noElasticity = true;
    setTitle("Changing Icons Config");

    m_tabNavMenu = CCMenu::create();
    m_tabNavMenu->setContentWidth(m_size.width - 30.f);
    m_tabNavMenu->setLayout(RowLayout::create());
    m_mainLayer->addChildAtPosition(m_tabNavMenu, Anchor::Bottom, ccp(0.f, 24.f));

#define ADD_TAB_BUTTON(spr, mode) \
auto __##spr##TabBtn = CCMenuItemToggler::create( \
    CCSprite::createWithSpriteFrameName(GEODE_STR(gj_##spr##Btn_off_001.png)), \
    CCSprite::createWithSpriteFrameName(GEODE_STR(gj_##spr##Btn_on_001.png)), \
    this, menu_selector(CIConfigPopup::onGamemodeTab) \
); \
__##spr##TabBtn->setTag(static_cast<int>(CITab::mode)); \
m_tabNavMenu->addChild(__##spr##TabBtn)

    ADD_TAB_BUTTON(icon, Cube);
    ADD_TAB_BUTTON(ship, Ship);
    ADD_TAB_BUTTON(ball, Ball);
    ADD_TAB_BUTTON(bird, UFO);
    ADD_TAB_BUTTON(dart, Wave);
    ADD_TAB_BUTTON(robot, Robot);
    ADD_TAB_BUTTON(spider, Spider);
    ADD_TAB_BUTTON(swing, Swing);
    ADD_TAB_BUTTON(jetpack, Jetpack);

#undef ADD_TAB_BUTTON

    m_tabNavMenu->updateLayout();

    addCheckbox(&TabSettings::disabled, "Disabled", cocos2d::Anchor::Left, ccp(40.f, 90.f));
    addCheckbox(
        &TabSettings::useAllIcons, "Use All Icons", Anchor::Left, ccp(40.f, 65.f),
        "Use all icons instead of the ones on the list."
    );

    m_iconListView = IconListView::create(currentSetting().iconSet, m_currentTab, 179.9f);
    m_mainLayer->addChildAtPosition(m_iconListView, Anchor::Right, ccp(-80.f, 15.f));

    refreshTab();
    return true;
}

void CIConfigPopup::onGamemodeTab(CCObject* sender) {
    m_currentTab = static_cast<CITab>(sender->getTag());
    refreshTab();
}

TabSettings& CIConfigPopup::currentSetting() {
    return CIManager::get()->getSetting(m_currentTab);
}

CCMenuItemToggler* CIConfigPopup::addCheckbox(bool TabSettings::* memberPtr, char const* name, Anchor anchor, CCPoint const& offset, char const* info) {
    auto btn = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(CIConfigPopup::onSettingCheckbox), 0.6f);
    btn->updateSprite();
    btn->setUserObject(CCMemberPtr<bool, TabSettings>::create(memberPtr));

    m_checkboxList.push_back(btn);
    m_buttonMenu->addChildAtPosition(btn, anchor, offset);

    auto text = CCLabelBMFont::create(name, "bigFont.fnt");
    text->setAnchorPoint(ccp(0.f, 0.5f));
    text->limitLabelWidth(200.f, 0.5f, 0.1f);
    m_mainLayer->addChildAtPosition(text, anchor, offset + ccp(12.f, 0.f));

    if (info) {
        auto infoBtn = CCMenuItemExt::createSpriteExtraWithFrameName(
            "GJ_infoIcon_001.png", 0.4f,
            [name, info](auto) {
                FLAlertLayer::create(name, info, "Ok")->show();
            }
        );
        m_buttonMenu->addChildAtPosition(infoBtn, anchor, offset + ccp(-12.f, 12.f));
    }

    return btn;
}

void CIConfigPopup::refreshTab() {
    for (auto btn : CCArrayExt<CCMenuItemToggler*>(m_tabNavMenu->getChildren())) {
        if (btn->getTag() == static_cast<int>(m_currentTab)) {
            btn->toggle(true);
            btn->setEnabled(false);
            continue;
        }

        btn->setEnabled(true);
        btn->toggle(false);
    }
    for (auto btn : m_checkboxList) {
        auto memberPtr = static_cast<CCMemberPtr<bool, TabSettings>*>(btn->getUserObject())->getValue();
        btn->toggle(currentSetting().*memberPtr);
    }

    m_iconListView->changeView(currentSetting().iconSet, m_currentTab);
}

void CIConfigPopup::onSettingCheckbox(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    auto memberPtr = static_cast<CCMemberPtr<bool, TabSettings>*>(btn->getUserObject())->getValue();

    currentSetting().*memberPtr = !btn->isToggled();
}

CIConfigPopup::~CIConfigPopup() {
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

} // namespace ci