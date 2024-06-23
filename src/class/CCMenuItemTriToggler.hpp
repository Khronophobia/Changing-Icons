#pragma once
#include "tribool.hpp"

namespace khronos {

/*
    A CCMenuItemToggler that has three possible values: true, false, and null
*/
class CCMenuItemTriToggler : public cocos2d::CCMenuItem {
public:
    static CCMenuItemTriToggler* create(cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    static CCMenuItemTriToggler* create(cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback);
    static CCMenuItemTriToggler* createWithStandardSprites(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale = 1.f);
    static CCMenuItemTriToggler* createWithStandardSprites(geode::utils::MiniFunction<void(CCMenuItemTriToggler*)>&& callback, float scale = 1.f);

    khronos::tribool getState() const;
    void setState(khronos::tribool state);
    khronos::tribool isToggled() const;
    void toggle(khronos::tribool state);
    void updateSprite();

    void activate() override;
    void selected() override;
    void unselected() override;
protected:
    bool init(cocos2d::CCNode* disabledSpr, cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
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
};

} // namespace ci