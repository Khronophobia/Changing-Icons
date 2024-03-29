#pragma once

namespace changing_icons::utils {
    template<class T>
    T tryGetJsonValue(matjson::Value const& value, std::string_view key, T defaultVar) {
        return value.contains(key) ? value[key].as<T>() : defaultVar;
    }
    /*
        Create a toggle button inside a menu with AnchorLayout.
        Meant to be used inside a Popup class.
    */
    CCMenuItemToggler* createToggleButton(
        cocos2d::CCMenu* menu, cocos2d::CCLayer* mainLayer, cocos2d::Anchor anchor, cocos2d::CCPoint offset, char const* label,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale
    );
    /*
        Create a toggle button inside a menu.
        Meant to be used inside a Popup class.
    */
    CCMenuItemToggler* createToggleButton(
        cocos2d::CCMenu* menu, cocos2d::CCLayer* mainLayer, cocos2d::CCPoint position, char const* label,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale
    );
    /*
        Create an info button relative to a toggle button.
        Meant to be used alongside createToggleButton().
    */
    CCMenuItemSpriteExtra* createToggleInfo(
        cocos2d::CCMenu* menu, CCMenuItemToggler* btn, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, gd::string const& infoDesc = ""
    );
}

template<>
struct fmt::formatter<IconType> : formatter<string_view> {
    fmt::appender format(IconType type, format_context& ctx) const;
};