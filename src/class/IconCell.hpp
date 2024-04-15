#pragma once
#include <properties/IconProperties.hpp>
#include "IconCellDelegate.hpp"

namespace changing_icons {
    class IconConfigLayer;

    class IconCell : public cocos2d::CCLayerColor {
    protected:
        int m_index;
        IconCellDelegate* m_delegate;

        bool init(
            IconCellDelegate* delegate,
            int index,
            IconType iconType,
            IconProperties const& icon,
            bool readOnly,
            bool isLast
        );
    public:
        static IconCell* create(
            IconCellDelegate* delegate,
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