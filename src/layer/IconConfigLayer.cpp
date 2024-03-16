#include <Geode/Geode.hpp>
#include <Geode/Enums.hpp>
#include "IconConfigLayer.hpp"
#include "../class/CIVariableRef.hpp"

using namespace geode::prelude;
using namespace changing_icons;

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
    m_noElasticity = true;
    m_activeTab = IconType::Cube;
    this->setTitle("Changing Icons Config");

    m_gamemodeBar = CCMenu::create();
    m_gamemodeBar->ignoreAnchorPointForPosition(false);
    m_gamemodeBar->setContentWidth(m_size.width - 40.f);
    m_gamemodeBar->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    m_mainLayer->addChildAtPosition(m_gamemodeBar, Anchor::Center, ccp(0.f, 90.f));

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

    auto cubeBtn = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("gj_iconBtn_off_001.png"),
        CCSprite::createWithSpriteFrameName("gj_iconBtn_on_001.png"),
        this,
        menu_selector(IconConfigLayer::onSwitchTab)
    );
    cubeBtn->setTag(0);
    cubeBtn->toggle(true);
    cubeBtn->setEnabled(false);
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
        0.7f
    );
    m_randomBtn->toggle(m_cubeConfig.random);
    m_randomBtn->setUserObject(CIVariableRef<bool>::create(m_cubeConfig.random));
    m_buttonMenu->addChildAtPosition(m_randomBtn, Anchor::Left, ccp(40.f, 56.f));

    auto randomText = CCLabelBMFont::create("Random", "bigFont.fnt");
    randomText->setAnchorPoint(ccp(0.f, 0.5f));
    randomText->setScale(0.5f);
    m_mainLayer->addChildAtPosition(randomText, Anchor::Left, ccp(54.f, 56.f));

    return true;
}

IconConfigData& IconConfigLayer::getCurrentConfig(IconType activeTab) {
    switch (activeTab) {
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
    for (auto btn : CCArrayExt<CCMenuItemToggler*>(m_gamemodeBar->getChildren())) {
        if (btn == sender) {
            btn->setEnabled(false);
            btn->toggle(true);
            continue;
        }
        btn->setEnabled(true);
        btn->toggle(false);
    }
    m_activeTab = static_cast<IconType>(sender->getTag());

    auto& currentConfig = IconConfigLayer::getCurrentConfig(m_activeTab);
    m_randomBtn->toggle(currentConfig.random);
    m_randomBtn->setUserObject(CIVariableRef<bool>::create(currentConfig.random));
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

void IconConfigLayer::onClose(CCObject* sender) {
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
    Popup::onClose(sender);
}