#include <Geode/Geode.hpp>
#include "TriTogglerInfoLayer.hpp"

using namespace geode::prelude;
using namespace changing_icons;

TriTogglerInfoLayer* TriTogglerInfoLayer::create() {
    auto ret = new TriTogglerInfoLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TriTogglerInfoLayer::init() {
    if (!FLAlertLayer::init(105)) return false;
    auto winSize = CCDirector::get()->getWinSize();

    auto bgSprite = CCScale9Sprite::create("square01_001.png");
    m_mainLayer->addChild(bgSprite);

    m_buttonMenu = CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu);

    auto constexpr layerWidth = 350.f;
    m_mainLayer->ignoreAnchorPointForPosition(false);
    m_mainLayer->setPosition(winSize / 2);
    m_mainLayer->setContentSize(ccp(layerWidth, 240.f));
    m_mainLayer->setLayout(
        CopySizeLayout::create()
            ->add(m_buttonMenu)
            ->add(bgSprite)
    );

    auto title = CCLabelBMFont::create("Info", "goldFont.fnt");
    title->setScale(0.9f);
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0.f, -25.f));

    auto okBtnSpr = ButtonSprite::create("OK");
    auto okBtn = CCMenuItemSpriteExtra::create(
        okBtnSpr, this, menu_selector(TriTogglerInfoLayer::onBtn1)
    );
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0.f, 28.f));

    auto topText = TextArea::create(
        "Settings inside this menu will <cy>override</c> the gamemode's settings if enabled.\n"
        "The <cj>override checkboxes</c> have three states:",
        "chatFont.fnt", 1.f, layerWidth - 50.f, ccp(0.5f, 0.f), 20.f, false
    );
    topText->setAnchorPoint(ccp(0.5f, 0.f));
    m_mainLayer->addChildAtPosition(topText, Anchor::Center, ccp(0.f, 15.f));

    auto constexpr checkXPos = 95.f;
    auto constexpr checkYPos = 0.f;
    auto constexpr checkYOffset = 24.f;
    auto constexpr textXPos = checkXPos + 15.f;

    auto disabledCheckSpr = CCSprite::create("CI_checkDisabled.png"_spr);
    disabledCheckSpr->setScale(0.6f);
    m_mainLayer->addChildAtPosition(disabledCheckSpr, Anchor::Left, ccp(checkXPos, checkYPos));

    auto offCheckSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    offCheckSpr->setScale(0.6f);
    m_mainLayer->addChildAtPosition(offCheckSpr, Anchor::Left, ccp(checkXPos, checkYPos - checkYOffset));

    auto onCheckSpr = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    onCheckSpr->setScale(0.6f);
    m_mainLayer->addChildAtPosition(onCheckSpr, Anchor::Left, ccp(checkXPos, checkYPos - checkYOffset * 2));

    auto checkExplanation = TextArea::create(
        "Won't override setting\n"
        "Will override setting to <cy>off</c>\n"
        "Will override setting to <cy>on</c>",
        "chatFont.fnt", 1.f, 300.f, ccp(0.f, 0.5f), checkYOffset, false
    );
    checkExplanation->setAnchorPoint(ccp(0.f, 1.f));
    m_mainLayer->addChildAtPosition(checkExplanation, Anchor::Left, ccp(textXPos, checkYPos));

    return true;
}

void TriTogglerInfoLayer::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}