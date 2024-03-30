#include "GlobalConfigLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <class/CCMenuItemTriToggler.hpp>
#include <class/CCVariableRef.hpp>
#include "TriTogglerInfoLayer.hpp"

using namespace geode::prelude;
using namespace changing_icons;

GlobalConfigLayer* GlobalConfigLayer::create() {
    auto ret = new GlobalConfigLayer();
    if (ret && ret->initAnchored(400.f, 300.f, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GlobalConfigLayer::setup() {
    m_configManager = CIConfigManager::get();
    m_noElasticity = true;
    this->setTitle("Global Overrides");
    auto& globalConfig = m_configManager->getGlobalConfig();

    static_cast<AnchorLayoutOptions*>(m_closeBtn->getLayoutOptions())
        ->setOffset(ccp(10.f, -10.f));
    m_buttonMenu->updateLayout();

    auto globalOverrideBg = CCScale9Sprite::create("square02_001.png");
    globalOverrideBg->setAnchorPoint(ccp(0.5f, 0.5f));
    globalOverrideBg->setContentSize(ccp(200.f, 170.f));
    globalOverrideBg->setOpacity(95);
    m_mainLayer->addChildAtPosition(globalOverrideBg, Anchor::Center, ccp(0.f, 20.f));

    auto globalOverrideMenu = CCMenu::create();
    globalOverrideMenu->ignoreAnchorPointForPosition(false);
    globalOverrideMenu->setContentSize(globalOverrideBg->getContentSize());
    globalOverrideBg->addChildAtPosition(globalOverrideMenu, Anchor::Center);

    auto triTogglerInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    triTogglerInfoSpr->setScale(0.7f);
    auto triTogglerInfoBtn = CCMenuItemSpriteExtra::create(
        triTogglerInfoSpr, this, menu_selector(GlobalConfigLayer::onTriTogglerInfo)
    );
    globalOverrideMenu->addChildAtPosition(triTogglerInfoBtn, Anchor::TopRight, ccp(-10.f, -10.f));

    auto constexpr toggleYPos = 65.f;
    auto constexpr toggleOffset = 24.f;

    auto disableBtn = CCMenuItemTriToggler::createWithLabel(
        CCSprite::create("CI_checkDisabled.png"_spr),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onVarTriToggle),
        "Disable",
        0.6f
    );
    disableBtn->setUserObject(CCVariableRef<std::optional<bool>>::create(globalConfig.override.disabled));
    globalOverrideMenu->addChildAtPosition(disableBtn, Anchor::Left, ccp(20.f, toggleYPos));

    auto useAllBtn = CCMenuItemTriToggler::createWithLabel(
        CCSprite::create("CI_checkDisabled.png"_spr),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onVarTriToggle),
        "Use All Icons",
        0.6f
    );
    useAllBtn->setUserObject(CCVariableRef<std::optional<bool>>::create(globalConfig.override.useAll));
    globalOverrideMenu->addChildAtPosition(useAllBtn, Anchor::Left, ccp(20.f, toggleYPos - toggleOffset));

    auto includePlayerBtn = CCMenuItemTriToggler::createWithLabel(
        CCSprite::create("CI_checkDisabled.png"_spr),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onVarTriToggle),
        "Include Player Icon",
        0.6f
    );
    includePlayerBtn->setUserObject(CCVariableRef<std::optional<bool>>::create(globalConfig.override.includePlayerIcon));
    globalOverrideMenu->addChildAtPosition(includePlayerBtn, Anchor::Left, ccp(20.f, toggleYPos - toggleOffset * 2));

    auto constexpr iconOrderYPos = 2.f;
    auto iconOrderLabel = CCLabelBMFont::create("Icon Order", "goldFont.fnt");
    iconOrderLabel->setScale(0.65f);
    globalOverrideBg->addChildAtPosition(iconOrderLabel, Anchor::Center, ccp(0.f, iconOrderYPos));

    m_iconOrderMenu = CCMenu::create();
    m_iconOrderMenu->ignoreAnchorPointForPosition(false);
    m_iconOrderMenu->setContentSize(ccp(20.f, 20.f));
    globalOverrideBg->addChildAtPosition(m_iconOrderMenu, Anchor::Center, ccp(0.f, iconOrderYPos - 22.f));

    auto randomOrderSpr = ButtonSprite::create("Random", "bigFont.fnt", "GJ_button_04.png");
    randomOrderSpr->setScale(0.5f);
    auto randomOrderBtn = CCMenuItemSpriteExtra::create(
        randomOrderSpr,
        this,
        menu_selector(GlobalConfigLayer::onOrderButton)
    );
    randomOrderBtn->setTag(0);
    m_iconOrderMenu->addChildAtPosition(randomOrderBtn, Anchor::Center);

    auto forwardOrderSpr = ButtonSprite::create("Forward", "bigFont.fnt", "GJ_button_04.png");
    forwardOrderSpr->setScale(0.5f);
    auto forwardOrderBtn = CCMenuItemSpriteExtra::create(
        forwardOrderSpr,
        this,
        menu_selector(GlobalConfigLayer::onOrderButton)
    );
    forwardOrderBtn->setTag(1);
    m_iconOrderMenu->addChildAtPosition(forwardOrderBtn, Anchor::Center, ccp(-45.f, -20.f));

    auto backwardOrderSpr = ButtonSprite::create("Backward", "bigFont.fnt", "GJ_button_04.png");
    backwardOrderSpr->setScale(0.5f);
    auto backwardOrderBtn = CCMenuItemSpriteExtra::create(
        backwardOrderSpr,
        this,
        menu_selector(GlobalConfigLayer::onOrderButton)
    );
    backwardOrderBtn->setTag(2);
    m_iconOrderMenu->addChildAtPosition(backwardOrderBtn, Anchor::Center, ccp(45.f, -20.f));

    auto mirrorEndBtn = CCMenuItemTriToggler::createWithLabel(
        CCSprite::create("CI_checkDisabled.png"_spr),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onVarTriToggle),
        "Mirror After End",
        0.6f
    );
    mirrorEndBtn->setUserObject(CCVariableRef<std::optional<bool>>::create(globalConfig.override.mirrorEnd));
    globalOverrideMenu->addChildAtPosition(mirrorEndBtn, Anchor::Left, ccp(20.f, -65.f));

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
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    cubeBtn->setTag(0);
    auto shipBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_shipBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_shipBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    shipBtn->setTag(1);
    auto ballBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_ballBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_ballBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    ballBtn->setTag(2);
    auto birdBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_birdBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_birdBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    birdBtn->setTag(3);
    auto dartBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_dartBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    dartBtn->setTag(4);
    auto robotBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_robotBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_robotBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    robotBtn->setTag(5);
    auto spiderBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_spiderBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_spiderBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    spiderBtn->setTag(6);
    auto swingBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_swingBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_swingBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
    );
    swingBtn->setTag(7);
    auto jetpackBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_jetpackBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_jetpackBtn_on_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onAddToOverride)
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

    auto gamemodeOverrideText = CCLabelBMFont::create(
        "Override In", "bigFont.fnt"
    );
    gamemodeOverrideText->setScale(0.6f);
    m_mainLayer->addChildAtPosition(gamemodeOverrideText, Anchor::Bottom, ccp(0.f, 55.f));

    // Initialize values
    disableBtn->setState(globalConfig.override.disabled);
    useAllBtn->setState(globalConfig.override.useAll);
    includePlayerBtn->setState(globalConfig.override.includePlayerIcon);
    if (globalConfig.override.order)
        setOrderChoice(globalConfig.override.order.value());
    mirrorEndBtn->setState(globalConfig.override.mirrorEnd);
    for (auto btn : CCArrayExt<CCMenuItemToggler*>(m_gamemodeBar->getChildren())) {
        auto btnType = static_cast<IconType>(btn->getTag());
        if (globalConfig.globalOverrides.find(btnType) != globalConfig.globalOverrides.end()) {
            btn->toggle(true);
        } else {
            btn->toggle(false);
        }
    }

    return true;
}

