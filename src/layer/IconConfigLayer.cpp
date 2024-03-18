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
    // Config
    m_globalConfig = Mod::get()->getSavedValue<GlobalConfigData>("global");
    m_cubeConfig = Mod::get()->getSavedValue<IconConfigData>("cube");
    m_shipConfig = Mod::get()->getSavedValue<IconConfigData>("ship");
    m_ballConfig = Mod::get()->getSavedValue<IconConfigData>("ball");
    m_birdConfig = Mod::get()->getSavedValue<IconConfigData>("bird");
    m_dartConfig = Mod::get()->getSavedValue<IconConfigData>("dart");
    m_robotConfig = Mod::get()->getSavedValue<IconConfigData>("robot");
    m_spiderConfig = Mod::get()->getSavedValue<IconConfigData>("spider");
    m_swingConfig = Mod::get()->getSavedValue<IconConfigData>("swing");
    m_jetpackConfig = Mod::get()->getSavedValue<IconConfigData>("jetpack");

    m_noElasticity = true;
    m_currentTab = m_globalConfig.currentTab;
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

    m_randomBtn = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(IconConfigLayer::onVarToggle),
        0.6f
    );
    m_buttonMenu->addChildAtPosition(m_randomBtn, Anchor::TopLeft, ccp(30.f, -45.f));

    auto randomText = CCLabelBMFont::create("Random", "bigFont.fnt");
    randomText->setAnchorPoint(ccp(0.f, 0.5f));
    randomText->setScale(0.5f);
    m_mainLayer->addChildAtPosition(randomText, Anchor::TopLeft, ccp(45.f, -45.f));

    auto randomInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    randomInfoSpr->setScale(0.4f);
    auto randomInfoBtn = CCMenuItemSpriteExtra::create(
        randomInfoSpr,
        this,
        menu_selector(IconConfigLayer::onVarInfo)
    );
    randomInfoBtn->setUserObject(CCString::create("Randomly select from entire icon kit instead of from the list."));
    m_buttonMenu->addChildAtPosition(randomInfoBtn, Anchor::TopLeft, ccp(17.f, -32.f));

    m_disableBtn = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(IconConfigLayer::onVarToggle),
        0.6f
    );
    m_buttonMenu->addChildAtPosition(m_disableBtn, Anchor::TopLeft, ccp(130.f, -45.f));

    auto disableText = CCLabelBMFont::create("Disable", "bigFont.fnt");
    disableText->setAnchorPoint(ccp(0.f, 0.5f));
    disableText->setScale(0.5f);
    m_mainLayer->addChildAtPosition(disableText, Anchor::TopLeft, ccp(145.f, -45.f));

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

    m_iconOrderLabel = CCLabelBMFont::create(
        "",
        "bigFont.fnt"
    );
    m_iconOrderLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_iconOrderLabel, Anchor::TopLeft, ccp(110.f, -80.f));

    auto iconOrderRArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconOrderRArrowSpr->setScale(0.5f);
    iconOrderRArrowSpr->setFlipX(true);
    auto iconOrderRArrowBtn = CCMenuItemSpriteExtra::create(
        iconOrderRArrowSpr,
        this,
        menu_selector(IconConfigLayer::onOrderArrow)
    );
    iconOrderRArrowBtn->setTag(1);

    auto iconOrderLArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconOrderLArrowSpr->setScale(0.5f);
    auto iconOrderLArrowBtn = CCMenuItemSpriteExtra::create(
        iconOrderLArrowSpr,
        this,
        menu_selector(IconConfigLayer::onOrderArrow)
    );
    iconOrderLArrowBtn->setTag(-1);

    m_buttonMenu->addChildAtPosition(iconOrderRArrowBtn, Anchor::TopLeft, ccp(170.f, -80.f));
    m_buttonMenu->addChildAtPosition(iconOrderLArrowBtn, Anchor::TopLeft, ccp(50.f, -80.f));

    auto iconOrderTitle = CCLabelBMFont::create("Icon Order", "goldFont.fnt");
    iconOrderTitle->setScale(0.5f);
    m_mainLayer->addChildAtPosition(iconOrderTitle, Anchor::TopLeft, ccp(110.f, -65.f));

    auto iconListMenu = CCMenu::create();
    iconListMenu->ignoreAnchorPointForPosition(false);
    iconListMenu->setContentSize(ccp(56.f, 56.f));
    m_mainLayer->addChildAtPosition(iconListMenu, Anchor::Left, ccp(110.f, -20.f));

    auto iconListAddBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"),
        this,
        menu_selector(IconConfigLayer::onAddIcon)
    );
    auto iconListClearBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"),
        this,
        nullptr
    );
    auto iconListSaveBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png"),
        this,
        nullptr
    );

    iconListMenu->addChildAtPosition(iconListAddBtn, Anchor::TopLeft);
    iconListMenu->addChildAtPosition(iconListSaveBtn, Anchor::TopRight);
    iconListMenu->addChildAtPosition(iconListClearBtn, Anchor::BottomLeft);

    IconConfigLayer::refreshTab();
    m_iconList->moveToTop();
    return true;
}

