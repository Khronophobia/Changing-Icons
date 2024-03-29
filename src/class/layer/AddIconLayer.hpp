#pragma once
#include <Geode/ui/Popup.hpp>
#include <properties/IconProperties.hpp>

namespace changing_icons {
    class IconConfigLayer;

    class AddIconLayer : public geode::Popup<IconType, IconConfigLayer*, IconProperties, std::optional<int>> {
    protected:
        IconConfigLayer* m_configLayer;
        IconProperties m_selectedIcon;
        std::optional<int> m_index;
        int m_currentPage;
        geode::Ref<cocos2d::CCArray> m_iconPageNodes = cocos2d::CCArray::create();
        geode::Ref<cocos2d::CCArray> m_colorPageNodes = cocos2d::CCArray::create();
        IconType m_iconType;
        cocos2d::CCMenu* m_iconList;
        int m_currentIconPage;
        int m_iconPageMax;
        SimplePlayer* m_iconDisplay;
        ColorChannelSprite* m_color1Display;
        ColorChannelSprite* m_color2Display;
        cocos2d::CCLabelBMFont* m_color1Label;
        cocos2d::CCLabelBMFont* m_color2Label;
        CCMenuItemSpriteExtra* m_color1Btn;
        CCMenuItemSpriteExtra* m_color2Btn;
        int m_selectedColorType;
        cocos2d::CCSprite* m_selectedColorSpr;

        bool setup(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps, std::optional<int> index) override;
        void setupIcons(int page);
        UnlockType convertIconType(IconType type) const;
        void setIconColor(std::optional<int> color, int colorType);
        void updateIconColors();
    public:
        static AddIconLayer* create(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps, std::optional<int> index = std::nullopt);
        static AddIconLayer* create(
            IconType iconType,
            IconConfigLayer* configLayer,
            int ID = 1,
            std::optional<int> color1 = std::nullopt,
            std::optional<int> color2 = std::nullopt
        );
        void onPage(CCObject* sender);
        void onIconPage(CCObject* sender);
        void onSelectIcon(CCObject* sender);
        void onAddIcon(CCObject* sender);
        void onColorType(CCObject* sender);
        void onSelectColor(CCObject* sender);
        void onClearColor(CCObject* sender);
    };
}