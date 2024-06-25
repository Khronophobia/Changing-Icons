#pragma once

namespace khronos {

// CCMenuItemToggler with text and also bool gets changed before callback
class CCMenuItemTogglerEx : public cocos2d::CCMenuItem {
public:
    static CCMenuItemTogglerEx* create(
        cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
    static CCMenuItemTogglerEx* createWithCallback(
        cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
        geode::utils::MiniFunction<void(CCMenuItemTogglerEx*)>&& callback,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
    static CCMenuItemTogglerEx* createWithStandardSprites(
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
        float scale = 1.f,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
    static CCMenuItemTogglerEx* createStandardSpritesWithCallback(
        geode::utils::MiniFunction<void(CCMenuItemTogglerEx*)>&& callback,
        float scale = 1.f,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
public:
    void activate() override;
    void selected() override;
    void unselected() override;
public:
    CCMenuItemToggler* getBtn() const noexcept;
    bool getState() const;
    bool isToggled() const;
    void setState(bool state);
    void setStateWithCallback(bool state);
    void toggle(bool val);
    void updateSprite();
    void setLabelOffset(cocos2d::CCPoint const& offset);
    void setLabel(char const* name);
protected:
    bool init(
        cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
        char const* name, cocos2d::CCPoint const& labelOffset
    );
    void updateSpriteShallow();
protected:
    CCMenuItemToggler* m_btn{};
    cocos2d::CCLabelBMFont* m_label{};
    cocos2d::CCPoint m_labelOffset;
};

} // namespace khronos