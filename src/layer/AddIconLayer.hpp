#pragma once
#include <Geode/ui/Popup.hpp>
#include "../IconConfigProperties.hpp"

namespace changing_icons {
    class AddIconLayer : public geode::Popup<
            IconType,
            int,
            std::optional<cocos2d::ccColor3B>,
            std::optional<cocos2d::ccColor3B>
        > {
    protected:
        IconType m_iconType;
        cocos2d::CCMenu* m_iconList;
        SimplePlayer* m_iconDisplay;
        ColorChannelSprite* m_color1Display;
        ColorChannelSprite* m_color2Display;
        cocos2d::CCLabelBMFont* m_color1Label;
        cocos2d::CCLabelBMFont* m_color2Label;
        IconProperties m_selectedIcon;
        bool setup(
            IconType iconType,
            int ID,
            std::optional<cocos2d::ccColor3B> color1,
            std::optional<cocos2d::ccColor3B> color2
        );
        size_t getIconCount() const;
        UnlockType convertIconType(IconType type);
        void setIconColor(std::optional<cocos2d::ccColor3B> color, int colorType);
        void updateIconColors();
    public:
        static AddIconLayer* create(
            IconType iconType,
            int ID = 1,
            std::optional<cocos2d::ccColor3B> color1 = std::nullopt,
            std::optional<cocos2d::ccColor3B> color2 = std::nullopt
        );
        void onSelectIcon(CCObject* sender);
        void onAddIcon(CCObject* sender);
        void onColor(CCObject* sender);
        void onSecondColor(CCObject* sender);
    };
}