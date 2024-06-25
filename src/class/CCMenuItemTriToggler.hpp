#pragma once
#include "tribool.hpp"

namespace khronos {

/*
    A CCMenuItemToggler that has three possible values: true, false, and null
*/
class CCMenuItemTriToggler : public cocos2d::CCMenuItem {
public:
    static CCMenuItemTriToggler* create(
        cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
    static CCMenuItemTriToggler* createWithCallback(
        cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
        geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
    static CCMenuItemTriToggler* createWithStandardSprites(
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale = 1.f,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
    static CCMenuItemTriToggler* createStandardSpritesWithCallback(
        geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback, float scale = 1.f,
        char const* name = nullptr, cocos2d::CCPoint const& labelOffset = {20.f, 0.f}
    );
public:
    khronos::tribool getState() const;
    void setState(khronos::tribool state);
    void setStateWithCallback(khronos::tribool state);
    khronos::tribool isToggled() const;
    void toggle(khronos::tribool state);
    void updateSprite();
    void setLabelOffset(cocos2d::CCPoint const& offset);
    void setLabel(char const* name);
public:
    void activate() override;
    void selected() override;
    void unselected() override;
protected:
    bool init(
        cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback,
        char const* name, cocos2d::CCPoint const& labelOffset
    );
    void updateContentSize();
    void triggerTrue(cocos2d::CCObject*);
    void triggerFalse(cocos2d::CCObject*);
    void triggerNull(cocos2d::CCObject*);
protected:
    khronos::tribool m_state{nullptr};
    CCMenuItemSpriteExtra* m_disabledBtn{};
    CCMenuItemSpriteExtra* m_offBtn{};
    CCMenuItemSpriteExtra* m_onBtn{};
    CCMenuItemSpriteExtra* m_activeBtn{};
    cocos2d::CCLabelBMFont* m_label{};
    cocos2d::CCPoint m_labelOffset;
};

} // namespace ci