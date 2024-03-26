#include "IconCell.hpp"
#include <class/layer/IconConfigLayer.hpp>
#include <CIConstants.hpp>

using namespace geode::prelude;
using namespace changing_icons;

IconCell* IconCell::create(
        IconConfigLayer* configLayer,
        int index,
        IconType iconType,
        IconProperties const& icon,
        bool isLast
    ) {
    auto ret = new IconCell();
    if (ret && ret->init(configLayer, index, iconType, icon, isLast)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool IconCell::init(
        IconConfigLayer* configLayer,
        int index,
        IconType iconType,
        IconProperties const& icon,
        bool isLast
    ) {
    if (!CCLayerColor::init()) return false;
    m_configLayer = configLayer;

    this->setContentSize(ccp(constants::ICONCELL_WIDTH, constants::ICONCELL_HEIGHT));
    this->setAnchorPoint(ccp(0.f, 0.f));
    if (index % 2 == 0)
        this->setOpacity(100);
    else
        this->setOpacity(50);
    m_index = index;

    auto playerColor1 = GameManager::get()->getPlayerColor();
    auto playerColor2 = GameManager::get()->getPlayerColor2();

    if (icon.color1) {
        auto color1Display = ColorChannelSprite::create();
        color1Display->setScale(0.5f);
        color1Display->setColor(GameManager::get()->colorForIdx(icon.color1.value()));
        this->addChildAtPosition(color1Display, Anchor::Left, ccp(55.f, 0.f));
        playerColor1 = icon.color1.value();
    }

    if (icon.color2) {
        auto color2Display = ColorChannelSprite::create();
        color2Display->setScale(0.5f);
        color2Display->setColor(GameManager::get()->colorForIdx(icon.color2.value()));
        this->addChildAtPosition(color2Display, Anchor::Left, ccp(75.f, 0.f));
        playerColor2 = icon.color2.value();
    }

    auto iconDisplay = SimplePlayer::create(0);
    iconDisplay->setScale(0.7f);
    iconDisplay->updatePlayerFrame(icon.iconID, iconType);
    iconDisplay->setColor(GameManager::get()->colorForIdx(playerColor1));
    iconDisplay->setSecondColor(GameManager::get()->colorForIdx(playerColor2));
    this->addChildAtPosition(iconDisplay, Anchor::Left, ccp(24.f, 0.f));

    auto menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    menu->setContentSize(ccp(40.f, 40.f));
    menu->setAnchorPoint(ccp(1.f, 0.5f));
    this->addChildAtPosition(menu, Anchor::Right);

    auto trashBtnSpr = CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png");
    auto trashBtn = CCMenuItemSpriteExtra::create(
        trashBtnSpr,
        this,
        menu_selector(IconCell::onDelete)
    );
    menu->addChildAtPosition(trashBtn, Anchor::Center, CCPointZero, false);

    auto editBtnSpr = CCSprite::createWithSpriteFrameName("geode.loader/pencil.png");
    editBtnSpr->setScale(0.5f);
    auto editBtn = CCMenuItemSpriteExtra::create(
        editBtnSpr,
        this,
        menu_selector(IconCell::onEdit)
    );
    menu->addChildAtPosition(editBtn, Anchor::Center, ccp(-20.f, 0.f), false);

    auto moveDownSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    moveDownSpr->setScale(0.7f);
    auto moveDownBtn = CCMenuItemSpriteExtra::create(
        moveDownSpr,
        this,
        menu_selector(IconCell::onMoveDown)
    );
    menu->addChildAtPosition(moveDownBtn, Anchor::Center, ccp(-40.f, -6.f), false);

    auto moveUpSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    moveUpSpr->setScale(0.7f);
    auto moveUpBtn = CCMenuItemSpriteExtra::create(
        moveUpSpr,
        this,
        menu_selector(IconCell::onMoveUp)
    );
    menu->addChildAtPosition(moveUpBtn, Anchor::Center, ccp(-40.f, 6.f), false);

    if (index == 0) moveUpBtn->setVisible(false);
    if (isLast) moveDownBtn->setVisible(false);

    return true;
}

void IconCell::onDelete(CCObject*) {
    m_configLayer->deleteIcon(m_index);
}

void IconCell::onEdit(CCObject*) {
    m_configLayer->editIconAtIndex(m_index);
}

void IconCell::onMoveDown(CCObject*) {
    m_configLayer->swapIcons(m_index, m_index + 1);
}

void IconCell::onMoveUp(CCObject*) {
    m_configLayer->swapIcons(m_index, m_index - 1);
}