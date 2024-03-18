#pragma once

namespace changing_icons {
    class IconCell : public cocos2d::CCLayerColor {
    protected:
        int m_index;
        bool init(
            int index,
            IconType iconType,
            int iconID,
            std::optional<cocos2d::ccColor3B> color1,
            std::optional<cocos2d::ccColor3B> color2,
            bool isLast
        );
    public:
        static IconCell* create(
            int index,
            IconType iconType,
            int iconID,
            std::optional<cocos2d::ccColor3B> color1 = std::nullopt,
            std::optional<cocos2d::ccColor3B> color2 = std::nullopt,
            bool isLast = false
        );
        void onDelete(CCObject*);
        void onMoveDown(CCObject*);
        void onMoveUp(CCObject*);
    };
}