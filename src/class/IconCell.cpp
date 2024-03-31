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
    if (ret && ret->init(configLayer, index, iconType, icon, false, isLast)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

IconCell* IconCell::create(int index, IconType type, IconProperties const& icon) {
    auto ret = new IconCell();
    if (ret && ret->init(nullptr, index, type, icon, true, false)) {
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
        bool readOnly,
        bool isLast
    ) {
    if (!CCLayerColor::init()) return false;
    m_configLayer = configLayer;
    auto gm = GameManager::get();

    this->setContentSize(ccp(constants::ICONCELL_WIDTH, constants::ICONCELL_HEIGHT));
    this->setAnchorPoint(ccp(0.f, 0.f));
    if (index % 2 == 0)
        this->setOpacity(100);
    else
        this->setOpacity(50);
    m_index = index;

    auto playerColor1 = gm->getPlayerColor();
    auto playerColor2 = gm->getPlayerColor2();

    if (icon.color1) {
        auto color1Display = ColorChannelSprite::create();
        color1Display->setScale(0.5f);
        color1Display->setColor(gm->colorForIdx(icon.color1.value()));
        this->addChildAtPosition(color1Display, Anchor::Left, ccp(50.f, 0.f));
        playerColor1 = icon.color1.value();

        auto colorLabel = CCLabelBMFont::create("1", "bigFont.fnt");
        // colorLabel->setColor(cc3x(0x7f));
        // colorLabel->setOpacity(127);
        colorLabel->setScale(0.7f);
        colorLabel->setPosition(color1Display->getContentSize() / 2);
        color1Display->addChild(colorLabel);
    }

    if (icon.color2) {
        auto color2Display = ColorChannelSprite::create();
        color2Display->setScale(0.5f);
        color2Display->setColor(gm->colorForIdx(icon.color2.value()));
        this->addChildAtPosition(color2Display, Anchor::Left, ccp(70.f, 0.f));
        playerColor2 = icon.color2.value();

        auto colorLabel = CCLabelBMFont::create("2", "bigFont.fnt");
        // colorLabel->setColor(cc3x(0x7f));
        // colorLabel->setOpacity(127);
        colorLabel->setScale(0.7f);
        colorLabel->setPosition(color2Display->getContentSize() / 2);
        color2Display->addChild(colorLabel);
    }

    auto iconDisplay = SimplePlayer::create(0);
    iconDisplay->setScale(0.7f);
    iconDisplay->updatePlayerFrame(icon.iconID, iconType);
    iconDisplay->setColor(gm->colorForIdx(playerColor1));
    iconDisplay->setSecondColor(gm->colorForIdx(playerColor2));
    if (icon.overrideGlow) {
        auto glowDisplay = ColorChannelSprite::create();
        glowDisplay->setScale(0.5f);
        glowDisplay->setColor(gm->colorForIdx(icon.glowColor.value_or(17)));
        this->addChildAtPosition(glowDisplay, Anchor::Left, ccp(90.f, 0.f));

        if (icon.glowColor) {
            iconDisplay->setGlowOutline(gm->colorForIdx(icon.glowColor.value()));
        } else {
            auto glowDisabledSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
            glowDisabledSpr->setPosition(glowDisplay->getContentSize() / 2);
            glowDisplay->addChild(glowDisabledSpr);
            iconDisplay->disableGlowOutline();
        }

        auto glowLabel = CCLabelBMFont::create("G", "bigFont.fnt");
        glowLabel->setScale(0.7f);
        glowLabel->setPosition(glowDisplay->getContentSize() / 2);
        glowDisplay->addChild(glowLabel);
    } else {
        if (gm->getPlayerGlow()) {
            iconDisplay->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
        }
    }
    this->addChildAtPosition(iconDisplay, Anchor::Left, ccp(20.f, 0.f));

    if (readOnly) return true;

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