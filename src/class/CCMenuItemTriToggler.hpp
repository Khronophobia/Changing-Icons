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
        cocos2d::CCLabelBMFont* m_label;
        std::optional<bool> m_state;
    public:
        static CCMenuItemTriToggler* create(
            CCNode* disabledNode, CCNode* offNode, CCNode* onNode, CCObject* target, cocos2d::SEL_MenuHandler callback, float scale = 1.f
        );
        static CCMenuItemTriToggler* createWithLabel(
            CCNode* disabledNode, CCNode* offNode, CCNode* onNode, CCObject* target, cocos2d::SEL_MenuHandler callback, char const* text, float scale = 1.f
        );
        bool init(CCNode* disabled, CCNode* off, CCNode* on, CCObject* target, cocos2d::SEL_MenuHandler callback, char const* text, float scale);
        void setState(std::optional<bool> state);
        std::optional<bool> getState();
        // Alias for setState
        void toggle(std::optional<bool> value);
        // Alias for getState
        std::optional<bool> isToggled();

        void activate() override;
        void selected() override;
        void unselected() override;
    };
}