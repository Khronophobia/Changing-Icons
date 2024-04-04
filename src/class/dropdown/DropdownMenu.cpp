#include <Geode/Geode.hpp>
#include "DropdownMenu.hpp"
#include "DropdownChoice.hpp"

using namespace geode::prelude;
using namespace changing_icons;

DropdownMenu* DropdownMenu::create(std::vector<std::string> const& choices, float width, CCObject* target, cocos2d::SEL_MenuHandler dropdownCallback, int selectedChoice) {
    auto ret = new DropdownMenu();
    if (ret && ret->init(choices, width, target, dropdownCallback, selectedChoice)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DropdownMenu::init(std::vector<std::string> const& choices, float width, CCObject* target, cocos2d::SEL_MenuHandler dropdownCallback, int selectedChoice) {
    m_dropdownTarget = target;
    m_dropdownCallback = dropdownCallback;
    m_choice = selectedChoice;
    m_choiceList = choices;

    m_stencil = CCLayerColor::create();
    m_stencil->ignoreAnchorPointForPosition(false);
    m_stencil->setAnchorPoint(ccp(0.f, 1.f));

    if (!CCClippingNode::init(m_stencil)) return false;
    this->setContentSize(ccp(width, 24.f));
    m_stencil->setContentSize(m_obContentSize);
    m_stencil->setPositionY(m_obContentSize.height);

    auto selectorMenu = CCMenu::create();
    selectorMenu->setContentSize(m_obContentSize);
    selectorMenu->setPosition(CCPointZero);
    this->addChild(selectorMenu);

    m_dropdownSelector = DropdownChoice::create(m_choiceList[m_choice].c_str(), width, this, menu_selector(DropdownMenu::onDropdown), true);
    m_dropdownSelector->setAnchorPoint(CCPointZero);
    selectorMenu->addChild(m_dropdownSelector);

    auto choiceBackground = CCLayerColor::create({0, 0, 0, 127});
    choiceBackground->ignoreAnchorPointForPosition(false);
    choiceBackground->setAnchorPoint(ccp(0.f, 1.f));
    choiceBackground->setContentSize(ccp(width, m_obContentSize.height * m_choiceList.size()));
    this->addChild(choiceBackground);

    m_choiceMenu = CCMenu::create();
    m_choiceMenu->setContentSize(m_obContentSize);
    m_choiceMenu->setPosition(ccp(0.f, -m_obContentSize.height));
    m_choiceMenu->setEnabled(false);
    m_choiceMenu->setTouchPriority(CCTouchDispatcher::get()->getForcePrio() - 1);
    this->addChild(m_choiceMenu);

    auto choiceIndex = 0;
    auto choicePosY = 0.f;
    for (auto const& str : m_choiceList) {
        auto dropdown = DropdownChoice::create(str.c_str(), width, this, menu_selector(DropdownMenu::onDropdownChoice), false);
        if (choiceIndex == m_choice) dropdown->setSelected(true);
        if (choiceIndex % 2 == 0) dropdown->m_background->setOpacity(95);
        dropdown->setAnchorPoint(ccp(0.f, 0.f));
        dropdown->setPosition(ccp(0.f, choicePosY));
        dropdown->setTag(choiceIndex);
        m_choiceMenu->addChild(dropdown);
        choicePosY -= m_obContentSize.height;
        ++choiceIndex;
    }

    return true;
}

void DropdownMenu::setChoice(int choice) {
    m_choice = choice;
    m_dropdownSelector->setLabelText(m_choiceList[choice].c_str());
    if (m_isExpanded) m_dropdownSelector->activate();

    for (auto& choice : CCArrayExt<DropdownChoice*>(m_choiceMenu->getChildren())) {
        if (choice->getTag() == m_choice) choice->setSelected(true);
        else choice->setSelected(false);
    }
}

int DropdownMenu::getChoice() const {
    return m_choice;
}

void DropdownMenu::onDropdown(CCObject* sender) {
    if (!m_isExpanded) {
        m_choiceMenu->setEnabled(true);
        m_stencil->runAction(CCEaseOut::create(CCScaleTo::create(0.25f, 1.f, m_choiceList.size() + 1), 2.f));
    } else {
        m_choiceMenu->setEnabled(false);
        m_stencil->runAction(CCEaseOut::create(CCScaleTo::create(0.25f, 1.f), 2.f));
    }
    m_isExpanded = !m_isExpanded;
}

void DropdownMenu::onDropdownChoice(CCObject* sender) {
    setChoice(sender->getTag());

    if (m_dropdownTarget && m_dropdownCallback) {
        (m_dropdownTarget->*m_dropdownCallback)(sender);
    }
}