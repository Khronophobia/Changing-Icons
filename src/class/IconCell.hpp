#pragma once

namespace changing_icons {
    class IconCell : public cocos2d::CCLayerColor {
    protected:
        bool init(
            int index,
            IconType iconType,
            int iconID,
            std::optional<cocos2d::ccColor3B> color1,
            std::optional<cocos2d::ccColor3B> color2
        );
    public:
        static IconCell* create(
            int index,
            IconType iconType,
            int iconID,
            std::optional<cocos2d::ccColor3B> color1 = std::nullopt,
            std::optional<cocos2d::ccColor3B> color2 = std::nullopt
        );
    };
}