#include "GlobalConfigLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <class/CCMenuItemTriToggler.hpp>
#include <class/CCVariableRef.hpp>

using namespace geode::prelude;
using namespace changing_icons;

GlobalConfigLayer* GlobalConfigLayer::create() {
    auto ret = new GlobalConfigLayer();
    if (ret && ret->initAnchored(400.f, 280.f, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GlobalConfigLayer::setup() {
    m_configManager = CIConfigManager::get();
    m_noElasticity = true;
    this->setTitle("Mod Configurations");
    auto& globalConfig = m_configManager->getGlobalConfig();

    auto globalOverrideLabel = CCLabelBMFont::create("Global Overrides", "bigFont.fnt");
    globalOverrideLabel->setScale(0.5f);
    m_mainLayer->addChildAtPosition(globalOverrideLabel, Anchor::Left, ccp(120.f, 95.f));

    auto globalOverrideBg = CCScale9Sprite::create("square02_001.png");
    globalOverrideBg->setAnchorPoint(ccp(0.f, 0.5f));
    globalOverrideBg->setContentSize(ccp(200.f, 150.f));
    globalOverrideBg->setOpacity(95);
    m_mainLayer->addChildAtPosition(globalOverrideBg, Anchor::Left, ccp(20.f, 10.f));

    auto globalOverrideMenu = CCMenu::create();
    globalOverrideMenu->ignoreAnchorPointForPosition(false);
    globalOverrideMenu->setContentSize(globalOverrideBg->getContentSize());
    globalOverrideBg->addChildAtPosition(globalOverrideMenu, Anchor::Center);

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
    globalOverrideMenu->addChildAtPosition(disableBtn, Anchor::Left, ccp(20.f, 55.f));

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
    globalOverrideMenu->addChildAtPosition(useAllBtn, Anchor::Left, ccp(20.f, 30.f));

    auto constexpr iconOrderYOffset = -20.f;

    m_iconOrderLabel = CCLabelBMFont::create(
        "Forward",
        "bigFont.fnt"
    );
    m_iconOrderLabel->setScale(0.75f);
    globalOverrideBg->addChildAtPosition(
        m_iconOrderLabel, Anchor::Left, ccp(100.f, iconOrderYOffset)
    );

    auto iconOrderRArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconOrderRArrowSpr->setScale(0.7f);
    iconOrderRArrowSpr->setFlipX(true);
    auto iconOrderRArrowBtn = CCMenuItemSpriteExtra::create(
        iconOrderRArrowSpr,
        this,
        menu_selector(GlobalConfigLayer::onOrderArrow)
    );
    iconOrderRArrowBtn->setTag(1);

    auto iconOrderLArrowSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    iconOrderLArrowSpr->setScale(0.7f);
    auto iconOrderLArrowBtn = CCMenuItemSpriteExtra::create(
        iconOrderLArrowSpr,
        this,
        menu_selector(GlobalConfigLayer::onOrderArrow)
    );
    iconOrderLArrowBtn->setTag(-1);

    globalOverrideMenu->addChildAtPosition(
        iconOrderRArrowBtn, Anchor::Left, ccp(175.f, iconOrderYOffset)
    );
    globalOverrideMenu->addChildAtPosition(
        iconOrderLArrowBtn, Anchor::Left, ccp(25.f, iconOrderYOffset)
    );

    auto iconOrderTitle = CCLabelBMFont::create("Icon Order", "goldFont.fnt");
    iconOrderTitle->setScale(0.7f);
    globalOverrideBg->addChildAtPosition(
        iconOrderTitle, Anchor::Left, ccp(100.f, iconOrderYOffset + 25.f)
    );

    auto enableIconOrderBtn = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(GlobalConfigLayer::onDisableOrder), 0.5f
    );
    globalOverrideMenu->addChildAtPosition(
        enableIconOrderBtn, Anchor::Left, ccp(162.f, iconOrderYOffset + 25.f)
    );

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
    globalOverrideMenu->addChildAtPosition(mirrorEndBtn, Anchor::Left, ccp(20.f, -55.f));

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
    m_iconOrderEnabled = globalConfig.override.order.has_value();
    enableIconOrderBtn->toggle(m_iconOrderEnabled);
    if (!m_iconOrderEnabled) disableOrder();
    else setOrderChoice(globalConfig.override.order.value());
    mirrorEndBtn->setState(globalConfig.override.mirrorEnd);
    for (auto btn : CCArrayExt<CCMenuItemToggler*>(m_gamemodeBar->getChildren())) {
        auto btnType = static_cast<IconType>(btn->getTag());
        if (std::find(globalConfig.globalOverrides.begin(), globalConfig.globalOverrides.end(), btnType) != globalConfig.globalOverrides.end()) {
            btn->toggle(true);
        } else {
            btn->toggle(false);
        }
    }

    return true;
}

void GlobalConfigLayer::onVarTriToggle(CCObject* sender) {
    auto btn = static_cast<CCMenuItemTriToggler*>(sender);
    auto obj = static_cast<CCNode*>(sender)->getUserObject();
    if (auto ref = typeinfo_cast<CCVariableRef<std::optional<bool>>*>(obj)) {
        ref->getVarRef() = btn->getState();
    }
}

void GlobalConfigLayer::disableOrder() {
    m_iconOrderLabel->setString("Disabled");
    m_iconOrderLabel->setColor(cc3x(0x7f));
}

void GlobalConfigLayer::onOrderArrow(CCObject* sender) {
    if (!m_iconOrderEnabled) return;
    auto& globalConfig = m_configManager->getGlobalConfig();
    int choiceTemp = static_cast<int>(globalConfig.override.order.value_or(IconOrder::Random)) + sender->getTag();
    if (choiceTemp < 0)
        choiceTemp = m_iconOrderList.size() - 1;
    else if (choiceTemp >= m_iconOrderList.size())
        choiceTemp = 0;

    setOrderChoice(choiceTemp);
    globalConfig.override.order = static_cast<IconOrder>(choiceTemp);
}

void GlobalConfigLayer::setOrderChoice(IconOrder choice) {
    m_iconOrderLabel->setString(m_iconOrderList.at(static_cast<int>(choice)).c_str());
    m_iconOrderLabel->setColor(cc3x(0xf));
}

void GlobalConfigLayer::setOrderChoice(int choice) {
    m_iconOrderLabel->setString(m_iconOrderList.at(choice).c_str());
}

void GlobalConfigLayer::onDisableOrder(CCObject* sender) {
    auto& globalConfig = m_configManager->getGlobalConfig();
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    m_iconOrderEnabled = !btn->m_toggled;
    if (!m_iconOrderEnabled) {
        globalConfig.override.order = std::nullopt;
        disableOrder();
    } else {
        globalConfig.override.order = IconOrder::Random;
        setOrderChoice(IconOrder::Random);
    }
}

void GlobalConfigLayer::onAddToOverride(CCObject* sender) {
    auto& overrideList = m_configManager->getGlobalConfig().globalOverrides;
    auto type = static_cast<IconType>(sender->getTag());
    if (std::find(overrideList.begin(), overrideList.end(), type) != overrideList.end()) {
        overrideList.erase(type);
    } else {
        overrideList.insert(type);
    }
}

GlobalConfigLayer::~GlobalConfigLayer() {
    log::debug("Saving global config.");
    Mod::get()->setSavedValue("global", m_configManager->getGlobalConfig());
}