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
    if (ret && ret->initAnchored(350.f, 300.f, iconType, ID, color1, color2)) {
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
        iconScrollLayer->m_contentLayer->getContentHeight()
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
    auto color1Label = CCLabelBMFont::create("1", "bigFont.fnt");
    color1Label->setPosition(m_color1Display->getContentSize() / 2);
    color1Label->setColor(cc3x(0x0));
    color1Label->setOpacity(127);
    color1Label->setScale(0.8f);
    m_color1Display->addChild(color1Label);

    m_color2Display = ColorChannelSprite::create();
    m_color2Display->setScale(0.7f);
    auto color2Label = CCLabelBMFont::create("2", "bigFont.fnt");
    color2Label->setPosition(m_color2Display->getContentSize() / 2);
    color2Label->setColor(cc3x(0x0));
    color2Label->setOpacity(127);
    color2Label->setScale(0.8f);
    m_color2Display->addChild(color2Label);

    m_mainLayer->addChildAtPosition(m_color1Display, Anchor::Top, ccp(75.f, -65.f));
    m_mainLayer->addChildAtPosition(m_color2Display, Anchor::Top, ccp(125.f, -65.f));

    return true;
}

size_t AddIconLayer::getIconCount() const {
    switch (m_iconType) {
        default:
        case IconType::Cube: return constants::CUBE_COUNT;
        case IconType::Ship: return constants::SHIP_COUNT;
        case IconType::Ball: return constants::BALL_COUNT;
        case IconType::Ufo: return constants::BIRD_COUNT;
        case IconType::Wave: return constants::DART_COUNT;
        case IconType::Robot: return constants::ROBOT_COUNT;
        case IconType::Spider: return constants::SPIDER_COUNT;
        case IconType::Swing: return constants::SWING_COUNT;
        case IconType::Jetpack: return constants::JETPACK_COUNT;
    }
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

void AddIconLayer::onSelectIcon(CCObject* sender) {
    auto iconID = sender->getTag();
    m_iconDisplay->updatePlayerFrame(iconID, m_iconType);
    m_selectedIcon.iconID = iconID;
}

void AddIconLayer::onAddIcon(CCObject* sender) {
    IconConfigLayer::getInstance()->addIcon(m_selectedIcon);
    AddIconLayer::onClose(nullptr);
}