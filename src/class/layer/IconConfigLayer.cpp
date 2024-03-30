#include <Geode/Geode.hpp>
#include "IconConfigLayer.hpp"
#include "AddIconLayer.hpp"
#include "GlobalConfigLayer.hpp"
#include "SavePresetLayer.hpp"
#include "LoadPresetLayer.hpp"
#include <class/CCVariableRef.hpp>
#include <class/IconCell.hpp>
#include <class/CIConfigManager.hpp>
#include <CIConstants.hpp>
#include <CIUtilities.hpp>

using namespace geode::prelude;
using namespace changing_icons;

IconConfigLayer* IconConfigLayer::create() {
    auto ret = new IconConfigLayer();
    if (ret && ret->initAnchored(400.f, 300.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool IconConfigLayer::setup() {
    m_configManager = CIConfigManager::get();

    m_noElasticity = true;
    m_currentTab = m_configManager->getGlobalConfig().currentTab;
    this->setTitle("Changing Icons");

    static_cast<AnchorLayoutOptions*>(m_closeBtn->getLayoutOptions())
        ->setOffset(ccp(10.f, -10.f));
    m_buttonMenu->updateLayout();

    auto globalConfigSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
    auto globalConfigBtn = CCMenuItemSpriteExtra::create(
        globalConfigSpr,
        this,
        menu_selector(IconConfigLayer::onGlobalConfig)
    );
    m_buttonMenu->addChildAtPosition(
        globalConfigBtn,
        Anchor::TopRight,
        ccp(-10.f, -10.f)
    );

    m_gamemodeBar = CCMenu::create();
    m_gamemodeBar->setTouchPriority(CCTouchDispatcher::get()->getForcePrio() - 1);
    m_gamemodeBar->ignoreAnchorPointForPosition(false);
    m_gamemodeBar->setContentWidth(m_size.width - 40.f);
    m_gamemodeBar->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    m_mainLayer->addChildAtPosition(m_gamemodeBar, Anchor::Bottom, ccp(0.f, 25.f));

    auto cubeBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_iconBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_iconBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    cubeBtn->setTag(0);
    auto shipBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_shipBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_shipBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    shipBtn->setTag(1);
    auto ballBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_ballBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_ballBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    ballBtn->setTag(2);
    auto birdBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_birdBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_birdBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    birdBtn->setTag(3);
    auto dartBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_dartBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    dartBtn->setTag(4);
    auto robotBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_robotBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_robotBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    robotBtn->setTag(5);
    auto spiderBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_spiderBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_spiderBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    spiderBtn->setTag(6);
    auto swingBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_swingBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_swingBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    swingBtn->setTag(7);
    auto jetpackBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_jetpackBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_jetpackBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    jetpackBtn->setTag(8);

    m_gamemodeBar->addChild(cubeBtn);
    m_gamemodeBar->addChild(shipBtn);
    m_gamemodeBar->addChild(ballBtn);
    m_gamemodeBar->addChild(birdBtn);
    m_gamemodeBar->addChild(dartBtn);
    m_gamemodeBar->addChild(robotBtn);
    m_gamemodeBar->addChild(spiderBtn);
    m_gamemodeBar->addChild(swingBtn);
    m_gamemodeBar->addChild(jetpackBtn);
    m_gamemodeBar->updateLayout();

    m_disableBtn = utils::createToggleButton(
        m_buttonMenu, m_mainLayer,
        Anchor::TopLeft, ccp(30.f, -45.f),
        "Disable",
        this, menu_selector(IconConfigLayer::onVarToggle), 0.6f
    );

    m_useAllBtn = utils::createToggleButton(
        m_buttonMenu, m_mainLayer,
        Anchor::TopLeft, ccp(30.f, -75.f),
        "Use All Icons",
        this, menu_selector(IconConfigLayer::onVarToggle), 0.6f
    );
    auto useAllInfoBtn = utils::createToggleInfo(
        m_buttonMenu, m_useAllBtn,
        this, menu_selector(IconConfigLayer::onVarInfo),
        "Select from <cj>all icons</c> instead of the <cy>list</c>."
    );

    m_includePlayerBtn = utils::createToggleButton(
        m_buttonMenu, m_mainLayer,
        Anchor::TopLeft, ccp(30.f, -105.f),
        "Include Player Icon",
        this, menu_selector(IconConfigLayer::onVarToggle), 0.6f
    );
    auto includePlayerInfoBtn = utils::createToggleInfo(
        m_buttonMenu, m_includePlayerBtn,\
        this, menu_selector(IconConfigLayer::onVarInfo),
        "Include the player's icon on the <cy>list</c>."
    );

    auto iconListBG = CCLayerColor::create({0, 0, 0, 95});
    iconListBG->setAnchorPoint(ccp(1.f, 0.5f));
    iconListBG->ignoreAnchorPointForPosition(false);
    iconListBG->setContentSize(ccp(constants::ICONCELL_WIDTH, 216.f));
    m_mainLayer->addChildAtPosition(iconListBG, Anchor::Right, ccp(-20.f, 5.f));

    m_iconList = ScrollLayer::create(iconListBG->getContentSize());
    iconListBG->addChild(m_iconList);

    m_iconListScrollbar = Scrollbar::create(m_iconList);
    iconListBG->addChildAtPosition(
        m_iconListScrollbar, Anchor::Right, ccp(5.f, 0.f), false
    );

    auto constexpr iconOrderYPos = 25.f;
    auto iconOrderLabel = CCLabelBMFont::create("Icon Order", "goldFont.fnt");
    iconOrderLabel->setScale(0.65f);
    m_mainLayer->addChildAtPosition(iconOrderLabel, Anchor::Left, ccp(110.f, iconOrderYPos));

    m_iconOrderMenu = CCMenu::create();
    m_iconOrderMenu->ignoreAnchorPointForPosition(false);
    m_iconOrderMenu->setContentSize(ccp(20.f, 20.f));
    m_mainLayer->addChildAtPosition(m_iconOrderMenu, Anchor::Left, ccp(110.f, iconOrderYPos - 22.f));

    auto randomOrderSpr = ButtonSprite::create("Random", "bigFont.fnt", "GJ_button_04.png");
    randomOrderSpr->setScale(0.5f);
    auto randomOrderBtn = CCMenuItemSpriteExtra::create(
        randomOrderSpr,
        this,
        menu_selector(IconConfigLayer::onOrderButton)
    );
    randomOrderBtn->setTag(0);
    m_iconOrderMenu->addChildAtPosition(randomOrderBtn, Anchor::Center);

    auto forwardOrderSpr = ButtonSprite::create("Forward", "bigFont.fnt", "GJ_button_04.png");
    forwardOrderSpr->setScale(0.5f);
    auto forwardOrderBtn = CCMenuItemSpriteExtra::create(
        forwardOrderSpr,
        this,
        menu_selector(IconConfigLayer::onOrderButton)
    );
    forwardOrderBtn->setTag(1);
    m_iconOrderMenu->addChildAtPosition(forwardOrderBtn, Anchor::Center, ccp(-45.f, -20.f));

    auto backwardOrderSpr = ButtonSprite::create("Backward", "bigFont.fnt", "GJ_button_04.png");
    backwardOrderSpr->setScale(0.5f);
    auto backwardOrderBtn = CCMenuItemSpriteExtra::create(
        backwardOrderSpr,
        this,
        menu_selector(IconConfigLayer::onOrderButton)
    );
    backwardOrderBtn->setTag(2);
    m_iconOrderMenu->addChildAtPosition(backwardOrderBtn, Anchor::Center, ccp(45.f, -20.f));

    /* auto const mirrorEndOffset = ccp(30.f, -30.f);
    m_mirrorEndBtn = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(IconConfigLayer::onVarToggle),
        0.6f
    );
    m_buttonMenu->addChildAtPosition(m_mirrorEndBtn, Anchor::Left, mirrorEndOffset);
    
    auto mirrorEndText = CCLabelBMFont::create("Mirror After End", "bigFont.fnt");
    mirrorEndText->setAnchorPoint(ccp(0.f, 0.5f));
    mirrorEndText->setScale(0.5f);
    m_mainLayer->addChildAtPosition(
        mirrorEndText, Anchor::Left, mirrorEndOffset + toggleTextOffset
    );

    auto mirrorEndInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    mirrorEndInfoSpr->setScale(0.4f);
    auto mirrorEndInfoBtn = CCMenuItemSpriteExtra::create(
        mirrorEndInfoSpr,
        this,
        menu_selector(IconConfigLayer::onVarInfo)
    );
    mirrorEndInfoBtn->setUserObject(CCString::create(
        "Only applicable when <cy>Icon Order</c> is set to <cj>Up</c> or <cj>Down</c>. Mirror the list when reaching the end instead of wrapping around."
    ));
    m_buttonMenu->addChildAtPosition(
        mirrorEndInfoBtn, Anchor::Left, mirrorEndOffset + toggleInfoOffset
    ); */

    m_mirrorEndBtn = utils::createToggleButton(
        m_buttonMenu, m_mainLayer,
        Anchor::Left, ccp(30.f, -50.f),
        "Mirror After End",
        this, menu_selector(IconConfigLayer::onVarToggle), 0.6f
    );
    auto mirrorEndInfoBtn = utils::createToggleInfo(
        m_buttonMenu, m_mirrorEndBtn,
        this, menu_selector(IconConfigLayer::onVarInfo),
        "Only applicable when <cy>Icon Order</c> is set to <cj>Up</c> or <cj>Down</c>. "
        "Mirror the list when reaching the end instead of wrapping around."
    );

    auto iconListMenu = CCMenu::create();
    iconListMenu->ignoreAnchorPointForPosition(false);
    iconListMenu->setContentSize(ccp(180.f, 50.f));
    iconListMenu->setLayout(
        RowLayout::create()
    );
    m_mainLayer->addChildAtPosition(iconListMenu, Anchor::Left, ccp(110.f, -85.f));

    auto iconListAddSpr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    iconListAddSpr->setScale(0.83f);
    auto iconListAddBtn = CCMenuItemSpriteExtra::create(
        iconListAddSpr,
        this,
        menu_selector(IconConfigLayer::onAddIcon)
    );
    auto iconListClearSpr = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
    iconListClearSpr->setScale(0.83f);
    auto iconListClearBtn = CCMenuItemSpriteExtra::create(
        iconListClearSpr,
        this,
        menu_selector(IconConfigLayer::onClearList)
    );
    auto iconListSaveBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png"),
        this,
        menu_selector(IconConfigLayer::onSaveList)
    );
    auto iconListLoadSpr = CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png");
    iconListLoadSpr->setScale(0.83f);
    auto iconListLoadBtn = CCMenuItemSpriteExtra::create(
        iconListLoadSpr,
        this,
        menu_selector(IconConfigLayer::onLoadList)
    );

    iconListMenu->addChild(iconListAddBtn);
    iconListMenu->addChild(iconListClearBtn);
    iconListMenu->addChild(iconListSaveBtn);
    iconListMenu->addChild(iconListLoadBtn);
    iconListMenu->updateLayout();

    refreshTab();
    return true;
}

CITabProperties& IconConfigLayer::getCurrentConfig() {
    return m_configManager->getConfig(m_currentTab);
}

void IconConfigLayer::onGlobalConfig(CCObject* sender) {
    GlobalConfigLayer::create()->show();
}

void IconConfigLayer::onSwitchTab(CCObject* sender) {
    m_currentTab = static_cast<IconType>(sender->getTag());
    m_configManager->getGlobalConfig().currentTab = m_currentTab;
    refreshTab();
}

void IconConfigLayer::refreshTab() {
    for (auto btn : CCArrayExt<CCMenuItemToggler*>(m_gamemodeBar->getChildren())) {
        if (btn->getTag() == static_cast<int>(m_currentTab)) {
            btn->setEnabled(false);
            btn->toggle(true);
            continue;
        }
        btn->setEnabled(true);
        btn->toggle(false);
    }

    auto& currentConfig = getCurrentConfig();
    m_useAllBtn->toggle(currentConfig.useAll);
    m_useAllBtn->setUserObject(CCVariableRef<bool>::create(currentConfig.useAll));
    m_includePlayerBtn->toggle(currentConfig.includePlayerIcon);
    m_includePlayerBtn->setUserObject(CCVariableRef<bool>::create(currentConfig.includePlayerIcon));
    m_disableBtn->toggle(currentConfig.disabled);
    m_disableBtn->setUserObject(CCVariableRef<bool>::create(currentConfig.disabled));
    m_mirrorEndBtn->toggle(currentConfig.mirrorEnd);
    m_mirrorEndBtn->setUserObject(CCVariableRef<bool>::create(currentConfig.mirrorEnd));

    setOrderChoice(currentConfig.order);
    refreshIconList(m_currentTab, true);
}

void IconConfigLayer::onVarInfo(CCObject* sender) {
    auto obj = static_cast<CCNode*>(sender)->getUserObject();
    auto infoStr = static_cast<CCString*>(obj)->getCString();
    FLAlertLayer::create(
        "Info",
        infoStr,
        "OK"
    )->show();
}

/* void IconConfigLayer::onRandomToggle(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    IconConfigLayer::getCurrentConfig(m_activeTab).random = !btn->m_toggled;
    log::debug("Random set to: {}", !btn->m_toggled);
} */

void IconConfigLayer::onVarToggle(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    auto obj = static_cast<CCNode*>(sender)->getUserObject();
    if (auto ref = typeinfo_cast<CCVariableRef<bool>*>(obj)) {
        ref->getVarRef() = !btn->m_toggled;
    }
}

void IconConfigLayer::onOrderButton(CCObject* sender) {
    setOrderChoice(sender->getTag());
    getCurrentConfig().order = static_cast<IconOrder>(sender->getTag());
}

void IconConfigLayer::setOrderChoice(IconOrder choice) {
    setOrderChoice(static_cast<int>(choice));
}

void IconConfigLayer::setOrderChoice(int choice) {
    for (auto btn : CCArrayExt<CCMenuItemSpriteExtra*>(m_iconOrderMenu->getChildren())) {
        auto spr = static_cast<ButtonSprite*>(btn->getNormalImage());
        if (btn->getTag() == choice)
            spr->updateBGImage("GJ_button_01.png");
        else
            spr->updateBGImage("GJ_button_04.png");
        // I need to do this for some reason??
        btn->setContentSize(spr->getScaledContentSize());
        spr->setPosition(btn->getContentSize() / 2);
    }
}

void IconConfigLayer::onAddIcon(CCObject*) {
    AddIconLayer::create(m_currentTab, this)->show();
}

void IconConfigLayer::editIconAtIndex(int index) {
    auto const& prevIcon = getCurrentConfig().iconSet.at(index);
    AddIconLayer::create(m_currentTab, this, prevIcon, index)->show();
}

void IconConfigLayer::onClearList(CCObject*) {
    createQuickPopup(
        "Clear List",
        "Are you sure you want to <cr>clear</c> the current list?",
        "Cancel", "Yes",
        [this](auto, bool btn2) {
            if (btn2) {
                getCurrentConfig().iconSet.clear();
                refreshIconList(m_currentTab, true);
            }
        }
    );
}

void IconConfigLayer::onSaveList(CCObject* sender) {
    auto const& currentConfig = getCurrentConfig();
    if (currentConfig.iconSet.empty()) {
        Notification::create("List is empty", NotificationIcon::Error)->show();
        return;
    }
    SavePresetLayer::create(m_currentTab, currentConfig.iconSet)->show();
}

void IconConfigLayer::onLoadList(CCObject* sender) {
    LoadPresetLayer::create(this, m_currentTab)->show();
}

void IconConfigLayer::refreshIconList(IconType currentTab, bool toTop) {
    auto& currentConfig = getCurrentConfig();
    auto iconList = currentConfig.iconSet;
    auto content = m_iconList->m_contentLayer;

    auto height = std::max(m_iconList->getContentHeight(), iconList.size() * constants::ICONCELL_HEIGHT);
    content->setContentHeight(height);

    m_iconList->m_contentLayer->removeAllChildren();
    size_t i = 0;
    for (auto const& icon : iconList) {
        bool isLast = false;
        if (i == iconList.size() - 1) isLast = true;
        auto cell = IconCell::create(
            this,
            i,
            currentTab,
            icon,
            isLast
        );
        cell->setPositionY(height - (i + 1) * constants::ICONCELL_HEIGHT);
        // cell->setTag(i);
        content->addChild(cell);
        i++;
    }
    if (toTop) {
        m_iconList->moveToTop();
        return;
    }
    auto maxScrollPos = m_iconList->getContentHeight() - content->getContentHeight();
    content->setPositionY(std::clamp<float>(
        content->getPositionY() + constants::ICONCELL_HEIGHT,
        maxScrollPos,
        0.f
    ));
}

void IconConfigLayer::addIcon(IconProperties icon) {
    log::info("Added icon ID {}", icon.iconID);
    getCurrentConfig().iconSet.push_back(icon);
    refreshIconList(m_currentTab);
}

void IconConfigLayer::swapIcons(int icon1, int icon2) {
    log::info("Swapped icons in index {} and {}", icon1, icon2);
    auto& iconSet = getCurrentConfig().iconSet;
    std::iter_swap(iconSet.begin() + icon1, iconSet.begin() + icon2);
    refreshIconList(m_currentTab);
}

void IconConfigLayer::replaceIcon(IconProperties icon, int index) {
    log::info("Replaced icon in index {}", index);
    auto& iconSet = getCurrentConfig().iconSet;
    iconSet.at(index) = icon;
    refreshIconList(m_currentTab);
}

void IconConfigLayer::deleteIcon(int index) {
    log::info("Removed icon at index {}", index);
    auto& iconSet = getCurrentConfig().iconSet;
    iconSet.erase(iconSet.begin() + index);
    refreshIconList(m_currentTab);
}

void IconConfigLayer::replaceList(std::vector<IconProperties> const& list) {
    getCurrentConfig().iconSet = list;
    refreshIconList(m_currentTab, true);
}

IconConfigLayer::~IconConfigLayer() {
    log::info("Saving config.");
    Mod::get()->setSavedValue("global", m_configManager->getGlobalConfig());
    Mod::get()->setSavedValue("cube", m_configManager->getConfig(IconType::Cube));
    Mod::get()->setSavedValue("ship", m_configManager->getConfig(IconType::Ship));
    Mod::get()->setSavedValue("ball", m_configManager->getConfig(IconType::Ball));
    Mod::get()->setSavedValue("bird", m_configManager->getConfig(IconType::Ufo));
    Mod::get()->setSavedValue("dart", m_configManager->getConfig(IconType::Wave));
    Mod::get()->setSavedValue("robot", m_configManager->getConfig(IconType::Robot));
    Mod::get()->setSavedValue("spider", m_configManager->getConfig(IconType::Spider));
    Mod::get()->setSavedValue("swing", m_configManager->getConfig(IconType::Swing));
    Mod::get()->setSavedValue("jetpack", m_configManager->getConfig(IconType::Jetpack));
}