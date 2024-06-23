#include "CIConfigPopup.hpp"
#include "CIManager.hpp"
#include "CCReference.hpp"
#include <CIConstants.hpp>

using namespace geode::prelude;

namespace ci {

CIConfigPopup::CIConfigPopup(CITab& tab)
    : m_currentTab(tab) {}

CIConfigPopup* CIConfigPopup::create(bool show) {
    auto ret = new (std::nothrow) CIConfigPopup(ci::CIManager::get()->getGlobalSetting().currentTab);
    if (ret && ret->initAnchored(400.f, 280.f, "GJ_square05.png")) {
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

    m_useAllCheckbox = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(CIConfigPopup::onSettingCheckbox), 0.7f
    );
    m_buttonMenu->addChildAtPosition(m_useAllCheckbox, Anchor::Left);

    m_iconListView = IconListView::create(currentSetting().iconSet, m_currentTab, 180.f);
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

template<typename T>
static void setReference(CCNode* node, T& ref) {
    if (auto refObj = dynamic_cast<khronos::CCReference<T>*>(node->getUserObject())) {
        refObj->reseat(ref);
    } else {
        node->setUserObject(khronos::CCReference<T>::create(ref));
    }
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

    m_useAllCheckbox->toggle(currentSetting().useAllIcons);
    setReference(m_useAllCheckbox, currentSetting().useAllIcons);

    m_iconListView->changeView(currentSetting().iconSet, m_currentTab);
}

void CIConfigPopup::onSettingCheckbox(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    auto refObj = static_cast<khronos::CCReference<bool>*>(btn->getUserObject());

    // Had to negate the bool because the actual bool gets changed after the callback
    refObj->get() = !btn->isToggled();
}

CIConfigPopup::~CIConfigPopup() {
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

} // namespace ci