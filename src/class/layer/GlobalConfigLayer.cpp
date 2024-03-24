#include "GlobalConfigLayer.hpp"
#include <class/CIConfigManager.hpp>

using namespace geode::prelude;
using namespace changing_icons;

GlobalConfigLayer* GlobalConfigLayer::create() {
    auto ret = new GlobalConfigLayer();
    if (ret && ret->initAnchored(400.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GlobalConfigLayer::setup() {
    m_configManager = CIConfigManager::get();
    m_noElasticity = true;
    this->setTitle("Global Config");

    return true;
}