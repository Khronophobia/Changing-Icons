#pragma once

class CCColourPickerFix : public cocos2d::extension::CCControlColourPicker {
public:
    // Inlined on Android and for some reason can't redefine in Broma
    cocos2d::ccColor3B const& getColorValue() {
        return m_rgb;
    }
};