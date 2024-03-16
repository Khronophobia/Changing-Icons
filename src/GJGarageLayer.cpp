#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include "layer/IconConfigLayer.hpp"

using namespace geode::prelude;
using namespace changing_icons;

class $modify(ModGarageLayer, GJGarageLayer) {
    bool init() {
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
            menu_selector(ModGarageLayer::onIconConfig)
        );

        menu->addChild(configBtn);
        menu->updateLayout();

        return true;
    }
    void onIconConfig(CCObject*) {
        IconConfigLayer::create()->show();
    }
};