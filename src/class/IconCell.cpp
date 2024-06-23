#include "IconCell.hpp"
#include "IconListView.hpp"
#include <CIConstants.hpp>
#include <CIUtilities.hpp>

using namespace geode::prelude;

namespace ci {

IconCell* IconCell::create(IconListView* list, IconSettings& settings, CITab type, size_t index, bool readOnly) {
    auto ret = new (std::nothrow) IconCell();
    if (ret && ret->init(list, settings, type, index, readOnly)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IconCell::init(IconListView* list, IconSettings& settings, CITab type, size_t index, bool readOnly) {
    if (!CCLayer::init()) return false;
    m_list = list;
    m_settings = &settings;
    m_index = index;
    m_type = type;
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0.f, 0.f));
    setContentSize({constants::iconCellSize.x, constants::iconCellSize.y});

    auto background = CCScale9Sprite::create("cellBackground.png"_spr);
    addChild(background);
    setLayout(CopySizeLayout::create()->add(background));

    m_iconDisplay = SimplePlayer::create(0);
    m_iconDisplay->setScale(0.75f);
    m_iconDisplay->setPosition({20.f, 15.f});
    addChild(m_iconDisplay);

    m_playerIconIndicator = CCLabelBMFont::create("P", "bigFont.fnt");
    m_playerIconIndicator->setScale(0.4f);
    m_playerIconIndicator->setPosition(30.f, 8.f);
    addChild(m_playerIconIndicator);

    m_primaryColorDisplay = ColorChannelSprite::create();
    m_primaryColorDisplay->setScale(0.35f);
    m_primaryColorDisplay->setPosition({48.f, 22.f});
    addChild(m_primaryColorDisplay);
    m_secondaryColorDisplay = ColorChannelSprite::create();
    m_secondaryColorDisplay->setScale(0.35f);
    m_secondaryColorDisplay->setPosition({48.f, 8.f});
    addChild(m_secondaryColorDisplay);

    m_glowColorDisplay = ColorChannelSprite::create();
    m_glowColorDisplay->setScale(0.35f);
    m_glowColorDisplay->setPosition({62.f, 15.f});
    addChild(m_glowColorDisplay);

    m_glowColorIgnoreSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    m_glowColorIgnoreSpr->setPosition(m_glowColorDisplay->getContentSize() / 2.f);
    m_glowColorDisplay->addChild(m_glowColorIgnoreSpr);

    m_editMenu = CCMenu::create();
    m_editMenu->setContentSize(ccp(8.f, 8.f));
    m_editMenu->setVisible(!readOnly);
    m_editMenu->setPosition(ccp(m_obContentSize.width - 10.f, m_obContentSize.height * 0.5f));
    addChild(m_editMenu);

    auto deleteBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png"),
        this, nullptr
    );
    m_editMenu->addChild(deleteBtn);

    auto editSpr = CCSprite::create("editIcon.png"_spr);
    editSpr->setScale(0.5f);
    auto editBtn = CCMenuItemSpriteExtra::create(
        editSpr,
        this, nullptr
    );
    editBtn->setPositionX(-15.f);
    m_editMenu->addChild(editBtn);

    auto moveDownSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    moveDownSpr->setScale(0.6f);
    m_moveDownBtn = CCMenuItemSpriteExtra::create(
        moveDownSpr,
        this, menu_selector(IconCell::onMoveDown)
    );
    m_moveDownBtn->setPosition(-30.f, -6.f);
    m_moveDownBtn->setVisible(index < list->getIconList().size() - 1);
    m_editMenu->addChild(m_moveDownBtn);

    auto moveUpSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    moveUpSpr->setScale(0.6f);
    m_moveUpBtn = CCMenuItemSpriteExtra::create(
        moveUpSpr,
        this, menu_selector(IconCell::onMoveUp)
    );
    m_moveUpBtn->setPosition(-30.f, 6.f);
    m_moveUpBtn->setVisible(index > 0);
    m_editMenu->addChild(m_moveUpBtn);

    updateCell();
    return true;
}

void IconCell::updateCell() {
    auto gm = GameManager::get();

    if (m_settings->isPlayerIcon) {
        m_iconDisplay->updatePlayerFrame(utils::getPlayerIcon(m_type), utils::toIconType(m_type));
        m_playerIconIndicator->setVisible(true);
    } else {
        m_iconDisplay->updatePlayerFrame(m_settings->iconID, utils::toIconType(m_type));
        m_playerIconIndicator->setVisible(false);
    }

    if (m_settings->primaryColor) {
        m_iconDisplay->setColor(utils::getPlayerColor(m_settings->primaryColor.value()));
        m_primaryColorDisplay->setVisible(true);
        m_primaryColorDisplay->setColor(utils::getPlayerColor(m_settings->primaryColor.value()));
    } else {
        m_iconDisplay->setColor(gm->colorForIdx(gm->getPlayerColor()));
        m_primaryColorDisplay->setVisible(false);
    }

    if (m_settings->secondaryColor) {
        m_iconDisplay->setSecondColor(utils::getPlayerColor(m_settings->secondaryColor.value()));
        m_secondaryColorDisplay->setVisible(true);
        m_secondaryColorDisplay->setColor(utils::getPlayerColor(m_settings->secondaryColor.value()));
    } else {
        m_iconDisplay->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
        m_secondaryColorDisplay->setVisible(false);
    }

    if (m_settings->overrideGlow) {
        m_glowColorDisplay->setVisible(true);
        if (m_settings->glowColor) {
            m_iconDisplay->setGlowOutline(utils::getPlayerColor(m_settings->glowColor.value()));
            m_glowColorDisplay->setColor(utils::getPlayerColor(m_settings->glowColor.value()));
            m_glowColorIgnoreSpr->setVisible(false);
        } else {
            m_iconDisplay->disableGlowOutline();
            m_glowColorDisplay->setColor(gm->colorForIdx(12));
            m_glowColorIgnoreSpr->setVisible(true);
        }
    } else {
        m_glowColorDisplay->setVisible(false);
        if (gm->getPlayerGlow()) {
            m_iconDisplay->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
        } else {
            m_iconDisplay->disableGlowOutline();
        }
    }

    m_moveUpBtn->setVisible(m_index > 0);
    m_moveDownBtn->setVisible(m_index < m_list->getIconList().size() - 1);
}

void IconCell::changeCell(IconSettings& settings, bool update) {
    m_settings = &settings;
    if (update) updateCell();
}
void IconCell::changeCell(IconSettings& settings, CITab type, bool update) {
    m_type = type;
    changeCell(settings, update);
}

void IconCell::onMoveUp(CCObject*) {
    m_list->swapIcons(m_index, m_index - 1);
}

void IconCell::onMoveDown(CCObject*) {
    m_list->swapIcons(m_index, m_index + 1);
}

} // namespace ci