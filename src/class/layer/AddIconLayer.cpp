#include <Geode/Geode.hpp>
#include "AddIconLayer.hpp"
#include "IconConfigLayer.hpp"
#include <CIConstants.hpp>
#include <CIUtilities.hpp>

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
    m_buttonMenu->updateLayout();

    if (m_selectedIcon.color1) {
        if (auto value = std::get_if<ccColor3B>(&m_selectedIcon.color1.value()))
            m_tempCustomColors[0] = *value;
    }
    if (m_selectedIcon.color2) {
        if (auto value = std::get_if<ccColor3B>(&m_selectedIcon.color2.value()))
            m_tempCustomColors[1] = *value;
    }
    if (m_selectedIcon.glowColor) {
        if (auto value = std::get_if<ccColor3B>(&m_selectedIcon.glowColor.value()))
            m_tempCustomColors[2] = *value;
    }

    auto screenMenu = CCMenu::create();
    m_mainLayer->addChildAtPosition(screenMenu, Anchor::Center);

    m_iconPageLayer = CCLayer::create();
    m_mainLayer->addChild(m_iconPageLayer);
    m_iconPageMenu = CCMenu::create();
    m_iconPageLayer->addChild(m_iconPageMenu);
    m_iconPageLayer->setLayout(CopySizeLayout::create()->add(m_iconPageMenu));

    m_colorPageLayer = CCLayer::create();
    m_colorPageLayer->setVisible(false);
    m_mainLayer->addChild(m_colorPageLayer);
    m_colorPageMenu = CCMenu::create();
    m_colorPageLayer->addChild(m_colorPageMenu);
    m_colorPageLayer->setLayout(CopySizeLayout::create()->add(m_colorPageMenu));

    static_cast<CopySizeLayout*>(m_mainLayer->getLayout())
        ->add(m_iconPageLayer)
        ->add(m_colorPageLayer);
    
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
    m_iconDisplay->setScale(1.2f);
    m_mainLayer->addChildAtPosition(m_iconDisplay, Anchor::Top, ccp(0.f, -60.f));

    // Icons Page
    auto iconListBG = CCScale9Sprite::create("square02_001.png");
    iconListBG->setContentSize(ccp(350.f, 140.f));
    iconListBG->setOpacity(95);
    m_iconPageLayer->addChildAtPosition(iconListBG, Anchor::Bottom, ccp(0.f, 130.f));

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
            menu_selector(AddIconLayer::onIconPageArrow)
        );
        nextIconPageBtn->setTag(1);

        auto prevIconPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        prevIconPageSpr->setScale(0.75f);
        auto prevIconPageBtn = CCMenuItemSpriteExtra::create(
            prevIconPageSpr,
            this,
            menu_selector(AddIconLayer::onIconPageArrow)
        );
        prevIconPageBtn->setTag(-1);

        m_iconPageMenu->addChildAtPosition(
            nextIconPageBtn,
            Anchor::Bottom,
            ccp(iconListBG->getContentWidth() / 2 + 20.f, 130.f)
        );
        m_iconPageMenu->addChildAtPosition(
            prevIconPageBtn,
            Anchor::Bottom,
            ccp(-iconListBG->getContentWidth() / 2 - 20.f, 130.f)
        );

        m_iconListPageMenu = CCMenu::create();
        iconListBG->addChildAtPosition(m_iconListPageMenu, Anchor::Bottom, ccp(0.f, -12.f));
        m_iconListPageMenu->setContentWidth(iconListBG->getContentWidth());
        m_iconListPageMenu->setLayout(RowLayout::create()->setGap(7.5f));
        for (int i = 0; i <= m_iconPageMax; i++) {
            auto spr = CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png");
            auto btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(AddIconLayer::onIconPage)
            );
            btn->setTag(i);
            btn->setLayoutOptions(AxisLayoutOptions::create()->setMaxScale(0.85f));
            m_iconListPageMenu->addChild(btn);
        }
        m_iconListPageMenu->updateLayout();
    }

    m_iconCursorSpr = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_iconCursorSpr->setScale(1.15f);

    setupIconPage(m_currentIconPage);

    // Colors Page

    m_color1Display = ColorChannelSprite::create();
    m_color1Display->setScale(0.7f);
    m_color1DisabledSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    m_color1DisabledSpr->setPosition(m_color1Display->getContentSize() / 2);
    m_color1Display->addChild(m_color1DisabledSpr);
    m_color1Label = CCLabelBMFont::create("1", "bigFont.fnt");
    // m_color1Label->setColor(cc3x(0x7f));
    // m_color1Label->setOpacity(127);
    m_color1Label->setScale(0.6f);
    m_color1Label->setPosition(m_color1Display->getContentSize() / 2);
    m_color1Display->addChild(m_color1Label);

    m_color1Btn = CCMenuItemSpriteExtra::create(
        m_color1Display,
        this,
        menu_selector(AddIconLayer::onColorType)
    );
    m_color1Btn->setTag(0);
    m_colorPageMenu->addChildAtPosition(m_color1Btn, Anchor::TopRight, ccp(-90.f, -80.f));

    m_color2Display = ColorChannelSprite::create();
    m_color2Display->setScale(0.7f);
    m_color2DisabledSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    m_color2DisabledSpr->setPosition(m_color2Display->getContentSize() / 2);
    m_color2Display->addChild(m_color2DisabledSpr);
    m_color2Label = CCLabelBMFont::create("2", "bigFont.fnt");
    // m_color2Label->setColor(cc3x(0x7f));
    // m_color2Label->setOpacity(127);
    m_color2Label->setScale(0.6f);
    m_color2Label->setPosition(m_color2Display->getContentSize() / 2);
    m_color2Display->addChild(m_color2Label);

    m_color2Btn = CCMenuItemSpriteExtra::create(
        m_color2Display,
        this,
        menu_selector(AddIconLayer::onColorType)
    );
    m_color2Btn->setTag(1);
    m_colorPageMenu->addChildAtPosition(m_color2Btn, Anchor::TopRight, ccp(-60.f, -80.f));

    m_glowColorDisplay = ColorChannelSprite::create();
    m_glowColorDisplay->setScale(0.7f);
    m_glowColorDisabledSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    m_glowColorDisabledSpr->setPosition(m_glowColorDisplay->getContentSize() / 2);
    m_glowColorDisplay->addChild(m_glowColorDisabledSpr);
    m_glowColorLabel = CCLabelBMFont::create("G", "bigFont.fnt");
    // m_glowColorLabel->setColor(cc3x(0x7f));
    // m_glowColorLabel->setOpacity(127);
    m_glowColorLabel->setScale(0.6f);
    m_glowColorLabel->setPosition(m_glowColorDisplay->getContentSize() / 2);
    m_glowColorDisplay->addChild(m_glowColorLabel);

    m_glowColorBtn = CCMenuItemSpriteExtra::create(
        m_glowColorDisplay,
        this,
        menu_selector(AddIconLayer::onColorType)
    );
    m_glowColorBtn->setTag(2);
    m_colorPageMenu->addChildAtPosition(m_glowColorBtn, Anchor::TopRight, ccp(-30.f, -80.f));

    m_overrideGlowBtn = utils::createToggleButton(
        m_colorPageMenu, m_colorPageLayer,
        Anchor::TopLeft, ccp(30.f, -80.f),
        "Override Glow",
        this, menu_selector(AddIconLayer::onToggleGlow), 0.6f
    );
    m_overrideGlowBtn->toggle(m_selectedIcon.overrideGlow);

    m_colorTypeCursorSpr = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_colorTypeCursorSpr->setScale(0.8f);
    m_colorTypeCursorSpr->setPosition(m_color1Btn->getContentSize() / 2);
    m_color1Btn->addChild(m_colorTypeCursorSpr);

    m_colorMenu = CCMenu::create();
    m_colorMenu->ignoreAnchorPointForPosition(false);
    m_colorMenu->setContentSize(ccp(450.f, 160.f));
    m_colorMenu->setLayout(
        RowLayout::create()
            ->setGap(2.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    m_colorPageLayer->addChildAtPosition(m_colorMenu, Anchor::Center, ccp(0.f, -30.f));

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
        auto lockSpr = CCSprite::createWithSpriteFrameName("GJ_lockGray_001.png");
        lockSpr->setScale(0.8f);
        lockSpr->setPosition(spr->getContentSize() / 2);
        spr->addChild(lockSpr);

        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(AddIconLayer::onSelectColor)
        );
        btn->setTag(ID);
        m_colorMenu->addChild(btn);

        if (
            !GameManager::get()->isColorUnlocked(ID, UnlockType::Col1) &&
            Mod::get()->getSettingValue<bool>("disable-locked-icons")
        ) {
            btn->setEnabled(false);
        } else {
            lockSpr->setVisible(false);
        }
    }
    m_customColorSpr = ColorChannelSprite::create();
    m_customColorSpr->setColor(m_tempCustomColors[0]);
    m_customColorSpr->setScale(0.8f);
    m_customColorBtn = CCMenuItemSpriteExtra::create(
        m_customColorSpr, this, menu_selector(AddIconLayer::onSelectCustomColor)
    );
    auto customColorLabel = CCLabelBMFont::create("C", "bigFont.fnt");
    customColorLabel->setScale(0.6f);
    customColorLabel->setPosition(m_customColorBtn->getContentSize() / 2);
    m_customColorBtn->addChild(customColorLabel);
    m_colorMenu->addChild(m_customColorBtn);

    m_colorMenu->updateLayout();

    m_color1CursorSpr = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_color1CursorSpr->setScale(0.85f);
    m_color2CursorSpr = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_color2CursorSpr->setScale(0.85f);
    m_glowColorCursorSpr = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_glowColorCursorSpr->setScale(0.85f);

    m_color2CursorSpr->setColor(cc3x(0x32));
    m_glowColorCursorSpr->setColor(cc3x(0x32));
    
    if (m_selectedIcon.color1) {
        if (std::holds_alternative<int>(m_selectedIcon.color1.value())) {
            auto colorNode = m_colorMenu->getChildByTag(
                std::get<int>(m_selectedIcon.color1.value())
            );
            m_color1CursorSpr->setPosition(colorNode->getContentSize() / 2);
            colorNode->addChild(m_color1CursorSpr);
        } else {
            m_color1CursorSpr->setPosition(m_customColorBtn->getContentSize() / 2);
            m_customColorBtn->addChild(m_color1CursorSpr);
        }
    }
    if (m_selectedIcon.color2) {
        if (std::holds_alternative<int>(m_selectedIcon.color2.value())) {
            auto colorNode = m_colorMenu->getChildByTag(
                std::get<int>(m_selectedIcon.color2.value())
            );
            m_color2CursorSpr->setPosition(colorNode->getContentSize() / 2);
            colorNode->addChild(m_color2CursorSpr);
        } else {
            m_color2CursorSpr->setPosition(m_customColorBtn->getContentSize() / 2);
            m_customColorBtn->addChild(m_color2CursorSpr);
        }
    }
    if (m_selectedIcon.glowColor) {
        if (std::holds_alternative<int>(m_selectedIcon.glowColor.value())) {
            auto colorNode = m_colorMenu->getChildByTag(
                std::get<int>(m_selectedIcon.glowColor.value())
            );
            m_glowColorCursorSpr->setPosition(colorNode->getContentSize() / 2);
            colorNode->addChild(m_glowColorCursorSpr);
        } else {
            m_glowColorCursorSpr->setPosition(m_customColorBtn->getContentSize() / 2);
            m_customColorBtn->addChild(m_glowColorCursorSpr);
        }
    }

    auto clearColorSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    clearColorSpr->setScale(0.8f);
    auto clearColorBtn = CCMenuItemSpriteExtra::create(
        clearColorSpr,
        this,
        menu_selector(AddIconLayer::onClearColor)
    );
    m_colorPageMenu->addChildAtPosition(clearColorBtn, Anchor::TopRight, ccp(-120.f, -80.f));

    updateIconColors();

    m_mainLayer->updateLayout();
    return true;
}

