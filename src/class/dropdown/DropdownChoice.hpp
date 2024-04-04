#pragma once

namespace changing_icons {
    class DropdownChoice : public cocos2d::CCMenuItem {
    protected:
        bool m_isSelecting;
        bool m_selected;

        bool init(char const* str, float width, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, bool addArrow, bool selected);
    public:
        cocos2d::CCLayerColor* m_background;
        cocos2d::CCLabelBMFont* m_label;
        cocos2d::CCSprite* m_arrowSpr;

        static DropdownChoice* create(char const* str, float width, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, bool addArrow, bool selected = false);
        void setSelected(bool selected);
        void setLabelText(char const* str);
        void selected() override;
        void unselected() override;
        void activate() override;
    };
}