void GlobalConfigLayer::onTriTogglerInfo(CCObject* sender) {
    TriTogglerInfoLayer::create()->show();
}

void GlobalConfigLayer::onVarTriToggle(CCObject* sender) {
    auto btn = static_cast<CCMenuItemTriToggler*>(sender);
    auto obj = static_cast<CCNode*>(sender)->getUserObject();
    if (auto ref = typeinfo_cast<CCVariableRef<std::optional<bool>>*>(obj)) {
        ref->getVarRef() = btn->getState();
    }
}

void GlobalConfigLayer::onOrderButton(CCObject* sender) {
    auto& overrideVars = m_configManager->getGlobalConfig().override;
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto spr = static_cast<ButtonSprite*>(btn->getNormalImage());
    if (static_cast<IconOrder>(btn->getTag()) == overrideVars.order) {
        overrideVars.order = std::nullopt;
        spr->updateBGImage("GJ_button_04.png");
        btn->setContentSize(spr->getScaledContentSize());
        spr->setPosition(btn->getContentSize() / 2);
        return;
    }
    overrideVars.order = static_cast<IconOrder>(btn->getTag());
    setOrderChoice(btn->getTag());
}

void GlobalConfigLayer::setOrderChoice(IconOrder choice) {
    GlobalConfigLayer::setOrderChoice(static_cast<int>(choice));
}

void GlobalConfigLayer::setOrderChoice(int choice) {
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

void GlobalConfigLayer::onAddToOverride(CCObject* sender) {
    auto& overrideList = m_configManager->getGlobalConfig().globalOverrides;
    auto type = static_cast<IconType>(sender->getTag());
    if (overrideList.find(type) != overrideList.end()) {
        overrideList.erase(type);
    } else {
        overrideList.insert(type);
    }
}

GlobalConfigLayer::~GlobalConfigLayer() {
    log::debug("Saving global config.");
    Mod::get()->setSavedValue("global", m_configManager->getGlobalConfig());
}