#include "IconListView.hpp"
#include "IconCell.hpp"
#include <CIConstants.hpp>

using namespace geode::prelude;

namespace ci {

IconListView* IconListView::create(IconList& iconList, CITab iconType, float height, bool readOnly, bool scrollBar) {
    auto ret = new (std::nothrow) IconListView();
    if (ret && ret->init(iconList, iconType, height, readOnly, scrollBar)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IconListView::init(IconList& iconList, CITab iconType, float height, bool readOnly, bool scrollBar) {
    if (!CCLayerColor::initWithColor({0, 0, 0, 64})) return false;
    m_iconList = &iconList;
    m_iconType = iconType;
    m_readOnly = readOnly;
    ignoreAnchorPointForPosition(false);
    setContentSize(ccp(constants::iconCellSize.x, height));

    m_scrollLayer = ScrollLayer::create(ccp(constants::iconCellSize.x, height));
    addChild(m_scrollLayer);

    if (scrollBar) {
        m_scrollBarObj = Scrollbar::create(m_scrollLayer);
        m_scrollBarObj->setPosition(constants::iconCellSize.x + 5.f, height * 0.5f);
        addChild(m_scrollBarObj);
    }

    refreshList();
    return true;
}

float IconListView::calculateCellYPos(size_t index) {
    return m_scrollLayer->m_contentLayer->getContentHeight() - (index + 1) * constants::iconCellSize.y;
}

void IconListView::addCell(size_t index) {
    auto cell = IconCell::create(this, (*m_iconList)[index], m_iconType, index, m_readOnly);
    cell->setPosition(ccp(0.f, calculateCellYPos(index)));
    m_scrollLayer->m_contentLayer->addChild(cell);
}

void IconListView::refreshList() {
    auto& content = m_scrollLayer->m_contentLayer;
    content->setContentHeight(std::max(m_scrollLayer->getContentHeight(), m_iconList->size() * constants::iconCellSize.y));
    if (content->getChildrenCount() == 0) {
        for (size_t i = 0; i < m_iconList->size(); ++i) {
            addCell(i);
        }
    } else {
        modifyCells();
    }

    m_scrollLayer->scrollToTop();
}

void IconListView::modifyCells() {
    auto& content = m_scrollLayer->m_contentLayer;
    auto childrenList = CCArrayExt<IconCell*>(content->getChildren()->shallowCopy());

    for (size_t i = 0; i < m_iconList->size(); ++i) {
        if (i < childrenList.size()) { // Children exist at this index
            auto cell = childrenList[i];
            cell->changeCell((*m_iconList)[i], m_iconType);
            cell->setPositionY(calculateCellYPos(i));
        } else { // Children doesn't exist at this index, but is on the list
            addCell(i);
        }
    }

    // The content layer has more cells than the list. Needs to be removed
    if (childrenList.size() > m_iconList->size()) {
        for (size_t i = m_iconList->size(); i < childrenList.size(); ++i) {
            childrenList[i]->removeFromParent();
        }
    }
}

void IconListView::changeView(IconList& iconList, CITab iconType, bool refresh) {
    m_iconList = &iconList;
    m_iconType = iconType;

    if (refresh) refreshList();
}

void IconListView::swapIcons(size_t index1, size_t index2, bool refresh) {
    std::iter_swap(m_iconList->begin() + index1, m_iconList->begin() + index2);

    if (refresh) refreshList();
}

void IconListView::deleteIcon(size_t index, bool refresh) {
    
}

IconListView::IconList const& IconListView::getIconList() {
    return *m_iconList;
}

void IconListView::setContentSize(CCSize const& size) {
    CCLayerColor::setContentSize(size);
    if (m_scrollBarObj) m_scrollBarObj->setPositionX(m_obContentSize.width + 5.f);
}

} // namespace ci