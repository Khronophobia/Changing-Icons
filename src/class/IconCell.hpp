#pragma once

namespace changing_icons {
    class IconConfigLayer;

    class IconCell : public cocos2d::CCLayerColor {
    protected:
        int m_index;
        IconConfigLayer* m_configLayer;

        bool init(
            IconConfigLayer* configLayer,
            int index,
            IconType iconType,
            int iconID,
            std::optional<int> color1,
            std::optional<int> color2,
            bool isLast
        );
    public:
        static IconCell* create(
            IconConfigLayer* m_configLayer,
            int index,
            IconType iconType,
            int iconID,
            std::optional<int> color1 = std::nullopt,
            std::optional<int> color2 = std::nullopt,
            bool isLast = false
        );
        void onDelete(CCObject*);
        void onEdit(CCObject*);
        void onMoveDown(CCObject*);
        void onMoveUp(CCObject*);
    };
}