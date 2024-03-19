#include <Geode/Geode.hpp>
#include "IconConfigLayer.hpp"
#include "AddIconLayer.hpp"
#include "../class/CIVariableRef.hpp"
#include "../class/IconCell.hpp"

using namespace geode::prelude;
using namespace changing_icons;

IconConfigLayer* IconConfigLayer::getInstance() {
    return m_instance;
}

IconConfigLayer* IconConfigLayer::create() {
    auto ret = new IconConfigLayer();
    if (ret && ret->initAnchored(400.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool IconConfigLayer::setup() {
    m_instance = this;

    m_configManager = CIConfigManager::get();

    m_noElasticity = true;
    m_currentTab = m_configManager->getGlobalConfig().currentTab;
    this->setTitle("Changing Icons Config");

    m_gamemodeBar = CCMenu::create();
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

    auto const toggleTextOffset = ccp(15.f, 0.f);
    auto const toggleInfoOffset = ccp(-12.f, 12.f);

    auto const randomBtnOffset = ccp(30.f, -75.f);
    m_randomBtn = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(IconConfigLayer::onVarToggle),
        0.6f
    );
    m_buttonMenu->addChildAtPosition(m_randomBtn, Anchor::TopLeft, randomBtnOffset);

    auto randomText = CCLabelBMFont::create("Use all icons", "bigFont.fnt");
    randomText->setAnchorPoint(ccp(0.f, 0.5f));
    randomText->setScale(0.5f);
    m_mainLayer->addChildAtPosition(
        randomText, Anchor::TopLeft, randomBtnOffset + toggleTextOffset
    );

    auto randomInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    randomInfoSpr->setScale(0.4f);
    auto randomInfoBtn = CCMenuItemSpriteExtra::create(
        randomInfoSpr,
        this,
        menu_selector(IconConfigLayer::onVarInfo)
    );
    randomInfoBtn->setUserObject(CCString::create("Select from <cj>all icons</c> instead of the <cj>list</c>."));
    m_buttonMenu->addChildAtPosition(
        randomInfoBtn, Anchor::TopLeft, randomBtnOffset + toggleInfoOffset
    );

    auto const disableBtnOffset = ccp(30.f, -45.f);
    m_disableBtn = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(IconConfigLayer::onVarToggle),
        0.6f
    );
    m_buttonMenu->addChildAtPosition(m_disableBtn, Anchor::TopLeft, disableBtnOffset);

    auto disableText = CCLabelBMFont::create("Disable", "bigFont.fnt");
    disableText->setAnchorPoint(ccp(0.f, 0.5f));
    disableText->setScale(0.5f);
    m_mainLayer->addChildAtPosition(
        disableText, Anchor::TopLeft, disableBtnOffset + toggleTextOffset
    );

    auto iconListBG = CCLayerColor::create({0, 0, 0, 95});
    iconListBG->setAnchorPoint(ccp(1.f, 0.5f));
    iconListBG->ignoreAnchorPointForPosition(false);
    iconListBG->setContentSize(ccp(160.f, 190.f));
    m_mainLayer->addChildAtPosition(iconListBG, Anchor::Right, ccp(-20.f, 10.f));

    m_iconList = ScrollLayer::create(iconListBG->getContentSize());
    iconListBG->addChild(m_iconList);

    m_iconListScrollbar = Scrollbar::create(m_iconList);
    iconListBG->addChildAtPosition(
        m_iconListScrollbar, Anchor::Right, ccp(5.f, 0.f), false
    );

    auto constexpr iconOrderYOffset = 10.f;

    m_iconOrderLabel = CCLabelBMFont::create(
        "",
        "bigFont.fnt"
    );
    m_iconOrderLabel->setScale(0.8f);
    m_mainLayer->addChildAtPosition(
        m_iconOrderLabel, Anchor::Left, ccp(110.f, iconOrderYOffset)
    );

    auto iconOrderRArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconOrderRArrowSpr->setScale(0.7f);
    iconOrderRArrowSpr->setFlipX(true);
    auto iconOrderRArrowBtn = CCMenuItemSpriteExtra::create(
        iconOrderRArrowSpr,
        this,
        menu_selector(IconConfigLayer::onOrderArrow)
    );
    iconOrderRArrowBtn->setTag(1);

    auto iconOrderLArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconOrderLArrowSpr->setScale(0.7f);
    auto iconOrderLArrowBtn = CCMenuItemSpriteExtra::create(
        iconOrderLArrowSpr,
        this,
        menu_selector(IconConfigLayer::onOrderArrow)
    );
    iconOrderLArrowBtn->setTag(-1);

    m_buttonMenu->addChildAtPosition(
        iconOrderRArrowBtn, Anchor::Left, ccp(180.f, iconOrderYOffset)
    );
    m_buttonMenu->addChildAtPosition(
        iconOrderLArrowBtn, Anchor::Left, ccp(40.f, iconOrderYOffset)
    );

    auto iconOrderTitle = CCLabelBMFont::create("Icon Order", "goldFont.fnt");
    iconOrderTitle->setScale(0.7f);
    m_mainLayer->addChildAtPosition(
        iconOrderTitle, Anchor::Left, ccp(110.f, iconOrderYOffset + 20.f)
    );

    auto const mirrorEndOffset = ccp(30.f, -20.f);
    m_mirrorEndBtn = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(IconConfigLayer::onVarToggle),
        0.6f
    );
    m_buttonMenu->addChildAtPosition(m_mirrorEndBtn, Anchor::Left, mirrorEndOffset);
    
    auto mirrorEndText = CCLabelBMFont::create("Mirror after End", "bigFont.fnt");
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
    );

    auto iconListMenu = CCMenu::create();
    iconListMenu->ignoreAnchorPointForPosition(false);
    iconListMenu->setContentSize(ccp(180.f, 50.f));
    iconListMenu->setLayout(
        RowLayout::create()
    );
    m_mainLayer->addChildAtPosition(iconListMenu, Anchor::Left, ccp(110.f, -65.f));

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
        nullptr
    );
    auto iconListLoadSpr = CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png");
    iconListLoadSpr->setScale(0.83f);
    auto iconListLoadBtn = CCMenuItemSpriteExtra::create(
        iconListLoadSpr,
        this,
        nullptr
    );

    iconListMenu->addChild(iconListAddBtn);
    iconListMenu->addChild(iconListClearBtn);
    iconListMenu->addChild(iconListSaveBtn);
    iconListMenu->addChild(iconListLoadBtn);
    iconListMenu->updateLayout();

    IconConfigLayer::refreshTab();
    return true;
}

