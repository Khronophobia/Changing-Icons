#include "GlobalConfigLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <class/CCMenuItemTriToggler.hpp>
#include <class/CCVariableRef.hpp>
#include <class/dropdown/DropdownMenu.hpp>
#include "TriTogglerInfoLayer.hpp"
#include <Geode/ui/GeodeUI.hpp>

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
    m_configManager = CIManager::get();
    m_noElasticity = true;
    this->setTitle("Global Settings");
    auto& globalConfig = m_configManager->getGlobalConfig();

    static_cast<AnchorLayoutOptions*>(m_closeBtn->getLayoutOptions())
        ->setOffset(ccp(10.f, -10.f));
    m_buttonMenu->updateLayout();

    auto modSettingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"),
        this,
        menu_selector(GlobalConfigLayer::onModSettings)
    );
    m_buttonMenu->addChildAtPosition(modSettingsBtn, Anchor::TopRight, ccp(-10.f, -10.f));

    auto globalOverrideBg = CCScale9Sprite::create("square02_001.png");
    globalOverrideBg->setAnchorPoint(ccp(0.5f, 0.5f));
    globalOverrideBg->setContentSize(ccp(200.f, 180.f));
    globalOverrideBg->setOpacity(95);
    m_mainLayer->addChildAtPosition(globalOverrideBg, Anchor::Center, ccp(0.f, 20.f));

    auto globalOverrideLabel = CCLabelBMFont::create("Overrides", "bigFont.fnt");
    globalOverrideLabel->setScale(0.55f);
    globalOverrideBg->addChildAtPosition(globalOverrideLabel, Anchor::Top, ccp(0.f, -10.f));

    auto globalOverrideMenu = CCMenu::create();
    globalOverrideMenu->ignoreAnchorPointForPosition(false);
    globalOverrideBg->addChildAtPosition(globalOverrideMenu, Anchor::Center);

    globalOverrideBg->setLayout(CopySizeLayout::create()->add(globalOverrideMenu));

    auto triTogglerInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    triTogglerInfoSpr->setScale(0.5f);
    auto triTogglerInfoBtn = CCMenuItemSpriteExtra::create(
        triTogglerInfoSpr, this, menu_selector(GlobalConfigLayer::onTriTogglerInfo)
    );
    globalOverrideMenu->addChildAtPosition(triTogglerInfoBtn, Anchor::TopRight, ccp(-12.f, -12.f));

    auto constexpr toggleYPos = 60.f;
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

    auto iconOrderLabel = CCLabelBMFont::create("Order", "bigFont.fnt");
    iconOrderLabel->setAnchorPoint(ccp(0.f, 0.5f));
    iconOrderLabel->setScale(0.6f);
    globalOverrideBg->addChildAtPosition(iconOrderLabel, Anchor::Left, ccp(12.f, 8.f));

    m_iconOrderDropdown = DropdownMenu::create(
        {"Random", "Forward", "Backward", "Shuffle", "Don't Override"},
        120.f,
        this, menu_selector(GlobalConfigLayer::onOrderDropdown)
    );
    m_iconOrderDropdown->setAnchorPoint(ccp(1.f, 0.5f));
    m_iconOrderDropdown->setScale(0.9f);
    globalOverrideBg->addChildAtPosition(m_iconOrderDropdown, Anchor::Right, ccp(-12.f, 8.f));

    auto constexpr listCheckboxYPos = -20.f;

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
    globalOverrideMenu->addChildAtPosition(includePlayerBtn, Anchor::Left, ccp(20.f, listCheckboxYPos));

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
    globalOverrideMenu->addChildAtPosition(mirrorEndBtn, Anchor::Left, ccp(20.f, listCheckboxYPos - toggleOffset));

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
    if (globalConfig.override.order) {
        m_iconOrderDropdown->setChoice(static_cast<int>(globalConfig.override.order.value()));
    } else {
        m_iconOrderDropdown->setChoice(m_iconOrderDropdown->getChoiceCount() - 1);
    }
    includePlayerBtn->setState(globalConfig.override.includePlayerIcon);
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

void GlobalConfigLayer::onOrderDropdown(CCObject* sender) {
    auto& globalConfig = m_configManager->getGlobalConfig();
    if (sender->getTag() >= m_iconOrderDropdown->getChoiceCount() - 1) {
        globalConfig.override.order = std::nullopt;
    } else {
        globalConfig.override.order = static_cast<IconOrder>(sender->getTag());
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

void GlobalConfigLayer::onModSettings(CCObject* sender) {
    openSettingsPopup(Mod::get());
}

GlobalConfigLayer::~GlobalConfigLayer() {
    log::debug("Saving global config.");
    Mod::get()->setSavedValue("global", m_configManager->getGlobalConfig());
}