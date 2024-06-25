#include "CCMenuItemTogglerEx.hpp"

namespace khronos {

CCMenuItemTogglerEx* CCMenuItemTogglerEx::create(
    cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto ret = new (std::nothrow) CCMenuItemTogglerEx();
    if (ret && ret->init(offSpr, onSpr, target, callback, name, labelOffset)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCMenuItemTogglerEx* CCMenuItemTogglerEx::createWithCallback(
    cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
    geode::utils::MiniFunction<void(CCMenuItemTogglerEx*)>&& callback,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto btn = CCMenuItemTogglerEx::create(offSpr, onSpr, nullptr, nullptr, name, labelOffset);
    geode::cocos::CCMenuItemExt::assignCallback(btn, std::move(callback));
    return btn;
}

CCMenuItemTogglerEx* CCMenuItemTogglerEx::createWithStandardSprites(
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    auto btn = CCMenuItemTogglerEx::create(offSpr, onSpr, target, callback, name, labelOffset);
    btn->setScale(scale);
    return btn;
}

CCMenuItemTogglerEx* CCMenuItemTogglerEx::createStandardSpritesWithCallback(
    geode::utils::MiniFunction<void(CCMenuItemTogglerEx*)>&& callback, float scale,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    auto offSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    auto btn = CCMenuItemTogglerEx::createWithCallback(offSpr, onSpr, std::move(callback), name, labelOffset);
    btn->setScale(scale);
    return btn;
}

bool CCMenuItemTogglerEx::init(
    cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
    char const* name, cocos2d::CCPoint const& labelOffset
) {
    if (!CCMenuItem::initWithTarget(target, callback)) return false;

    m_labelOffset = labelOffset;
    m_btn = CCMenuItemToggler::create(offSpr, onSpr, nullptr, nullptr);
    setContentSize(m_btn->getContentSize());
    m_btn->setPosition(m_obContentSize * 0.5f);
    addChild(m_btn);

    if (name) {
        m_label = cocos2d::CCLabelBMFont::create(name, "bigFont.fnt");
        m_label->setScale(0.8f);
        m_label->setAnchorPoint(ccp(0.f, 0.5f));
        m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
        addChild(m_label);
    }

    return true;
}

CCMenuItemToggler* CCMenuItemTogglerEx::getBtn() const noexcept {
    return m_btn;
}

bool CCMenuItemTogglerEx::getState() const {
    return m_btn->isToggled();
}

bool CCMenuItemTogglerEx::isToggled() const {
    return m_btn->isToggled();
}

void CCMenuItemTogglerEx::setState(bool state) {
    m_btn->toggle(state);
}

void CCMenuItemTogglerEx::setStateWithCallback(bool state) {
    m_btn->toggleWithCallback(state);
}

void CCMenuItemTogglerEx::toggle(bool val) {
    m_btn->toggle(val);
}

void CCMenuItemTogglerEx::updateSpriteShallow() {
    setContentSize(m_btn->getScaledContentSize());
    m_btn->setPosition(m_obContentSize * 0.5f);
    if (m_label) m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
}

void CCMenuItemTogglerEx::updateSprite() {
    m_btn->updateSprite();
    updateSpriteShallow();
}

void CCMenuItemTogglerEx::setLabelOffset(cocos2d::CCPoint const& offset) {
    m_labelOffset = offset;
    if (m_label) m_label->setPosition(m_obContentSize * 0.5f + m_labelOffset);
}

void CCMenuItemTogglerEx::setLabel(char const* name) {
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

void CCMenuItemTogglerEx::activate() {
    m_btn->activate();
    updateSpriteShallow();
    CCMenuItem::activate();
}

void CCMenuItemTogglerEx::selected() {
    m_btn->selected();
}

void CCMenuItemTogglerEx::unselected() {
    m_btn->unselected();
}

} // namespace khronos