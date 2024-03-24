#include "CCMenuItemTriToggler.hpp"

using namespace geode::prelude;
using namespace changing_icons;

CCMenuItemTriToggler* CCMenuItemTriToggler::create(CCNode* disabled, CCNode* off, CCNode* on, CCObject* target, SEL_MenuHandler callback, float scale) {
    auto ret = new CCMenuItemTriToggler();
    if (ret && ret->init(disabled, off, on, target, callback, scale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CCMenuItemTriToggler::init(CCNode* disabledNode, CCNode* offNode, CCNode* onNode, CCObject* target, SEL_MenuHandler callback, float scale) {
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

    this->setVisibleButton();

    return true;
}

CCMenuItemSpriteExtra* CCMenuItemTriToggler::getActiveButton() {
    switch (m_state) {
        default:
        case -1: return m_disabledButton;
        case 0: return m_offButton;
        case 1: return m_onButton;
    }
}

void CCMenuItemTriToggler::setVisibleButton() {
    switch (m_state) {
    default:
    case -1:
        this->setContentSize(m_disabledButton->getContentSize());
        m_disabledButton->setVisible(true);
        m_offButton->setVisible(false);
        m_onButton->setVisible(false);
        break;
    case 0:
        this->setContentSize(m_offButton->getContentSize());
        m_offButton->setVisible(true);
        m_disabledButton->setVisible(false);
        m_onButton->setVisible(false);
        break;
    case 1:
        this->setContentSize(m_onButton->getContentSize());
        m_onButton->setVisible(true);
        m_disabledButton->setVisible(false);
        m_offButton->setVisible(false);
        break;
    }
    m_disabledButton->setPosition(m_obContentSize / 2);
    m_offButton->setPosition(m_obContentSize / 2);
    m_onButton->setPosition(m_obContentSize / 2);
}

void CCMenuItemTriToggler::setState(int state) {
    m_state = state;
    this->setVisibleButton();
}

int CCMenuItemTriToggler::getState() {
    return m_state;
}

void CCMenuItemTriToggler::toggle(std::optional<bool> value) {
    if (!value) m_state = -1;
    else m_state = value.value();
    this->setVisibleButton();
}

std::optional<bool> CCMenuItemTriToggler::isToggled() {
    switch (m_state) {
        default:
        case -1: return std::nullopt;
        case 0: return false;
        case 1: return true;
    }
}

void CCMenuItemTriToggler::activate() {
    auto nextState = m_state + 1;
    if (nextState > 1) nextState = -1;
    this->setState(nextState);

    CCMenuItem::activate();
}

void CCMenuItemTriToggler::selected() {
    this->getActiveButton()->selected();
}

void CCMenuItemTriToggler::unselected() {
    this->getActiveButton()->unselected();
}