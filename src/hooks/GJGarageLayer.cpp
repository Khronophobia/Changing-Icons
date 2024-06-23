#include <Geode/modify/GJGarageLayer.hpp>
#include <class/CIConfigPopup.hpp>

class $modify(CIGarageLayer, GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init()) return false;

        auto configBtn = CCMenuItemSpriteExtra::create(
            geode::CircleButtonSprite::createWithSprite(
                "iconConfigBtn.png"_spr, 1.f, geode::CircleBaseColor::Gray, geode::CircleBaseSize::Small
            ),
            this,
            menu_selector(CIGarageLayer::onCIButton)
        );
        auto menu = getChildByID("shards-menu");
        menu->addChild(configBtn);
        menu->updateLayout();

        return true;
    }
    void onCIButton(cocos2d::CCObject*) {
        ci::CIConfigPopup::create();
    }
};