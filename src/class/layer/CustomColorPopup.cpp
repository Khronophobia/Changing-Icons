#include <Geode/Geode.hpp>
#include "CustomColorPopup.hpp"

using namespace geode::prelude;
using namespace changing_icons;

CustomColorPopup* CustomColorPopup::create(AddIconLayer* instance) {
    auto ret = new CustomColorPopup();
    if (ret && ret->initAnchored(300.f, 300.f, instance)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CustomColorPopup::setup(AddIconLayer* instance) {
    m_instance = instance;

    return true;
}