IconConfigData& IconConfigLayer::getCurrentConfig() {
    switch (m_currentTab) {
        default:
        case IconType::Cube: return m_cubeConfig;
        case IconType::Ship: return m_shipConfig;
        case IconType::Ball: return m_ballConfig;
        case IconType::Ufo: return m_birdConfig;
        case IconType::Wave: return m_dartConfig;
        case IconType::Robot: return m_robotConfig;
        case IconType::Spider: return m_spiderConfig;
        case IconType::Swing: return m_swingConfig;
        case IconType::Jetpack: return m_jetpackConfig;
    }
}

void IconConfigLayer::onSwitchTab(CCObject* sender) {
    m_currentTab = static_cast<IconType>(sender->getTag());
    m_globalConfig.currentTab = m_currentTab;
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

    IconConfigLayer::setOrderChoice(currentConfig.order);
    IconConfigLayer::refreshIconList(m_currentTab);
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

void IconConfigLayer::refreshIconList(IconType currentTab) {
    auto& currentConfig = IconConfigLayer::getCurrentConfig();
    auto iconList = currentConfig.iconSet;
    auto content = m_iconList->m_contentLayer;

    auto height = std::max<int>(m_iconList->getContentHeight(), iconList.size() * 30.f);
    content->setContentHeight(height);

    m_iconList->m_contentLayer->removeAllChildren();
    size_t i = 0;
    for (auto const& icon : iconList) {
        auto cell = IconCell::create(
            i,
            currentTab,
            icon.iconID,
            icon.color1,
            icon.color2
        );
        cell->setPositionY(height - (i + 1) * 30.f);
        cell->setTag(i);
        content->addChild(cell);
        i++;
    }
    m_iconListScrollbar->setTarget(m_iconList);
}

void IconConfigLayer::addIcon(IconProperties icon) {
    log::debug("Added icon ID {}", icon.iconID);
    IconConfigLayer::getCurrentConfig().iconSet.push_back(icon);
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
    Mod::get()->setSavedValue("global", m_globalConfig);
    Mod::get()->setSavedValue("cube", m_cubeConfig);
    Mod::get()->setSavedValue("ship", m_shipConfig);
    Mod::get()->setSavedValue("ball", m_ballConfig);
    Mod::get()->setSavedValue("bird", m_birdConfig);
    Mod::get()->setSavedValue("dart", m_dartConfig);
    Mod::get()->setSavedValue("robot", m_robotConfig);
    Mod::get()->setSavedValue("spider", m_spiderConfig);
    Mod::get()->setSavedValue("swing", m_swingConfig);
    Mod::get()->setSavedValue("jetpack", m_jetpackConfig);
}