#include "GJGarageLayer.hpp"
#include <class/layer/IconConfigLayer.hpp>

using namespace geode::prelude;
using namespace changing_icons;

bool CIGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;

    auto menu = this->getChildByID("shards-menu");
    auto configBtn = CCMenuItemSpriteExtra::create(
        CircleButtonSprite::createWithSprite(
            "iconConfigBtn.png"_spr,
            1.f,
            CircleBaseColor::Gray,
            CircleBaseSize::Small
        ),
        this,
        menu_selector(CIGarageLayer::onIconConfig)
    );

    menu->addChild(configBtn);
    menu->updateLayout();

    return true;
}
void CIGarageLayer::onIconConfig(CCObject*) {
    IconConfigLayer::create()->show();
}