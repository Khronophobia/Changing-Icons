#pragma once
#include <settings/IconSettings.hpp>

namespace ci {

class IconListView : public cocos2d::CCLayerColor {
public:
    using IconList = std::vector<IconSettings>;
public:
    static IconListView* create(IconList& iconList, CITab iconType, float height, bool readOnly = false, bool scrollBar = true);
    void refreshList();
    void changeView(IconList& iconList, CITab iconType, bool refresh = true);
    void setContentSize(cocos2d::CCSize const& size) override;
    IconList const& getIconList();
    void swapIcons(size_t index1, size_t index2, bool refresh = true);
    void deleteIcon(size_t index, bool refresh = true);
protected:
    bool init(IconList& iconList, CITab iconType, float height, bool readOnly, bool scrollBar);
    float calculateCellYPos(size_t index);
    void addCell(size_t index);
    void modifyCells();
protected:
    IconList* m_iconList{};
    CITab m_iconType;
    bool m_readOnly{};
    geode::ScrollLayer* m_scrollLayer{};
    geode::Scrollbar* m_scrollBarObj{};
};

} // namespace ci