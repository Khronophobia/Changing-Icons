#pragma once

namespace changing_icons {
    class IconCell : public cocos2d::CCLayerColor {
    protected:
        int m_index;
        bool init(
            int index,
            IconType iconType,
            int iconID,
            std::optional<int> color1,
            std::optional<int> color2,
            bool isLast
        );
    public:
        static IconCell* create(
            int index,
            IconType iconType,
            int iconID,
            std::optional<int> color1 = std::nullopt,
            std::optional<int> color2 = std::nullopt,
            bool isLast = false
        );
        void onDelete(CCObject*);
        void onMoveDown(CCObject*);
        void onMoveUp(CCObject*);
    };
}