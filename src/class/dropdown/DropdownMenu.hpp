#pragma once

namespace changing_icons {
    class DropdownChoice;

    class DropdownMenu : public cocos2d::CCClippingNode {
    protected:
        cocos2d::CCObject* m_dropdownTarget;
        cocos2d::SEL_MenuHandler m_dropdownCallback;
        std::vector<std::string> m_choiceList;
        cocos2d::CCLayerColor* m_stencil;
        cocos2d::CCMenu* m_choiceMenu;
        DropdownChoice* m_dropdownSelector;
        bool m_isExpanded;
        int m_choice;

        bool init(std::vector<std::string> const& choices, float width, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler dropdownCallback, int selectedChoice);
    public:
        static DropdownMenu* create(std::vector<std::string> const& choices, float width, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler dropdownCallback, int selectedChoice = 0);
        void setChoice(int choice);
        int getChoice() const;

        void onDropdown(CCObject*);
        void onDropdownChoice(CCObject*);
    };
}