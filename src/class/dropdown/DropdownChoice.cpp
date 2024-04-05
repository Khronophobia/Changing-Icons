#include <Geode/Geode.hpp>
#include "DropdownChoice.hpp"

using namespace geode::prelude;
using namespace changing_icons;

DropdownChoice* DropdownChoice::create(char const* str, float width, CCObject* target, SEL_MenuHandler callback, bool addArrow, bool selected) {
    auto ret = new DropdownChoice();
    if (ret && ret->init(str, width, target, callback, addArrow, selected)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DropdownChoice::init(char const* str, float width, CCObject* target, SEL_MenuHandler callback, bool addArrow, bool selected) {
    if (!CCMenuItem::initWithTarget(target, callback)) return false;
    this->setContentSize(ccp(width, 24.f));
    m_selected = selected;
    m_width = width;

    m_background = CCLayerColor::create({0, 0, 0, 127});
    m_background->setContentSize(m_obContentSize);
    this->addChild(m_background);

    m_label = CCLabelBMFont::create(str, "bigFont.fnt");
    m_label->setAnchorPoint(ccp(0.f, 0.5f));
    m_label->setPosition(ccp(5.f, m_obContentSize.height / 2));
    if (selected) m_label->setColor(cc3x(0x00ff00));
    if (addArrow)
        m_label->limitLabelWidth(width - 25.f, 0.6f, 0.1f);
    else
        m_label->limitLabelWidth(width - 10.f, 0.6f, 0.1f);
    this->addChild(m_label);

    if (addArrow) {
        m_arrowSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
        m_arrowSpr->setScale(0.75f);
        m_arrowSpr->setPosition(ccp(
            m_obContentSize.width - m_arrowSpr->getScaledContentSize().width / 2 - 5.f,
            m_obContentSize.height / 2
        ));
        this->addChild(m_arrowSpr);
    }

    return true;
}

void DropdownChoice::setSelected(bool selected) {
    if (selected) m_label->setColor(cc3x(0x00ff00));
    else m_label->setColor(cc3x(0xf));
    m_selected = selected;
}

void DropdownChoice::setLabelText(char const* str) {
    m_label->setString(str);
    if (m_arrowSpr)
        m_label->limitLabelWidth(m_width - 25.f, 0.6f, 0.1f);
    else
        m_label->limitLabelWidth(m_width - 10.f, 0.6f, 0.1f);
}

void DropdownChoice::selected() {
    if (m_selected) m_label->setColor(cc3x(0x00bf00));
    else m_label->setColor(cc3x(0xbf));
}

void DropdownChoice::unselected() {
    if (m_selected) m_label->setColor(cc3x(0x00ff00));
    else m_label->setColor(cc3x(0xf));
}

void DropdownChoice::activate() {
    CCMenuItem::activate();
    if (!m_arrowSpr) return;

    auto constexpr actionDuration = 0.25f;

    if (m_isSelecting) {
        m_arrowSpr->runAction(
            CCEaseOut::create(CCRotateTo::create(actionDuration, 0.f), 2.f)
        );
    } else {
        m_arrowSpr->runAction(
            CCEaseOut::create(CCRotateTo::create(actionDuration, -90.f), 2.f)
        );
    }
    m_isSelecting = !m_isSelecting;
}