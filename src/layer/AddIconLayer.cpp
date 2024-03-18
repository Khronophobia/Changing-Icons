#include <Geode/Geode.hpp>
#include "AddIconLayer.hpp"
#include "IconConfigLayer.hpp"
#include "../constants.hpp"

using namespace geode::prelude;
using namespace changing_icons;

AddIconLayer* AddIconLayer::create(
            IconType iconType,
            int ID,
            std::optional<cocos2d::ccColor3B> color1,
            std::optional<cocos2d::ccColor3B> color2
        ) {
    auto ret = new AddIconLayer();
    if (ret && ret->initAnchored(420.f, 300.f, iconType, ID, color1, color2)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AddIconLayer::setup(
            IconType iconType,
            int ID,
            std::optional<cocos2d::ccColor3B> color1,
            std::optional<cocos2d::ccColor3B> color2
        ) {
    m_noElasticity = true;
    m_iconType = iconType;
    m_selectedIcon = {
        .iconID = 1,
        .color1 = std::nullopt,
        .color2 = std::nullopt
    };

    m_closeBtn->removeFromParent();
    auto closeSpr = ButtonSprite::create("Cancel");
    closeSpr->setScale(0.8f);
    m_closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        menu_selector(AddIconLayer::onClose)
    );
    m_buttonMenu->addChildAtPosition(m_closeBtn, Anchor::Bottom, ccp(-40.f, 20.f));

    auto addIconSpr = ButtonSprite::create("Add");
    addIconSpr->setScale(0.8f);
    auto addIconBtn = CCMenuItemSpriteExtra::create(
        addIconSpr,
        this,
        menu_selector(AddIconLayer::onAddIcon)
    );
    m_buttonMenu->addChildAtPosition(addIconBtn, Anchor::Bottom, ccp(40.f, 20.f));

    auto iconListBg = CCLayerColor::create({0, 0, 0, 95});
    iconListBg->ignoreAnchorPointForPosition(false);
    iconListBg->setAnchorPoint(ccp(0.f, 1.f));
    iconListBg->setContentSize(ccp(180.f, 230.f));
    m_mainLayer->addChildAtPosition(iconListBg, Anchor::TopLeft, ccp(20.f, -10.f));

    auto iconScrollLayer = ScrollLayer::create(iconListBg->getContentSize());
    iconScrollLayer->setAnchorPoint(ccp(0.f, 0.5f));
    iconListBg->addChild(iconScrollLayer);

    m_iconList = CCMenu::create();
    m_iconList->setContentWidth(170.f);
    m_iconList->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(15.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
    );

    // Setup icons
    for (size_t i = 1; i <= AddIconLayer::getIconCount(); i++) {
        auto iconSpr = GJItemIcon::createBrowserItem(convertIconType(m_iconType), i);
        auto iconBtn = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(AddIconLayer::onSelectIcon)
        );
        iconBtn->setTag(i);
        m_iconList->addChild(iconBtn);
    }
    m_iconList->updateLayout();

    auto contentHeight = std::max<int>(
        m_iconList->getContentHeight() + 10.f,
        iconScrollLayer->getContentHeight()
    );
    iconScrollLayer->m_contentLayer->setContentHeight(contentHeight);
    iconScrollLayer->m_contentLayer->addChildAtPosition(
        m_iconList,
        Anchor::Top,
        ccp(0.f, -m_iconList->getContentHeight() / 2 - 5.f),
        false
    );
    iconScrollLayer->moveToTop();

    auto iconScrollbar = Scrollbar::create(iconScrollLayer);
    iconListBg->addChildAtPosition(
        iconScrollbar,
        Anchor::Right,
        ccp(5.f, 0.f),
        false
    );

    m_iconDisplay = SimplePlayer::create(1);
    m_iconDisplay->updatePlayerFrame(1, m_iconType);
    m_iconDisplay->setColor(GameManager::get()->colorForIdx(17));
    m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(12));

    m_mainLayer->addChildAtPosition(m_iconDisplay, Anchor::Top, ccp(100.f, -30.f));

    m_color1Display = ColorChannelSprite::create();
    m_color1Display->setScale(0.7f);

    m_color1Label = CCLabelBMFont::create("1U", "bigFont.fnt");
    m_color1Label->setPosition(m_color1Display->getContentSize() / 2);
    m_color1Label->setColor(cc3x(0x0));
    m_color1Label->setOpacity(127);
    m_color1Label->setScale(0.65f);
    if (color1) {
        m_color1Label->setString("1");
        m_color1Display->setColor(m_selectedIcon.color1.value());
    } else m_color1Display->setColor(GameManager::get()->colorForIdx(17));

    m_color1Display->addChild(m_color1Label);

    m_color2Display = ColorChannelSprite::create();
    m_color2Display->setScale(0.7f);

    m_color2Label = CCLabelBMFont::create("2U", "bigFont.fnt");
    m_color2Label->setPosition(m_color2Display->getContentSize() / 2);
    m_color2Label->setColor(cc3x(0x0));
    m_color2Label->setOpacity(127);
    m_color2Label->setScale(0.65f);
    if (color2) {
        m_color2Label->setString("2");
        m_color2Display->setColor(m_selectedIcon.color2.value());
    } else m_color2Display->setColor(GameManager::get()->colorForIdx(12));

    m_color2Display->addChild(m_color2Label);

    m_mainLayer->addChildAtPosition(m_color1Display, Anchor::Top, ccp(50.f, -65.f));
    m_mainLayer->addChildAtPosition(m_color2Display, Anchor::Top, ccp(155.f, -65.f));

    auto color1ListBg = CCLayerColor::create({0, 0, 0, 95});
    color1ListBg->ignoreAnchorPointForPosition(false);
    color1ListBg->setContentSize(ccp(90.f, 160.f));
    m_mainLayer->addChildAtPosition(color1ListBg, Anchor::Center, ccp(50.f, -10.f));

    auto color1ScrollLayer = ScrollLayer::create(color1ListBg->getContentSize());
    color1ListBg->addChild(color1ScrollLayer);

    auto color1Menu = CCMenu::create();
    color1Menu->ignoreAnchorPointForPosition(false);
    color1Menu->setContentWidth(80.f);
    color1Menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(4.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
    );

    auto color2ListBg = CCLayerColor::create({0, 0, 0, 95});
    color2ListBg->ignoreAnchorPointForPosition(false);
    color2ListBg->setContentSize(ccp(90.f, 160.f));
    m_mainLayer->addChildAtPosition(color2ListBg, Anchor::Center, ccp(155.f, -10.f));

    auto color2ScrollLayer = ScrollLayer::create(color2ListBg->getContentSize());
    color2ListBg->addChild(color2ScrollLayer);

    auto color2Menu = CCMenu::create();
    color2Menu->ignoreAnchorPointForPosition(false);
    color2Menu->setContentWidth(80.f);
    color2Menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(4.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
    );

    // Setup colors
    for (size_t i = 0; i <= constants::COLOR_COUNT; i++) {
        auto color1Spr = ColorChannelSprite::create();
        color1Spr->setColor(GameManager::get()->colorForIdx(i));
        color1Spr->setScale(0.7f);
        auto color1Btn = CCMenuItemSpriteExtra::create(
            color1Spr,
            this,
            menu_selector(AddIconLayer::onColor)
        );
        color1Btn->setTag(i);
        color1Menu->addChild(color1Btn);

        auto color2Spr = ColorChannelSprite::create();
        color2Spr->setColor(GameManager::get()->colorForIdx(i));
        color2Spr->setScale(0.7f);
        auto color2Btn = CCMenuItemSpriteExtra::create(
            color2Spr,
            this,
            menu_selector(AddIconLayer::onSecondColor)
        );
        color2Btn->setTag(i);
        color2Menu->addChild(color2Btn);
    }
    color1Menu->updateLayout();
    color2Menu->updateLayout();

    auto colorContentHeight = std::max<float>(
        color1Menu->getContentHeight() + 10.f,
        color1ScrollLayer->getContentHeight()
    );
    color1ScrollLayer->m_contentLayer->setContentHeight(colorContentHeight);
    color2ScrollLayer->m_contentLayer->setContentHeight(colorContentHeight);

    color1ScrollLayer->m_contentLayer->addChildAtPosition(
        color1Menu,
        Anchor::Top,
        ccp(0.f, -color1Menu->getContentHeight() / 2 - 5.f),
        false
    );
    color2ScrollLayer->m_contentLayer->addChildAtPosition(
        color2Menu,
        Anchor::Top,
        ccp(0.f, -color2Menu->getContentHeight() / 2 - 5.f),
        false
    );
    color1ScrollLayer->moveToTop();
    color2ScrollLayer->moveToTop();

    return true;
}

