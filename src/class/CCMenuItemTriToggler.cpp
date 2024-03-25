#include "CCMenuItemTriToggler.hpp"

using namespace geode::prelude;
using namespace changing_icons;

CCMenuItemTriToggler* CCMenuItemTriToggler::create(CCNode* disabledNode, CCNode* offNode, CCNode* onNode, CCObject* target, SEL_MenuHandler callback, float scale) {
    auto ret = new CCMenuItemTriToggler();
    if (ret && ret->init(disabledNode, offNode, onNode, target, callback, nullptr, scale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCMenuItemTriToggler* CCMenuItemTriToggler::createWithLabel(CCNode* disabledNode, CCNode* offNode, CCNode* onNode, CCObject* target, cocos2d::SEL_MenuHandler callback, char const* text, float scale) {
    auto ret = new CCMenuItemTriToggler();
    if (ret && ret->init(disabledNode, offNode, onNode, target, callback, text, scale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CCMenuItemTriToggler::init(CCNode* disabledNode, CCNode* offNode, CCNode* onNode, CCObject* target, SEL_MenuHandler callback, char const* text, float scale) {
    if (!CCMenuItem::initWithTarget(target, callback)) return false;

    disabledNode->setScale(scale);
    offNode->setScale(scale);
    onNode->setScale(scale);
    m_disabledButton = CCMenuItemSpriteExtra::create(disabledNode, nullptr, nullptr);
    this->addChild(m_disabledButton);
    m_offButton = CCMenuItemSpriteExtra::create(offNode, nullptr, nullptr);
    this->addChild(m_offButton);
    m_onButton = CCMenuItemSpriteExtra::create(onNode, nullptr, nullptr);
    this->addChild(m_onButton);

    if (text) {
        m_label = CCLabelBMFont::create(text, "bigFont.fnt");
        m_label->setAnchorPoint(ccp(0.f, 0.5f));
        m_label->setScale(0.85f * scale);
        this->addChildAtPosition(m_label, Anchor::Center, ccp(24.f, 0.f) * scale);
    }

    this->setVisibleButton();

    return true;
}

CCMenuItemSpriteExtra* CCMenuItemTriToggler::getActiveButton() {
    if (!m_state) return m_disabledButton;
    else if (!m_state.value()) return m_offButton;
    else return m_onButton;
}

void CCMenuItemTriToggler::setVisibleButton() {
    if (!m_state) {
        this->setContentSize(m_disabledButton->getContentSize());
        m_disabledButton->setVisible(true);
        m_offButton->setVisible(false);
        m_onButton->setVisible(false);
        if (m_label) m_label->setColor(cc3x(0x7f));
    } else if (!m_state.value()) {
        this->setContentSize(m_offButton->getContentSize());
        m_offButton->setVisible(true);
        m_disabledButton->setVisible(false);
        m_onButton->setVisible(false);
        if (m_label) m_label->setColor(cc3x(0xf));
    } else {
        this->setContentSize(m_onButton->getContentSize());
        m_onButton->setVisible(true);
        m_disabledButton->setVisible(false);
        m_offButton->setVisible(false);
        if (m_label) m_label->setColor(cc3x(0xf));
    }
    m_disabledButton->setPosition(m_obContentSize / 2);
    m_offButton->setPosition(m_obContentSize / 2);
    m_onButton->setPosition(m_obContentSize / 2);
    this->updateLayout();
}

void CCMenuItemTriToggler::setState(std::optional<bool> state) {
    m_state = state;
    this->setVisibleButton();
}

std::optional<bool> CCMenuItemTriToggler::getState() {
    return m_state;
}

void CCMenuItemTriToggler::toggle(std::optional<bool> value) {
    setState(value);
}

std::optional<bool> CCMenuItemTriToggler::isToggled() {
    return getState();
}

void CCMenuItemTriToggler::activate() {
    if (!m_state) setState(false);
    else if (!m_state.value()) setState(true);
    else setState(std::nullopt);

    CCMenuItem::activate();
}

void CCMenuItemTriToggler::selected() {
    this->getActiveButton()->selected();
}

void CCMenuItemTriToggler::unselected() {
    this->getActiveButton()->unselected();
}