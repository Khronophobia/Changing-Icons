#include "IconCell.hpp"

using namespace geode::prelude;
using namespace changing_icons;

IconCell* IconCell::create(
    int index,
    IconType iconType,
    int iconID,
    std::optional<cocos2d::ccColor3B> color1,
    std::optional<cocos2d::ccColor3B> color2
) {
    auto ret = new IconCell();
    if (ret && ret->init(index, iconType, iconID, color1, color2)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool IconCell::init(
    int index,
    IconType iconType,
    int iconID,
    std::optional<cocos2d::ccColor3B> color1,
    std::optional<cocos2d::ccColor3B> color2
) {
    if (!CCLayerColor::init()) return false;
    this->setContentSize(ccp(170.f, 40.f));
    this->setAnchorPoint(ccp(0.f, 0.f));
    if (index % 2 == 0)
        this->setOpacity(100);
    else
        this->setOpacity(50);

    if (color1) {
        auto color1Display = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        color1Display->setScale(0.8f);
        color1Display->setColor(color1.value());
        this->addChildAtPosition(color1Display, Anchor::Left, ccp(60.f, 0.f));
    } else color1 = GameManager::get()->colorForIdx(17);

    if (color2) {
        auto color2Display = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        color2Display->setScale(0.8f);
        color2Display->setColor(color2.value());
        this->addChildAtPosition(color2Display, Anchor::Left, ccp(90.f, 0.f));
    } else color2 = GameManager::get()->colorForIdx(12);

    auto icon = SimplePlayer::create(0);
    icon->setColor(color1.value());
    icon->setSecondColor(color2.value());
    icon->updatePlayerFrame(iconID, iconType);
    this->addChildAtPosition(icon, Anchor::Left, ccp(24.f, 0.f));

    auto menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    menu->setContentSize(ccp(40.f, 40.f));
    menu->setAnchorPoint(ccp(1.f, 0.5f));
    this->addChildAtPosition(menu, Anchor::Right);

    auto trashBtnSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    trashBtnSpr->setScale(0.7f);
    auto trashBtn = CCMenuItemSpriteExtra::create(
        trashBtnSpr,
        this,
        nullptr
    );
    trashBtn->setPosition(menu->getContentSize() / 2);
    menu->addChild(trashBtn);

    return true;
}