void AddIconLayer::setupIconPage(int page) {
    m_iconList->removeAllChildren();
    auto const iconAmount = GameManager::get()->countForType(m_iconType);
    for (
            int i = 1;
            i <= constants::ICONS_PER_PAGE;
            i++
    ) {
        auto iconID = i + page * constants::ICONS_PER_PAGE;
        if (iconID > iconAmount) break;
        auto iconSpr = GJItemIcon::createBrowserItem(
            GameManager::get()->iconTypeToUnlockType(m_iconType), iconID
        );
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
        if (
            !GameManager::get()->isIconUnlocked(iconID, m_iconType) &&
            Mod::get()->getSettingValue<bool>("disable-locked-icons")
        ) {
            iconSpr->changeToLockedState(1.25f);
            iconBtn->setEnabled(false);
        }
    }
    m_iconList->updateLayout();
    updateIconCursor();
    if (m_iconListPageMenu) {
        for (auto& btn : CCArrayExt<CCMenuItemSpriteExtra*>(m_iconListPageMenu->getChildren())) {
            if (btn->getTag() == m_currentIconPage)
                btn->setNormalImage(CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"));
            else
                btn->setNormalImage(CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png"));
        }
    }
}

void AddIconLayer::updateIconCursor() {
    m_iconCursorSpr->removeFromParent();
    if (auto iconNode = m_iconList->getChildByTag(m_selectedIcon.iconID)) {
        m_iconCursorSpr->setPosition(iconNode->getContentSize() / 2);
        iconNode->addChild(m_iconCursorSpr);
    }
}

void AddIconLayer::setIconColor(std::optional<std::variant<int, ccColor3B>> color, int colorType) {
    switch(colorType) {
        case 0: m_selectedIcon.color1 = color; break;
        case 1: m_selectedIcon.color2 = color; break;
        case 2: m_selectedIcon.glowColor = color; break;
    }
    updateIconColors();
}

void AddIconLayer::updateIconColors() {
    auto gm = GameManager::get();
    if (m_selectedIcon.color1)
        m_color1DisabledSpr->setVisible(false);
    else
        m_color1DisabledSpr->setVisible(true);
    m_color1Display->setColor(
        utils::getColorFromVariant(m_selectedIcon.color1.value_or(17))
    );
    m_iconDisplay->setColor(
        utils::getColorFromVariant(m_selectedIcon.color1.value_or(gm->getPlayerColor()))
    );

    if (m_selectedIcon.color2)
        m_color2DisabledSpr->setVisible(false);
    else
        m_color2DisabledSpr->setVisible(true);
    m_color2Display->setColor(
        utils::getColorFromVariant(m_selectedIcon.color2.value_or(12))
    );
    m_iconDisplay->setSecondColor(
        utils::getColorFromVariant(m_selectedIcon.color2.value_or(gm->getPlayerColor2()))
    );

    if (m_selectedIcon.glowColor)
        m_glowColorDisabledSpr->setVisible(false);
    else
        m_glowColorDisabledSpr->setVisible(true);
    m_glowColorDisplay->setColor(
        utils::getColorFromVariant(m_selectedIcon.glowColor.value_or(17))
    );

    if (m_selectedIcon.overrideGlow) {
        if (m_selectedIcon.glowColor) {
            m_iconDisplay->setGlowOutline(
                utils::getColorFromVariant(m_selectedIcon.glowColor.value())
            );
        } else {
            m_iconDisplay->disableGlowOutline();
        }
    } else {
        if (gm->getPlayerGlow()) {
            m_iconDisplay->setGlowOutline(
                gm->colorForIdx(gm->getPlayerGlowColor())
            );
        } else {
            m_iconDisplay->disableGlowOutline();
        }
    }
}

void AddIconLayer::onPage(CCObject* sender) {
    m_currentPage = sender->getTag();
    switch (m_currentPage) {
    case 0:
        m_iconPageLayer->setVisible(true);
        m_colorPageLayer->setVisible(false);
        break;
    case 1:
        m_iconPageLayer->setVisible(false);
        m_colorPageLayer->setVisible(true);
        break;
    }
}

void AddIconLayer::onIconPageArrow(CCObject* sender) {
    m_currentIconPage += sender->getTag();
    if (m_currentIconPage < 0) m_currentIconPage = m_iconPageMax;
    else if (m_currentIconPage > m_iconPageMax) m_currentIconPage = 0;
    setupIconPage(m_currentIconPage);
}

void AddIconLayer::onIconPage(CCObject* sender) {
    if (m_currentIconPage == sender->getTag()) return;
    m_currentIconPage = sender->getTag();
    setupIconPage(m_currentIconPage);
}

void AddIconLayer::onSelectIcon(CCObject* sender) {
    auto iconID = sender->getTag();
    m_iconDisplay->updatePlayerFrame(iconID, m_iconType);
    m_selectedIcon.iconID = iconID;

    auto iconNode = static_cast<CCNode*>(sender);
    m_iconCursorSpr->removeFromParent();
    m_iconCursorSpr->setPosition(iconNode->getContentSize() / 2);
    iconNode->addChild(m_iconCursorSpr);
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

    auto colorNode = static_cast<CCNode*>(sender);
    m_colorTypeCursorSpr->removeFromParent();
    m_colorTypeCursorSpr->setPosition(colorNode->getContentSize() / 2);
    colorNode->addChild(m_colorTypeCursorSpr);

    m_customColorSpr->setColor(m_tempCustomColors[m_selectedColorType]);

    UnlockType colorUnlockType;
    switch (m_selectedColorType) {
    case 0:
        m_color1CursorSpr->setColor(cc3x(0xff));
        m_color2CursorSpr->setColor(cc3x(0x32));
        m_glowColorCursorSpr->setColor(cc3x(0x32));
        colorUnlockType = UnlockType::Col1;
        break;
    case 1:
        m_color1CursorSpr->setColor(cc3x(0x32));
        m_color2CursorSpr->setColor(cc3x(0xff));
        m_glowColorCursorSpr->setColor(cc3x(0x32));
        colorUnlockType = UnlockType::Col2;
        break;
    case 2:
        m_color1CursorSpr->setColor(cc3x(0x32));
        m_color2CursorSpr->setColor(cc3x(0x32));
        m_glowColorCursorSpr->setColor(cc3x(0xff));
        colorUnlockType = UnlockType::Col2;
        break;
    }
    if (Mod::get()->getSettingValue<bool>("disable-locked-icons")) {
        for (auto& btn : CCArrayExt<CCMenuItemSpriteExtra*>(m_colorMenu->getChildren())) {
            if (btn == m_customColorBtn) continue;
            auto lockSpr = getChildOfType<CCSprite>(btn->getNormalImage(), 0);
            if (!GameManager::get()->isColorUnlocked(btn->getTag(), colorUnlockType)) {
                lockSpr->setVisible(true);
                btn->setEnabled(false);
            } else {
                lockSpr->setVisible(false);
                btn->setEnabled(true);
            }
        }
    }
}

void AddIconLayer::onSelectColor(CCObject* sender) {
    setIconColor(sender->getTag(), m_selectedColorType);

    CCSprite* cursorSpr = nullptr;
    switch (m_selectedColorType) {
        default:
        case 0: cursorSpr = m_color1CursorSpr; break;
        case 1: cursorSpr = m_color2CursorSpr; break;
        case 2: cursorSpr = m_glowColorCursorSpr; break;
    }
    if (!cursorSpr) return; // Just in case
    auto colorNode = static_cast<CCNode*>(sender);
    cursorSpr->removeFromParent();
    cursorSpr->setPosition(colorNode->getContentSize() / 2);
    colorNode->addChild(cursorSpr);
}

void AddIconLayer::onSelectCustomColor(CCObject* sender) {
    setIconColor(m_tempCustomColors[m_selectedColorType], m_selectedColorType);

    CCSprite* cursorSpr = nullptr;
    switch (m_selectedColorType) {
        default:
        case 0: cursorSpr = m_color1CursorSpr; break;
        case 1: cursorSpr = m_color2CursorSpr; break;
        case 2: cursorSpr = m_glowColorCursorSpr; break;
    }
    if (!cursorSpr) return;
    cursorSpr->removeFromParent();
    cursorSpr->setPosition(m_customColorBtn->getContentSize() / 2);
    m_customColorBtn->addChild(cursorSpr);
}

void AddIconLayer::onClearColor(CCObject* sender) {
    setIconColor(std::nullopt, m_selectedColorType);
    CCSprite* cursorSpr = nullptr;
    switch (m_selectedColorType) {
        default:
        case 0: cursorSpr = m_color1CursorSpr; break;
        case 1: cursorSpr = m_color2CursorSpr; break;
        case 2: cursorSpr = m_glowColorCursorSpr; break;
    }
    if (!cursorSpr) return;
    cursorSpr->removeFromParent();
}

void AddIconLayer::onToggleGlow(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    m_selectedIcon.overrideGlow = !btn->isToggled();
    updateIconColors();
}