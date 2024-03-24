#pragma once

namespace changing_icons {
    class CCMenuItemTriToggler final : public cocos2d::CCMenuItem {
    private:
        CCMenuItemSpriteExtra* getActiveButton();
        void setVisibleButton();
    protected:
        CCMenuItemSpriteExtra* m_disabledButton;
        CCMenuItemSpriteExtra* m_offButton;
        CCMenuItemSpriteExtra* m_onButton;
        int m_state = -1;
    public:
        static CCMenuItemTriToggler* create(CCNode* disabled, CCNode* off, CCNode* on, CCObject* target, cocos2d::SEL_MenuHandler callback, float scale = 1.f);
        bool init(CCNode* disabled, CCNode* off, CCNode* on, CCObject* target, cocos2d::SEL_MenuHandler callback, float scale);
        void setState(int state);
        int getState();
        // setState alternative that takes std::optional<bool>
        void toggle(std::optional<bool>);
        // getState alternative that returns std::optional<bool>
        std::optional<bool> isToggled();

        void activate() override;
        void selected() override;
        void unselected() override;
    };
}