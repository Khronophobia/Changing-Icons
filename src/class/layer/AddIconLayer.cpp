#include <Geode/Geode.hpp>
#include "AddIconLayer.hpp"
#include "IconConfigLayer.hpp"
#include <CIConstants.hpp>

using namespace geode::prelude;
using namespace changing_icons;

AddIconLayer* AddIconLayer::create(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps, std::optional<int> index) {
    auto ret = new AddIconLayer();
    if (ret && ret->initAnchored(480.f, 300.f, iconType, configLayer, iconProps, index, "GJ_square02.png")) {
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
    if (ret && ret->initAnchored(480.f, 300.f, iconType, configLayer, iconProps, std::nullopt, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AddIconLayer::setup(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps, std::optional<int> index) {
    m_noElasticity = true;
    m_iconType = iconType;
    m_selectedIcon = iconProps;
    m_configLayer = configLayer;
    m_index = index;
    m_iconPageMax = GameManager::get()->countForType(iconType) / constants::ICONS_PER_PAGE;
    if (m_index) this->setTitle("Edit Icon");
    else this->setTitle("Add Icon");

    static_cast<AnchorLayoutOptions*>(m_closeBtn->getLayoutOptions())
        ->setOffset(ccp(10.f, -10.f));
    m_mainLayer->updateLayout();

    auto screenMenu = CCMenu::create();
    m_mainLayer->addChildAtPosition(screenMenu, Anchor::Center);
    
    auto iconPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto iconPageBtn = CCMenuItemSpriteExtra::create(
        iconPageSpr,
        this,
        menu_selector(AddIconLayer::onPage)
    );
    iconPageBtn->setTag(0);
    screenMenu->addChildAtPosition(
        iconPageBtn,
        Anchor::Left,
        ccp(iconPageBtn->getContentWidth() / 2 + 4.f, 0.f)
    );

    auto colorPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    colorPageSpr->setFlipX(true);
    auto colorPageBtn = CCMenuItemSpriteExtra::create(
        colorPageSpr,
        this,
        menu_selector(AddIconLayer::onPage)
    );
    colorPageBtn->setTag(1);
    screenMenu->addChildAtPosition(
        colorPageBtn,
        Anchor::Right,
        ccp(-colorPageBtn->getContentWidth() / 2 - 4.f, 0.f)
    );

    ButtonSprite* addIconSpr;
    if (m_index) addIconSpr = ButtonSprite::create("Edit");
    else addIconSpr = ButtonSprite::create("Add");
    addIconSpr->setScale(0.8f);
    auto addIconBtn = CCMenuItemSpriteExtra::create(
        addIconSpr,
        this,
        menu_selector(AddIconLayer::onAddIcon)
    );
    m_buttonMenu->addChildAtPosition(addIconBtn, Anchor::Bottom, ccp(0.f, 20.f));

    m_iconDisplay = SimplePlayer::create(0);
    m_iconDisplay->updatePlayerFrame(m_selectedIcon.iconID, m_iconType);
    m_iconDisplay->setScale(1.1f);
    m_mainLayer->addChildAtPosition(m_iconDisplay, Anchor::Top, ccp(0.f, -50.f));

    // Icons Page
    auto iconListBG = CCScale9Sprite::create("square02_001.png");
    iconListBG->setContentSize(ccp(350.f, 140.f));
    iconListBG->setOpacity(95);
    m_mainLayer->addChildAtPosition(iconListBG, Anchor::Bottom, ccp(0.f, 120.f));

    m_iconList = CCMenu::create();
    m_iconList->ignoreAnchorPointForPosition(false);
    m_iconList->setContentSize(iconListBG->getContentSize() - ccp(20.f, 20.f));
    m_iconList->setLayout(
        RowLayout::create()
            ->setGap(18.f)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    iconListBG->addChildAtPosition(m_iconList, Anchor::Center);

    if (m_iconPageMax > 0) {
        auto nextIconPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        nextIconPageSpr->setScale(0.75f);
        nextIconPageSpr->setFlipX(true);
        auto nextIconPageBtn = CCMenuItemSpriteExtra::create(
            nextIconPageSpr,
            this,
            menu_selector(AddIconLayer::onIconPage)
        );
        nextIconPageBtn->setTag(1);

        auto prevIconPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        prevIconPageSpr->setScale(0.75f);
        auto prevIconPageBtn = CCMenuItemSpriteExtra::create(
            prevIconPageSpr,
            this,
            menu_selector(AddIconLayer::onIconPage)
        );
        prevIconPageBtn->setTag(-1);

        m_buttonMenu->addChildAtPosition(
            nextIconPageBtn,
            Anchor::Bottom,
            ccp(iconListBG->getContentWidth() / 2 + 20.f, 120.f)
        );
        m_buttonMenu->addChildAtPosition(
            prevIconPageBtn,
            Anchor::Bottom,
            ccp(-iconListBG->getContentWidth() / 2 - 20.f, 120.f)
        );

        m_iconPageNodes->addObject(prevIconPageBtn);
        m_iconPageNodes->addObject(nextIconPageBtn);
    }

    setupIcons(m_currentIconPage);

    m_iconPageNodes->addObject(colorPageBtn);
    m_iconPageNodes->addObject(iconListBG);

    // Colors Page

    m_color1Display = ColorChannelSprite::create();
    m_color1Display->setScale(0.7f);
    m_color1Label = CCLabelBMFont::create("1X", "bigFont.fnt");
    m_color1Label->setColor(cc3x(0x7f));
    m_color1Label->setOpacity(127);
    m_color1Label->setScale(0.6f);
    m_color1Label->setPosition(m_color1Display->getContentSize() / 2);
    m_color1Display->addChild(m_color1Label);

    m_color1Btn = CCMenuItemSpriteExtra::create(
        m_color1Display,
        this,
        menu_selector(AddIconLayer::onColorType)
    );
    m_color1Btn->setTag(0);
    m_buttonMenu->addChildAtPosition(m_color1Btn, Anchor::Top, ccp(-20.f, -85.f));

    m_color2Display = ColorChannelSprite::create();
    m_color2Display->setScale(0.7f);
    m_color2Label = CCLabelBMFont::create("2X", "bigFont.fnt");
    m_color2Label->setColor(cc3x(0x7f));
    m_color2Label->setOpacity(127);
    m_color2Label->setScale(0.6f);
    m_color2Label->setPosition(m_color2Display->getContentSize() / 2);
    m_color2Display->addChild(m_color2Label);

    m_color2Btn = CCMenuItemSpriteExtra::create(
        m_color2Display,
        this,
        menu_selector(AddIconLayer::onColorType)
    );
    m_color2Btn->setTag(1);
    m_buttonMenu->addChildAtPosition(m_color2Btn, Anchor::Top, ccp(20.f, -85.f));

    m_selectedColorSpr = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_selectedColorSpr->setScale(0.8f);
    m_selectedColorSpr->setPosition(m_color1Btn->getPosition());
    m_mainLayer->addChild(m_selectedColorSpr);

    auto colorMenu = CCMenu::create();
    colorMenu->ignoreAnchorPointForPosition(false);
    colorMenu->setContentSize(ccp(450.f, 150.f));
    colorMenu->setLayout(
        RowLayout::create()
            ->setGap(2.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    m_mainLayer->addChildAtPosition(colorMenu, Anchor::Center, ccp(0.f, -35.f));

    std::array<int, constants::COLOR_COUNT + 1> constexpr colorOrder{
        51, 19, 48, 9, 37, 53, 54, 55, 25, 56, 57, 58,
        62, 63, 10, 29, 26, 59, 60, 61, 30, 64, 65, 66,
        70, 42, 11, 27, 71, 14, 31, 45, 46, 67, 68, 69,
        72, 73, 0, 1, 105, 28, 32, 20, 2, 38, 79, 80,
        74, 75, 44, 3, 40, 76, 77, 78, 33, 21, 81, 82,
        83, 16, 4, 5, 22, 39, 84, 50, 34, 85, 86, 87, 106, 88, 89, 90,
        52, 41, 6, 35, 47, 23, 92, 93, 49, 95, 96, 97,
        98, 8, 36, 103, 7, 13, 24, 104, 43, 99, 100, 101,
        12, 91, 17, 102, 18, 94, 15
    };
    for (auto ID : colorOrder) {
        auto spr = ColorChannelSprite::create();
        spr->setScale(0.8f);
        spr->setColor(GameManager::get()->colorForIdx(ID));
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(AddIconLayer::onSelectColor)
        );
        btn->setTag(ID);
        colorMenu->addChild(btn);
    }
    colorMenu->updateLayout();

    auto clearColorSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    clearColorSpr->setScale(0.8f);
    auto clearColorBtn = CCMenuItemSpriteExtra::create(
        clearColorSpr,
        this,
        menu_selector(AddIconLayer::onClearColor)
    );
    m_buttonMenu->addChildAtPosition(clearColorBtn, Anchor::Right, ccp(-30.f, 60.f));

    m_colorPageNodes->addObject(iconPageBtn);
    m_colorPageNodes->addObject(m_color1Btn);
    m_colorPageNodes->addObject(m_color2Btn);
    m_colorPageNodes->addObject(m_selectedColorSpr);
    m_colorPageNodes->addObject(colorMenu);
    m_colorPageNodes->addObject(clearColorBtn);

    for (auto& node : CCArrayExt<CCNode*>(m_colorPageNodes)) node->setVisible(false);
    updateIconColors();

    return true;
}

void AddIconLayer::setupIcons(int page) {
    m_iconList->removeAllChildren();
    for (
            int i = 1;
            i <= constants::ICONS_PER_PAGE;
            i++
    ) {
        auto iconID = i + page * constants::ICONS_PER_PAGE;
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

UnlockType AddIconLayer::convertIconType(IconType type) const {
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
    updateIconColors();
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
        m_iconDisplay->setColor(
            GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor())
        );
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
        m_iconDisplay->setSecondColor(
            GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2())
        );
    }
}

void AddIconLayer::onPage(CCObject* sender) {
    m_currentPage = sender->getTag();
    switch (m_currentPage) {
    case 0:
        for (auto& node : CCArrayExt<CCNode*>(m_iconPageNodes)) node->setVisible(true);
        for (auto& node : CCArrayExt<CCNode*>(m_colorPageNodes)) node->setVisible(false);
        break;
    case 1:
        for (auto& node : CCArrayExt<CCNode*>(m_iconPageNodes)) node->setVisible(false);
        for (auto& node : CCArrayExt<CCNode*>(m_colorPageNodes)) node->setVisible(true);
        break;
    }
}

void AddIconLayer::onIconPage(CCObject* sender) {
    m_currentIconPage += sender->getTag();
    if (m_currentIconPage < 0) m_currentIconPage = m_iconPageMax;
    else if (m_currentIconPage > m_iconPageMax) m_currentIconPage = 0;
    AddIconLayer::setupIcons(m_currentIconPage);
}

void AddIconLayer::onSelectIcon(CCObject* sender) {
    auto iconID = sender->getTag();
    m_iconDisplay->updatePlayerFrame(iconID, m_iconType);
    m_selectedIcon.iconID = iconID;
}

void AddIconLayer::onAddIcon(CCObject* sender) {
    if (m_index) {
        m_configLayer->replaceIcon(m_selectedIcon, m_index.value());
    } else {
        m_configLayer->addIcon(m_selectedIcon);
    }
    AddIconLayer::onClose(nullptr);
}

void AddIconLayer::onColorType(CCObject* sender) {
    m_selectedColorType = sender->getTag();
    switch (m_selectedColorType) {
        case 0: m_selectedColorSpr->setPosition(m_color1Btn->getPosition()); break;
        case 1: m_selectedColorSpr->setPosition(m_color2Btn->getPosition()); break;
    }
}

void AddIconLayer::onSelectColor(CCObject* sender) {
    switch (m_selectedColorType) {
        case 0: m_selectedIcon.color1 = sender->getTag(); break;
        case 1: m_selectedIcon.color2 = sender->getTag(); break;
    }
    updateIconColors();
}

void AddIconLayer::onClearColor(CCObject* sender) {
    switch (m_selectedColorType) {
        case 0: m_selectedIcon.color1 = std::nullopt; break;
        case 1: m_selectedIcon.color2 = std::nullopt; break;
    }
    updateIconColors();
}