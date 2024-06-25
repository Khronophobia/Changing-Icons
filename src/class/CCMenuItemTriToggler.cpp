#include "CCMenuItemTriToggler.hpp"

namespace khronos {

CCMenuItemTriToggler* CCMenuItemTriToggler::create(
    cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto ret = new (std::nothrow) CCMenuItemTriToggler();
    if (ret && ret->init(disabledSpr, offSpr, onSpr, target, callback, name, labelOffset)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCMenuItemTriToggler* CCMenuItemTriToggler::createWithCallback(
    cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
    geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto item = CCMenuItemTriToggler::create(disabledSpr, offSpr, onSpr, nullptr, nullptr, name, labelOffset);
    geode::cocos::CCMenuItemExt::assignCallback(item, std::move(callback));
    return item;
}

CCMenuItemTriToggler* CCMenuItemTriToggler::createWithStandardSprites(
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto disabledSpr = cocos2d::CCSprite::create("CI_checkDisabled.png"_spr);
    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    auto btn = CCMenuItemTriToggler::create(disabledSpr, offSpr, onSpr, target, callback, name, labelOffset);
    btn->setScale(scale);
    return btn;
}

CCMenuItemTriToggler* CCMenuItemTriToggler::createStandardSpritesWithCallback(
    geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback, float scale,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto disabledSpr = cocos2d::CCSprite::create("CI_checkDisabled.png"_spr);
    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    auto btn = CCMenuItemTriToggler::createWithCallback(disabledSpr, offSpr, onSpr, std::move(callback), name, labelOffset);
    btn->setScale(scale);
    return btn;
}

bool CCMenuItemTriToggler::init(
    cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
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

    m_labelOffset = labelOffset;
    if (name) {
        m_label = cocos2d::CCLabelBMFont::create(name, "bigFont.fnt");
        m_label->setScale(0.8f);
        m_label->setAnchorPoint(ccp(0.f, 0.5f));
        m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
        addChild(m_label);
    }

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

void CCMenuItemTriToggler::setStateWithCallback(khronos::tribool state) {
    setState(state);
    activate();
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
    if (m_label) m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
}

void CCMenuItemTriToggler::updateSprite() {
    m_disabledBtn->updateSprite();
    m_offBtn->updateSprite();
    m_onBtn->updateSprite();

    updateContentSize();
}

void CCMenuItemTriToggler::setLabelOffset(cocos2d::CCPoint const& offset) {
    m_labelOffset = offset;
    if (m_label) m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
}

void CCMenuItemTriToggler::setLabel(char const* name) {
    if (name) {
        if (m_label) {
            m_label->setString(name);
        } else {
            m_label = cocos2d::CCLabelBMFont::create(name, "bigFont.fnt");
        }
        m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
    } else {
        if (m_label) m_label->removeFromParent();
        m_label = nullptr;
    }
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