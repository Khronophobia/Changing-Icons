#pragma once
#include <settings/IconSettings.hpp>

namespace ci {

class IconListView;

class IconCell : public cocos2d::CCLayer {
public:
    static IconCell* create(IconListView* list, IconSettings& settings, CITab type, size_t index, bool readOnly);
    void updateCell();
    void changeCell(IconSettings& settings, bool update = true);
    void changeCell(IconSettings& settings, CITab type, bool update = true);

    void onMoveUp(cocos2d::CCObject*);
    void onMoveDown(cocos2d::CCObject*);
protected:
    bool init(IconListView* list, IconSettings& settings, CITab type, size_t index, bool readOnly);
protected:
    IconListView* m_list{};
    IconSettings* m_settings{};
    CITab m_type;
    size_t m_index{};
    SimplePlayer* m_iconDisplay{};
    ColorChannelSprite* m_primaryColorDisplay{};
    ColorChannelSprite* m_secondaryColorDisplay{};
    ColorChannelSprite* m_glowColorDisplay{};
    cocos2d::CCSprite* m_glowColorIgnoreSpr{};
    cocos2d::CCLabelBMFont* m_playerIconIndicator{};
    cocos2d::CCMenu* m_editMenu{};
    CCMenuItemSpriteExtra* m_moveUpBtn{};
    CCMenuItemSpriteExtra* m_moveDownBtn{};
};

} // namespace ci