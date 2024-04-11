#pragma once
#include <Geode/ui/TextInput.hpp>
#include <properties/IconProperties.hpp>

namespace changing_icons {
    using IconColor = std::variant<int, cocos2d::ccColor3B>;
    class IconConfigLayer;

    class AddIconLayer : public geode::Popup<IconType, IconConfigLayer*, IconProperties, std::optional<int>>, cocos2d::extension::ColorPickerDelegate, TextInputDelegate {
    private:
        enum ColorType {
            Col1 = 0,
            Col2 = 1,
            Glow = 2
        };
        enum Color {
            Red,
            Green,
            Blue
        };
    protected:
        IconConfigLayer* m_configLayer;
        IconProperties m_selectedIcon;
        std::optional<int> m_index;
        int m_currentPage;
        cocos2d::CCLayer* m_iconPageLayer;
        cocos2d::CCMenu* m_iconPageMenu;
        CCMenuItemSpriteExtra* m_iconPageBtn;
        cocos2d::CCLayer* m_colorPageLayer;
        cocos2d::CCMenu* m_colorPageMenu;
        CCMenuItemSpriteExtra* m_colorPageBtn;
        IconType m_iconType;
        cocos2d::CCMenu* m_iconList;
        cocos2d::CCMenu* m_iconListPageMenu;
        geode::Ref<cocos2d::CCSprite> m_iconCursorSpr;
        int m_currentIconPage;
        int m_iconPageMax;
        SimplePlayer* m_iconDisplay;
        ColorChannelSprite* m_color1Display;
        ColorChannelSprite* m_color2Display;
        ColorChannelSprite* m_glowColorDisplay;
        cocos2d::CCLabelBMFont* m_color1Label;
        cocos2d::CCSprite* m_color1DisabledSpr;
        cocos2d::CCLabelBMFont* m_color2Label;
        cocos2d::CCSprite* m_color2DisabledSpr;
        cocos2d::CCLabelBMFont* m_glowColorLabel;
        cocos2d::CCSprite* m_glowColorDisabledSpr;
        CCMenuItemSpriteExtra* m_color1Btn;
        CCMenuItemSpriteExtra* m_color2Btn;
        CCMenuItemSpriteExtra* m_glowColorBtn;
        CCMenuItemToggler* m_overrideGlowBtn;
        ColorChannelSprite* m_customColorSpr;
        CCMenuItemSpriteExtra* m_customColorBtn;
        std::array<cocos2d::ccColor3B, 3> m_tempCustomColors{geode::cocos::cc3x(0xff), geode::cocos::cc3x(0xff), geode::cocos::cc3x(0xff)};
        int m_selectedColorType;
        geode::Ref<cocos2d::CCSprite> m_colorTypeCursorSpr;
        geode::Ref<cocos2d::CCSprite> m_color1CursorSpr;
        geode::Ref<cocos2d::CCSprite> m_color2CursorSpr;
        geode::Ref<cocos2d::CCSprite> m_glowColorCursorSpr;
        cocos2d::CCMenu* m_colorMenu;
        geode::Ref<cocos2d::CCLayer> m_customColorLayer;
        cocos2d::extension::CCControlColourPicker* m_colorWheel;
        geode::TextInput* m_redInput;
        geode::TextInput* m_greenInput;
        geode::TextInput* m_blueInput;
        geode::TextInput* m_hexInput;

        bool setup(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps, std::optional<int> index) override;
        void setupIconPage(int page);
        void setIconColor(std::optional<std::variant<int, cocos2d::ccColor3B>> color, int colorType);
        void updateIconColor(int colorType);
        void updateIconColors();
        void updateIconCursor();
        void toggleColorWheel(bool toggle);
        std::optional<IconColor>& getSelectedColor();
    public:
        static AddIconLayer* create(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps, std::optional<int> index = std::nullopt);
        static AddIconLayer* create(
            IconType iconType,
            IconConfigLayer* configLayer,
            int ID = 1,
            std::optional<int> color1 = std::nullopt,
            std::optional<int> color2 = std::nullopt
        );

        void setWheelColor(cocos2d::ccColor3B const& color);
        void colorValueChanged(cocos2d::ccColor3B color) override;

        void textChanged(CCTextInputNode* input) override;

        void onPage(CCObject* sender);
        void onIconPageArrow(CCObject* sender);
        void onIconPage(CCObject* sender);
        void onSelectIcon(CCObject* sender);
        void onAddIcon(CCObject* sender);
        void onColorType(CCObject* sender);
        void onSelectColor(CCObject* sender);
        void onSelectCustomColor(CCObject* sender);
        void onClearColor(CCObject* sender);
        void onToggleGlow(CCObject* sender);
    };
}