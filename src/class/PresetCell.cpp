#include "PresetCell.hpp"
#include <CIConstants.hpp>
#include <CIUtilities.hpp>
#include <class/layer/LoadPresetLayer.hpp>
#include <properties/ConfigProperties.hpp>

using namespace geode::prelude;
using namespace changing_icons;

PresetCell* PresetCell::create(LoadPresetLayer* presetLayer, int index, IconType type, CIPreset const& preset, ghc::filesystem::path const& filename) {
    auto ret = new PresetCell();
    if (ret && ret->init(presetLayer, index, type, preset, filename)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PresetCell::init(LoadPresetLayer* presetLayer, int index, IconType type, CIPreset const& preset, ghc::filesystem::path const& filename) {
    if (!CCLayerColor::init()) return false;
    m_presetLayer = presetLayer;
    m_preset = preset;
    m_iconType = type;
    m_filename = filename;

    this->setContentSize(ccp(constants::PRESETCELL_WIDTH, constants::PRESETCELL_HEIGHT));
    this->setAnchorPoint(ccp(0.f, 0.f));
    if (index % 2 == 0)
        this->setOpacity(100);
    else
        this->setOpacity(50);

    auto title = CCLabelBMFont::create(m_preset.name.c_str(), "bigFont.fnt");
    title->setAnchorPoint(ccp(0.f, 1.f));
    title->limitLabelWidth(m_obContentSize.width - 8.f, 0.4f, 0.1f);
    this->addChildAtPosition(title, Anchor::TopLeft, ccp(4.f, 0.f));

    auto const& icon = m_preset.iconSet[0];
    auto iconDisplay = SimplePlayer::create(0);
    iconDisplay->setScale(0.7f);
    iconDisplay->updatePlayerFrame(icon.iconID, m_iconType);
    iconDisplay->setColor(
        utils::getColorFromVariant(icon.color1.value_or(GameManager::get()->getPlayerColor()))
    );
    iconDisplay->setSecondColor(
        utils::getColorFromVariant(icon.color2.value_or(GameManager::get()->getPlayerColor2()))
    );
    if (icon.overrideGlow) {
        if (icon.glowColor) {
            iconDisplay->setGlowOutline(
                utils::getColorFromVariant(icon.glowColor.value_or(GameManager::get()->getPlayerColor()))
            );
        } else {
            iconDisplay->disableGlowOutline();
        }
    } else {
        if (GameManager::get()->getPlayerGlow()) {
            iconDisplay->setGlowOutline(GameManager::get()->colorForIdx(GameManager::get()->getPlayerGlowColor()));
        }
    }
    this->addChildAtPosition(iconDisplay, Anchor::Left, ccp(22.f, -6.f));

    auto menu = CCMenu::create();
    menu->setPosition(ccp(m_obContentSize.width, m_obContentSize.height / 2 - 6.f));
    this->addChild(menu);

    auto selectBtnSpr = ButtonSprite::create("Select");
    selectBtnSpr->setScale(0.5f);
    auto selectBtn = CCMenuItemSpriteExtra::create(
        selectBtnSpr, this, menu_selector(PresetCell::onSelect)
    );
    selectBtn->setPosition(ccp(-30.f, 0.f));
    menu->addChild(selectBtn);

    auto deleteBtnSpr = ButtonSprite::create("Delete");
    deleteBtnSpr->setScale(0.5f);
    auto deleteBtn = CCMenuItemSpriteExtra::create(
        deleteBtnSpr, this, menu_selector(PresetCell::onDelete)
    );
    deleteBtn->setPosition(ccp(-85.f, 0.f));
    menu->addChild(deleteBtn);

    return true;
}

void PresetCell::onDelete(CCObject*) {
    auto popup = createQuickPopup(
        "Delete List",
        "Are you sure you want to delete <cy>" + m_preset.name + "</c>?",
        "Cancel", "Yes",
        [this](auto, bool btn2) {
            if (btn2) {
                m_presetLayer->deletePreset(m_filename);
            }
        }
    );
}

void PresetCell::onSelect(CCObject*) {
    m_presetLayer->selectPreset(m_preset);
}