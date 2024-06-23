#include "CCMenuItemTriToggler.hpp"

namespace khronos {

CCMenuItemTriToggler* CCMenuItemTriToggler::create(cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
    auto ret = new (std::nothrow) CCMenuItemTriToggler();
    if (ret && ret->init(disabledSpr, offSpr, onSpr, target, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCMenuItemTriToggler* CCMenuItemTriToggler::create(cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback) {
    auto item = CCMenuItemTriToggler::create(disabledSpr, offSpr, onSpr, nullptr, nullptr);
    geode::cocos::CCMenuItemExt::assignCallback(item, std::move(callback));
    return item;
}

CCMenuItemTriToggler* CCMenuItemTriToggler::createWithStandardSprites(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale) {
    auto disabledSpr = cocos2d::CCSprite::create("CI_checkDisabled.png"_spr);
    disabledSpr->setScale(scale);
    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    offSpr->setScale(scale);
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    onSpr->setScale(scale);
    return CCMenuItemTriToggler::create(disabledSpr, offSpr, onSpr, target, callback);
}

CCMenuItemTriToggler* CCMenuItemTriToggler::createWithStandardSprites(geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback, float scale) {
    auto disabledSpr = cocos2d::CCSprite::create("CI_checkDisabled.png"_spr);
    disabledSpr->setScale(scale);
    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    offSpr->setScale(scale);
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    onSpr->setScale(scale);
    return CCMenuItemTriToggler::create(disabledSpr, offSpr, onSpr, std::move(callback));
}

bool CCMenuItemTriToggler::init(cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
    if (!CCMenuItem::initWithTarget(target, callback)) return false;

    m_disabledBtn = CCMenuItemSpriteExtra::create(disabledSpr, this, menu_selector(CCMenuItemTriToggler::triggerNull));
    addChild(m_disabledBtn);
    m_offBtn = CCMenuItemSpriteExtra::create(offSpr, this, menu_selector(CCMenuItemTriToggler::triggerFalse));
    m_offBtn->setVisible(false);
    addChild(m_offBtn);
    m_onBtn = CCMenuItemSpriteExtra::create(onSpr, this, menu_selector(CCMenuItemTriToggler::triggerTrue));
    m_onBtn->setVisible(false);
    addChild(m_onBtn);

    m_activeBtn = m_disabledBtn;
    updateContentSize();

    return true;
}

khronos::tribool CCMenuItemTriToggler::getState() const {
    return m_state;
}

void CCMenuItemTriToggler::setState(khronos::tribool state) {
    m_state = state;
    if (m_state) {
        m_activeBtn = m_onBtn;
        m_onBtn->setVisible(true);
        m_offBtn->setVisible(false);
        m_disabledBtn->setVisible(false);
    } else if (!m_state) {
        m_activeBtn = m_offBtn;
        m_offBtn->setVisible(true);
        m_onBtn->setVisible(false);
        m_disabledBtn->setVisible(false);
    } else {
        m_activeBtn = m_disabledBtn;
        m_disabledBtn->setVisible(true);
        m_offBtn->setVisible(false);
        m_onBtn->setVisible(false);
    }

    updateContentSize();
}

khronos::tribool CCMenuItemTriToggler::isToggled() const {
    return getState();
}

void CCMenuItemTriToggler::toggle(khronos::tribool state) {
    setState(state);
}

void CCMenuItemTriToggler::updateContentSize() {
    setContentSize(m_activeBtn->getScaledContentSize());
    m_activeBtn->setPosition(m_obContentSize * 0.5f);
}

void CCMenuItemTriToggler::updateSprite() {
    m_disabledBtn->updateSprite();
    m_offBtn->updateSprite();
    m_onBtn->updateSprite();

    updateContentSize();
}

void CCMenuItemTriToggler::triggerNull(CCObject*) {
    setState(false);
}

void CCMenuItemTriToggler::triggerFalse(CCObject*) {
    setState(true);
}

void CCMenuItemTriToggler::triggerTrue(CCObject*) {
    setState(nullptr);
}

void CCMenuItemTriToggler::activate() {
    m_activeBtn->activate();
    CCMenuItem::activate();
}

void CCMenuItemTriToggler::selected() {
    m_activeBtn->selected();
}

void CCMenuItemTriToggler::unselected() {
    m_activeBtn->unselected();
}

} // namespace ci