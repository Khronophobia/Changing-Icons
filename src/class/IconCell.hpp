#pragma once
#include <properties/IconProperties.hpp>

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
            IconProperties const& icon,
            bool readOnly,
            bool isLast
        );
    public:
        static IconCell* create(
            IconConfigLayer* m_configLayer,
            int index,
            IconType iconType,
            IconProperties const& icon,
            bool isLast = false
        );
        static IconCell* create(int index, IconType iconType, IconProperties const& icon);
        void onDelete(CCObject*);
        void onEdit(CCObject*);
        void onMoveDown(CCObject*);
        void onMoveUp(CCObject*);
    };
}