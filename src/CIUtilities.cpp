#include "CIUtilities.hpp"
#include <CIConfigProperties.hpp>

using namespace geode::prelude;
using namespace changing_icons;

CCMenuItemToggler* changing_icons::utils::createToggleButton(
    CCMenu* menu, CCLayer* layer, Anchor anchor, CCPoint offset, char const* label,
    CCObject* target, cocos2d::SEL_MenuHandler callback, float scale
) {
    auto btn = CCMenuItemToggler::createWithStandardSprites(target, callback, scale);
    menu->addChildAtPosition(btn, anchor, offset);

    auto labelText = CCLabelBMFont::create(label, "bigFont.fnt");
    labelText->setAnchorPoint(ccp(0.f, 0.5f));
    labelText->limitLabelWidth(150.f, 0.85f * scale, 0.1f);
    layer->addChildAtPosition(labelText, anchor, offset + ccp(24.f, 0.f) * scale);

    return btn;
}

CCMenuItemToggler* changing_icons::utils::createToggleButton(
    CCMenu* menu, CCLayer* layer, CCPoint position, char const* label,
    CCObject* target, cocos2d::SEL_MenuHandler callback, float scale
) {
    auto btn = CCMenuItemToggler::createWithStandardSprites(target, callback, scale);
    btn->setPosition(position);
    menu->addChild(btn);

    auto labelText = CCLabelBMFont::create(label, "bigFont.fnt");
    labelText->setAnchorPoint(ccp(0.f, 0.5f));
    labelText->limitLabelWidth(150.f, 0.85f * scale, 0.1f);
    labelText->setPosition(position + ccp(24.f, 0.f) * scale);
    layer->addChild(labelText);

    return btn;
}

CCMenuItemSpriteExtra* changing_icons::utils::createToggleInfo(
    CCMenu* menu, CCMenuItemToggler* btn, CCObject* target, SEL_MenuHandler callback, gd::string const& infoDesc
) {
    bool isAnchorLayout = false;
    Anchor anchor;
    CCPoint offset;
    if (auto layoutOpts = static_cast<AnchorLayoutOptions*>(btn->getLayoutOptions())) {
        isAnchorLayout = true;
        anchor = layoutOpts->getAnchor();
        offset = layoutOpts->getOffset();
    }
    auto scale = btn->m_offButton->getNormalImage()->getScale();
    auto constexpr posOffset = 20.f;

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.7f * scale);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr, target, callback
    );
    if (!infoDesc.empty())
        infoBtn->setUserObject(CCString::create(infoDesc));

    if (isAnchorLayout) {
        menu->addChildAtPosition(infoBtn, anchor, offset + ccp(-posOffset, posOffset) * scale);
    } else {
        infoBtn->setPosition(btn->getPosition() + ccp(-posOffset, posOffset) * scale);
        menu->addChild(infoBtn);
    }

    return infoBtn;
}

fmt::appender fmt::formatter<IconType>::format(IconType type, format_context& ctx) const {
    switch(type) {
        default: return formatter<string_view>::format("Unknown", ctx);
        case IconType::Cube: return formatter<string_view>::format("Cube", ctx);
        case IconType::Ship: return formatter<string_view>::format("Ship", ctx);
        case IconType::Ball: return formatter<string_view>::format("Ball", ctx);
        case IconType::Ufo: return formatter<string_view>::format("UFO", ctx);
        case IconType::Wave: return formatter<string_view>::format("Wave", ctx);
        case IconType::Robot: return formatter<string_view>::format("Robot", ctx);
        case IconType::Spider: return formatter<string_view>::format("Spider", ctx);
        case IconType::Swing: return formatter<string_view>::format("Swing", ctx);
        case IconType::Jetpack: return formatter<string_view>::format("Jetpack", ctx);
    }
}

fmt::appender fmt::formatter<IconOrder>::format(IconOrder order, format_context& ctx) const {
    switch(order) {
        case IconOrder::Random: return formatter<string_view>::format("Random", ctx);
        case IconOrder::Forward: return formatter<string_view>::format("Forward", ctx);
        case IconOrder::Backward: return formatter<string_view>::format("Backward", ctx);
    }
}