IconConfigData& IconConfigLayer::getCurrentConfig() {
    return m_configManager->getConfigOfType(m_currentTab);
}

void IconConfigLayer::onSwitchTab(CCObject* sender) {
    m_currentTab = static_cast<IconType>(sender->getTag());
    m_configManager->getGlobalConfig().currentTab = m_currentTab;
    IconConfigLayer::refreshTab();
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

    auto& currentConfig = IconConfigLayer::getCurrentConfig();
    m_randomBtn->toggle(currentConfig.random);
    m_randomBtn->setUserObject(CIVariableRef<bool>::create(currentConfig.random));
    m_disableBtn->toggle(currentConfig.disabled);
    m_disableBtn->setUserObject(CIVariableRef<bool>::create(currentConfig.disabled));
    m_mirrorEndBtn->toggle(currentConfig.mirrorEnd);
    m_mirrorEndBtn->setUserObject(CIVariableRef<bool>::create(currentConfig.mirrorEnd));

    IconConfigLayer::setOrderChoice(currentConfig.order);
    IconConfigLayer::refreshIconList(m_currentTab, true);
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
    if (auto ref = typeinfo_cast<CIVariableRef<bool>*>(obj)) {
        ref->getVarRef() = !btn->m_toggled;
    }
}

void IconConfigLayer::onOrderArrow(CCObject* sender) {
    auto& currentConfig = IconConfigLayer::getCurrentConfig();
    int choiceTemp = static_cast<int>(currentConfig.order) + sender->getTag();
    if (choiceTemp < 0)
        choiceTemp = m_iconOrderList.size() - 1;
    else if (choiceTemp >= m_iconOrderList.size())
        choiceTemp = 0;

    IconConfigLayer::setOrderChoice(choiceTemp);
    currentConfig.order = static_cast<IconOrder>(choiceTemp);
}