size_t AddIconLayer::getIconCount() const {
    return GameManager::get()->countForType(m_iconType);
}

UnlockType AddIconLayer::convertIconType(IconType type) {
    switch (type) {
        default:
        case IconType::Cube: return UnlockType::Cube;
        case IconType::Ship: return UnlockType::Ship;
        case IconType::Ball: return UnlockType::Ball;
        case IconType::Ufo: return UnlockType::Bird;
        case IconType::Wave: return UnlockType::Dart;
        case IconType::Robot: return UnlockType::Robot;
        case IconType::Spider: return UnlockType::Spider;
        case IconType::Swing: return UnlockType::Swing;
        case IconType::Jetpack: return UnlockType::Jetpack;
    }
}

void AddIconLayer::setIconColor(std::optional<cocos2d::ccColor3B> color, int colorType) {
    switch(colorType) {
        case 0: m_selectedIcon.color1 = color; break;
        case 1: m_selectedIcon.color2 = color; break;
    }
    AddIconLayer::updateIconColors();
}

void AddIconLayer::updateIconColors() {
    if (m_selectedIcon.color1) {
        m_color1Label->setString("1");
        m_color1Display->setColor(m_selectedIcon.color1.value());
        m_iconDisplay->setColor(m_selectedIcon.color1.value());
    } else {
        m_color1Label->setString("1U");
        m_color1Display->setColor(GameManager::get()->colorForIdx(17));
        m_iconDisplay->setColor(GameManager::get()->colorForIdx(17));
    }
    if (m_selectedIcon.color2) {
        m_color2Label->setString("2");
        m_color2Display->setColor(m_selectedIcon.color2.value());
        m_iconDisplay->setSecondColor(m_selectedIcon.color2.value());
    } else {
        m_color2Label->setString("2U");
        m_color2Display->setColor(GameManager::get()->colorForIdx(12));
        m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(12));
    }
 }

void AddIconLayer::onSelectIcon(CCObject* sender) {
    auto iconID = sender->getTag();
    m_iconDisplay->updatePlayerFrame(iconID, m_iconType);
    m_selectedIcon.iconID = iconID;
}

void AddIconLayer::onAddIcon(CCObject* sender) {
    IconConfigLayer::getInstance()->addIcon(m_selectedIcon);
    AddIconLayer::onClose(nullptr);
}

void AddIconLayer::onColor(CCObject* sender) {
    AddIconLayer::setIconColor(
        GameManager::get()->colorForIdx(sender->getTag()),
        0
    );
}

void AddIconLayer::onSecondColor(CCObject* sender) {
    AddIconLayer::setIconColor(
        GameManager::get()->colorForIdx(sender->getTag()),
        1
    );
}