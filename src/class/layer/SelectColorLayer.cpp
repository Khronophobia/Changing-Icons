#include "SelectColorLayer.hpp"
#include "AddIconLayer.hpp"

using namespace geode::prelude;
using namespace changing_icons;

SelectColorLayer* SelectColorLayer::create(AddIconLayer* addLayer, int colorType, IconProperties icon, IconType type) {
    auto ret = new SelectColorLayer();
    if (ret && ret->initAnchored(450.f, 280.f, addLayer, colorType, icon, type, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SelectColorLayer::setup(AddIconLayer* addLayer, int colorType, IconProperties icon, IconType type) {
    m_noElasticity = true;
    m_addLayer = addLayer;
    m_icon = icon;

    m_iconDisplay = SimplePlayer::create(0);
    m_iconDisplay->updatePlayerFrame(icon.iconID, type);
    m_iconDisplay->setScale(1.1f);
    m_mainLayer->addChildAtPosition(m_iconDisplay, Anchor::Top, ccp(0.f, -25.f));

    m_color1Display = ColorChannelSprite::create();
    m_color1Display->setScale(0.7f);
    m_color1Label = CCLabelBMFont::create("1X", "bigFont.fnt");
    m_color1Label->setColor(cc3x(0x0));
    m_color1Label->setOpacity(127);
    m_color1Label->setScale(0.6f);
    m_color1Label->setPosition(m_color1Display->getContentSize() / 2);
    m_color1Display->addChild(m_color1Label);

    auto color1Btn = CCMenuItemSpriteExtra::create(
        m_color1Display,
        this,
        menu_selector(SelectColorLayer::onColorTab)
    );
    color1Btn->setTag(0);
    m_buttonMenu->addChildAtPosition(color1Btn, Anchor::Top, ccp(-25.f, -55.f));

    m_color2Display = ColorChannelSprite::create();
    m_color2Display->setScale(0.7f);
    m_color2Label = CCLabelBMFont::create("2X", "bigFont.fnt");
    m_color2Label->setColor(cc3x(0x0));
    m_color2Label->setOpacity(127);
    m_color2Label->setScale(0.6f);
    m_color2Label->setPosition(m_color2Display->getContentSize() / 2);
    m_color2Display->addChild(m_color2Label);

    auto color2Btn = CCMenuItemSpriteExtra::create(
        m_color2Display,
        this,
        menu_selector(SelectColorLayer::onColorTab)
    );
    color2Btn->setTag(1);
    m_buttonMenu->addChildAtPosition(color2Btn, Anchor::Top, ccp(25.f, -55.f));

    if (icon.color1) {
        m_iconDisplay->setColor(GameManager::get()->colorForIdx(icon.color1.value()));
        m_color1Display->setColor(GameManager::get()->colorForIdx(icon.color1.value()));
        m_color1Label->setString("1");
    }
    else {
        m_iconDisplay->setColor(GameManager::get()->colorForIdx(17));
        m_color1Display->setColor(GameManager::get()->colorForIdx(17));
    }

    if (icon.color2) {
        m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(icon.color2.value()));
        m_color2Display->setColor(GameManager::get()->colorForIdx(icon.color2.value()));
        m_color2Label->setString("2");
    }
    else {
        m_iconDisplay->setSecondColor(GameManager::get()->colorForIdx(12));
        m_color2Display->setColor(GameManager::get()->colorForIdx(12));
    }

    m_colorSelector = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    m_colorSelector->setScale(0.8f);
    m_mainLayer->addChild(m_colorSelector);

    SelectColorLayer::setColorType(m_colorType);

    return true;
}

void SelectColorLayer::setColorType(int colorType) {
    m_colorType = colorType;
    switch (colorType) {
    case 0:
        m_colorSelector->setPosition(m_color1Display->getParent()->getPosition());
        break;
    case 1:
        m_colorSelector->setPosition(m_color2Display->getParent()->getPosition());
        break;
    }
}

void SelectColorLayer::onColorTab(CCObject* sender) {
    SelectColorLayer::setColorType(sender->getTag());
}