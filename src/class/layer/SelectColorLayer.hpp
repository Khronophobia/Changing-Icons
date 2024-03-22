#pragma once
#include <Geode/ui/Popup.hpp>
#include "../../CIConfigProperties.hpp"

namespace changing_icons {
    class AddIconLayer;

    class SelectColorLayer : public geode::Popup<AddIconLayer*, int, IconProperties, IconType> {
    protected:
        AddIconLayer* m_addLayer;
        int m_colorType;
        IconProperties m_icon;
        SimplePlayer* m_iconDisplay;
        ColorChannelSprite* m_color1Display;
        ColorChannelSprite* m_color2Display;
        cocos2d::CCLabelBMFont* m_color1Label;
        cocos2d::CCLabelBMFont* m_color2Label;
        cocos2d::CCSprite* m_colorSelector;

        bool setup(AddIconLayer* addLayer, int colorType, IconProperties icon, IconType type);
        void setColorType(int colorType);
    public:
        static SelectColorLayer* create(
            AddIconLayer* addLayer, int colorType, IconProperties icon, IconType type
        );
        void onColorTab(CCObject* sender);
    };
}