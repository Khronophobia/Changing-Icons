#include <Geode/Geode.hpp>
#include "AddIconLayer.hpp"
#include "IconConfigLayer.hpp"
#include "SelectColorLayer.hpp"

using namespace geode::prelude;
using namespace changing_icons;

AddIconLayer* AddIconLayer::create(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps) {
    auto ret = new AddIconLayer();
    if (ret && ret->initAnchored(400.f, 250.f, iconType, configLayer, iconProps, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

AddIconLayer* AddIconLayer::create(
            IconType iconType,
            IconConfigLayer* configLayer,
            int ID,
            std::optional<int> color1,
            std::optional<int> color2
        ) {
    auto iconProps = IconProperties{
        .iconID = ID,
        .color1 = color1,
        .color2 = color2
    };
    auto ret = new AddIconLayer();
    if (ret && ret->initAnchored(400.f, 250.f, iconType, configLayer, iconProps, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AddIconLayer::setup(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps) {
    m_noElasticity = true;
    m_iconType = iconType;
    m_selectedIcon = iconProps;
    m_configLayer = configLayer;
    m_pageMax = GameManager::get()->countForType(iconType) / m_iconPerPage;

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

    m_iconDisplay = SimplePlayer::create(0);
    m_iconDisplay->updatePlayerFrame(m_selectedIcon.iconID, m_iconType);
    m_mainLayer->addChildAtPosition(m_iconDisplay, Anchor::Top, ccp(0.f, -30.f));

    m_color1Display = ColorChannelSprite::create();
    m_color1Display->setScale(0.7f);
    m_color1Label = CCLabelBMFont::create("1X", "bigFont.fnt");
    m_color1Label->setColor(cc3x(0x0));
    m_color1Label->setOpacity(127);
    m_color1Label->setScale(0.6f);
    m_color1Label->setPosition(m_color1Display->getContentSize() / 2);
    m_color1Display->addChild(m_color1Label);
    if (m_selectedIcon.color1) {
        m_color1Label->setString("1");
        m_color1Display->setColor(GameManager::get()->colorForIdx(m_selectedIcon.color1.value()));
        m_iconDisplay->setColor(GameManager::get()->colorForIdx(m_selectedIcon.color1.value()));
    } else {
        m_color1Display->setColor(GameManager::get()->colorForIdx(17));
        m_iconDisplay->setColor(GameManager::get()->colorForIdx(17));
    }

    auto color1Btn = CCMenuItemSpriteExtra::create(
        m_color1Display,
        this,
        menu_selector(AddIconLayer::onColor)
    );
    color1Btn->setTag(0);
    m_buttonMenu->addChildAtPosition(color1Btn, Anchor::Top, ccp(-20.f, -60.f));

    m_color2Display = ColorChannelSprite::create();
    m_color2Display->setScale(0.7f);
    m_color2Label = CCLabelBMFont::create("2X", "bigFont.fnt");
    m_color2Label->setColor(cc3x(0x0));
    m_color2Label->setOpacity(127);
    m_color2Label->setScale(0.6f);
    m_color2Label->setPosition(m_color2Display->getContentSize() / 2);
    m_color2Display->addChild(m_color2Label);
    if (m_selectedIcon.color2) {
        m_color2Label->setString("2");
        m_color2Display->setColor(GameManager::get()->colorForIdx(m_selectedIcon.color2.value()));
        m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(m_selectedIcon.color2.value()));
    } else {
        m_color2Display->setColor(GameManager::get()->colorForIdx(12));
        m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(12));
    }

    auto color2Btn = CCMenuItemSpriteExtra::create(
        m_color2Display,
        this,
        menu_selector(AddIconLayer::onColor)
    );
    color2Btn->setTag(1);
    m_buttonMenu->addChildAtPosition(color2Btn, Anchor::Top, ccp(20.f, -60.f));

    auto iconListBG = CCScale9Sprite::create("square02_001.png");
    iconListBG->setContentSize(ccp(330.f, 130.f));
    iconListBG->setOpacity(95);
    m_mainLayer->addChildAtPosition(iconListBG, Anchor::Center, ccp(0.f, -20.f));

    m_iconList = CCMenu::create();
    m_iconList->ignoreAnchorPointForPosition(false);
    m_iconList->setContentSize(iconListBG->getContentSize() - ccp(20.f, 20.f));
    m_iconList->setLayout(
        RowLayout::create()
            ->setGap(20.f)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    iconListBG->addChildAtPosition(m_iconList, Anchor::Center, CCPointZero, false);

    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextPageSpr->setScale(0.75f);
    nextPageSpr->setFlipX(true);
    auto nextPageBtn = CCMenuItemSpriteExtra::create(
        nextPageSpr,
        this,
        menu_selector(AddIconLayer::onIconPage)
    );
    nextPageBtn->setTag(1);

    auto prevPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    prevPageSpr->setScale(0.75f);
    auto prevPageBtn = CCMenuItemSpriteExtra::create(
        prevPageSpr,
        this,
        menu_selector(AddIconLayer::onIconPage)
    );
    prevPageBtn->setTag(-1);

    m_buttonMenu->addChildAtPosition(nextPageBtn, Anchor::Right, ccp(-20.f, -20.f));
    m_buttonMenu->addChildAtPosition(prevPageBtn, Anchor::Left, ccp(20.f, -20.f));

    AddIconLayer::setupIcons(m_currentPage);

    return true;
}

void AddIconLayer::setupIcons(int page) {
    m_iconList->removeAllChildren();
    for (
            int i = 1;
            i <= m_iconPerPage;
            i++
    ) {
        auto iconID = i + page * m_iconPerPage;
        if (iconID > GameManager::get()->countForType(m_iconType)) break;
        auto iconSpr = GJItemIcon::createBrowserItem(convertIconType(m_iconType), iconID);
        auto iconBtn = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(AddIconLayer::onSelectIcon)
        );
        iconBtn->setLayoutOptions(
            AxisLayoutOptions::create()->setMaxScale(0.65f)
        );
        iconBtn->setTag(iconID);
        m_iconList->addChild(iconBtn);
    }
    m_iconList->updateLayout();
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

void AddIconLayer::setIconColor(std::optional<int> color, int colorType) {
    switch(colorType) {
        case 0: m_selectedIcon.color1 = color; break;
        case 1: m_selectedIcon.color2 = color; break;
    }
    AddIconLayer::updateIconColors();
}

void AddIconLayer::updateIconColors() {
    if (m_selectedIcon.color1) {
        m_color1Label->setString("1");
        m_color1Display->setColor(
            GameManager::get()->colorForIdx(m_selectedIcon.color1.value())
        );
        m_iconDisplay->setColor(
            GameManager::get()->colorForIdx(m_selectedIcon.color1.value())
        );
    } else {
        m_color1Label->setString("1X");
        m_color1Display->setColor(GameManager::get()->colorForIdx(17));
        m_iconDisplay->setColor(GameManager::get()->colorForIdx(17));
    }
    if (m_selectedIcon.color2) {
        m_color2Label->setString("2");
        m_color2Display->setColor(
            GameManager::get()->colorForIdx(m_selectedIcon.color2.value())
        );
        m_iconDisplay->setSecondColor(
            GameManager::get()->colorForIdx(m_selectedIcon.color2.value())
        );
    } else {
        m_color2Label->setString("2X");
        m_color2Display->setColor(GameManager::get()->colorForIdx(12));
        m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(12));
    }
}

void AddIconLayer::onIconPage(CCObject* sender) {
    m_currentPage += sender->getTag();
    if (m_currentPage < 0) m_currentPage = m_pageMax;
    else if (m_currentPage > m_pageMax) m_currentPage = 0;
    AddIconLayer::setupIcons(m_currentPage);
}

void AddIconLayer::onSelectIcon(CCObject* sender) {
    auto iconID = sender->getTag();
    m_iconDisplay->updatePlayerFrame(iconID, m_iconType);
    m_selectedIcon.iconID = iconID;
}

void AddIconLayer::onAddIcon(CCObject* sender) {
    m_configLayer->addIcon(m_selectedIcon);
    AddIconLayer::onClose(nullptr);
}

void AddIconLayer::onColor(CCObject* sender) {
    SelectColorLayer::create(
        this,
        sender->getTag(),
        m_selectedIcon,
        m_iconType
    )->show();
}