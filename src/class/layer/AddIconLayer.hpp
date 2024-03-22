#pragma once
#include <Geode/ui/Popup.hpp>
#include "../../CIConfigProperties.hpp"

namespace changing_icons {
    class IconConfigLayer;

    class AddIconLayer : public geode::Popup<IconType, IconConfigLayer*, IconProperties> {
    protected:
        IconType m_iconType;
        cocos2d::CCMenu* m_iconList;
        int m_iconPerPage = 40;
        int m_currentPage = 0;
        int m_pageMax;
        SimplePlayer* m_iconDisplay;
        ColorChannelSprite* m_color1Display;
        ColorChannelSprite* m_color2Display;
        cocos2d::CCLabelBMFont* m_color1Label;
        cocos2d::CCLabelBMFont* m_color2Label;
        IconProperties m_selectedIcon;
        IconConfigLayer* m_configLayer;

        bool setup(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps);
        void setupIcons(int page);
        UnlockType convertIconType(IconType type);
        void setIconColor(std::optional<int> color, int colorType);
        void updateIconColors();
    public:
        static AddIconLayer* create(IconType iconType, IconConfigLayer* configLayer, IconProperties iconProps);
        static AddIconLayer* create(
            IconType iconType,
            IconConfigLayer* configLayer,
            int ID = 1,
            std::optional<int> color1 = std::nullopt,
            std::optional<int> color2 = std::nullopt
        );
        void onIconPage(CCObject* sender);
        void onSelectIcon(CCObject* sender);
        void onAddIcon(CCObject* sender);
        void onColor(CCObject* sender);
    };
}