void IconConfigLayer::setOrderChoice(IconOrder choice) {
    m_iconOrderLabel->setString(m_iconOrderList.at(static_cast<int>(choice)).c_str());
}

void IconConfigLayer::setOrderChoice(int choice) {
    m_iconOrderLabel->setString(m_iconOrderList.at(choice).c_str());
}

void IconConfigLayer::onAddIcon(CCObject*) {
    AddIconLayer::create(m_currentTab)->show();
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

void IconConfigLayer::refreshIconList(IconType currentTab, bool toTop) {
    auto& currentConfig = IconConfigLayer::getCurrentConfig();
    auto iconList = currentConfig.iconSet;
    auto content = m_iconList->m_contentLayer;

    auto height = std::max<int>(m_iconList->getContentHeight(), iconList.size() * 30.f);
    content->setContentHeight(height);

    m_iconList->m_contentLayer->removeAllChildren();
    size_t i = 0;
    for (auto const& icon : iconList) {
        bool isLast = false;
        if (i == iconList.size() - 1) isLast = true;
        auto cell = IconCell::create(
            i,
            currentTab,
            icon.iconID,
            icon.color1,
            icon.color2,
            isLast
        );
        cell->setPositionY(height - (i + 1) * 30.f);
        cell->setTag(i);
        content->addChild(cell);
        i++;
    }
    if (toTop) {
        m_iconList->moveToTop();
        return;
    }
    auto maxScrollPos = m_iconList->getContentHeight() - content->getContentHeight();
    content->setPositionY(std::clamp<float>(
        content->getPositionY() + 30.f,
        maxScrollPos,
        0.f
    ));
}

void IconConfigLayer::addIcon(IconProperties icon) {
    log::debug("Added icon ID {}", icon.iconID);
    IconConfigLayer::getCurrentConfig().iconSet.push_back(icon);
    IconConfigLayer::refreshIconList(m_currentTab);
}

void IconConfigLayer::swapIcons(int icon1, int icon2) {
    log::debug("Swapped icons in index {} and {}", icon1, icon2);
    auto& iconSet = IconConfigLayer::getCurrentConfig().iconSet;
    std::iter_swap(iconSet.begin() + icon1, iconSet.begin() + icon2);
    IconConfigLayer::refreshIconList(m_currentTab);
}

void IconConfigLayer::deleteIcon(int index) {
    log::debug("Removed icon at index {}", index);
    auto& iconSet = IconConfigLayer::getCurrentConfig().iconSet;
    iconSet.erase(iconSet.begin() + index);
    IconConfigLayer::refreshIconList(m_currentTab);
}

IconConfigLayer::~IconConfigLayer() {
    m_instance = nullptr;
    log::debug("Saving config.");
    Mod::get()->setSavedValue("global", m_configManager->getGlobalConfig());
    Mod::get()->setSavedValue("cube", m_configManager->getConfigOfType(IconType::Cube));
    Mod::get()->setSavedValue("ship", m_configManager->getConfigOfType(IconType::Ship));
    Mod::get()->setSavedValue("ball", m_configManager->getConfigOfType(IconType::Ball));
    Mod::get()->setSavedValue("bird", m_configManager->getConfigOfType(IconType::Ufo));
    Mod::get()->setSavedValue("dart", m_configManager->getConfigOfType(IconType::Wave));
    Mod::get()->setSavedValue("robot", m_configManager->getConfigOfType(IconType::Robot));
    Mod::get()->setSavedValue("spider", m_configManager->getConfigOfType(IconType::Spider));
    Mod::get()->setSavedValue("swing", m_configManager->getConfigOfType(IconType::Swing));
    Mod::get()->setSavedValue("jetpack", m_configManager->getConfigOfType(IconType::Jetpack));
}