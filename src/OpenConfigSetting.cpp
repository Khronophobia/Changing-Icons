#include "OpenConfigSetting.hpp"
#include <class/CIConfigManager.hpp>
#include <class/layer/IconConfigLayer.hpp>

using namespace geode::prelude;
using namespace changing_icons;

OpenConfigSettingNode* OpenConfigSettingNode::create(OpenConfigSettingValue* value, float width) {
    auto ret = new OpenConfigSettingNode();
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool OpenConfigSettingNode::init(OpenConfigSettingValue* value, float width) {
    if (!SettingNode::init(value)) return false;
    this->setContentSize(ccp(width, 40.f));

    auto menu = CCMenu::create();
    menu->setPosition(m_obContentSize / 2);
    this->addChild(menu);

    if (CIManager::get()->getConfigLayer()) {
        auto text = CCLabelBMFont::create("Already At Config", "bigFont.fnt");
        text->setScale(0.5f);
        menu->addChild(text);
    } else {
        auto spr = ButtonSprite::create("Open Config", "bigFont.fnt", "GJ_button_05.png");
        spr->setScale(0.5f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(OpenConfigSettingNode::onConfig));
        menu->addChild(btn);
    }

    return true;
}

void OpenConfigSettingNode::onConfig(CCObject* sender) {
    IconConfigLayer::create(true)->show();
}