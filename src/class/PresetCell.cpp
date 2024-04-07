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
    m_index = index;
    m_iconType = type;
    m_filename = filename;

    this->setContentSize(ccp(constants::PRESETCELL_WIDTH, constants::PRESETCELL_HEIGHT));
    this->setAnchorPoint(ccp(0.f, 0.f));
    if (index % 2 == 0)
        this->setOpacity(100);
    else
        this->setOpacity(50);
    auto playerColor1 = GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor());
    auto playerColor2 = GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2());

    m_presetName = CCLabelBMFont::create(preset.name.c_str(), "bigFont.fnt");
    m_presetName->setAnchorPoint(ccp(0.f, 0.5f));
    m_presetName->limitLabelWidth(constants::PRESETCELL_WIDTH - 4, 0.4f, 0.1f);
    this->addChildAtPosition(m_presetName, Anchor::TopLeft, ccp(2.f, -8.f));

    auto const& firstIcon = preset.iconSet.at(0);
    if (firstIcon.color1) playerColor1 = utils::getColorFromVariant(firstIcon.color1.value());
    if (firstIcon.color2) playerColor2 = utils::getColorFromVariant(firstIcon.color2.value());

    auto iconDisplay = SimplePlayer::create(0);
    iconDisplay->setScale(0.7f);
    iconDisplay->updatePlayerFrame(firstIcon.iconID, type);
    iconDisplay->setColor(playerColor1);
    iconDisplay->setSecondColor(playerColor2);
    this->addChildAtPosition(iconDisplay, Anchor::BottomLeft, ccp(24.f, 16.f));

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(20.f, 20.f));
    this->addChildAtPosition(menu, Anchor::BottomRight, ccp(0.f, 16.f), false);

    auto loadBtnSpr = ButtonSprite::create("Load");
    loadBtnSpr->setScale(0.5f);
    auto loadBtn = CCMenuItemSpriteExtra::create(
        loadBtnSpr,
        this,
        menu_selector(PresetCell::onLoad)
    );
    loadBtn->setPositionX(-24.f);
    menu->addChild(loadBtn);

    auto previewBtnSpr = ButtonSprite::create("View");
    previewBtnSpr->setScale(0.5f);
    auto previewBtn = CCMenuItemSpriteExtra::create(
        previewBtnSpr,
        this,
        menu_selector(PresetCell::onView)
    );
    previewBtn->setPositionX(-64.f);
    menu->addChild(previewBtn);

    auto deleteBtnSpr = ButtonSprite::create("Delete");
    deleteBtnSpr->setScale(0.5f);
    auto deleteBtn = CCMenuItemSpriteExtra::create(
        deleteBtnSpr,
        this,
        menu_selector(PresetCell::onDelete)
    );
    deleteBtn->setPositionX(-110.f);
    menu->addChild(deleteBtn);

    return true;
}

void PresetCell::onLoad(CCObject* sender) {
    createQuickPopup(
        "Load Set",
        "Load <cy>" + m_preset.name + "</c>?",
        "Cancel", "Load",
        [this](auto, bool btn2) {
            if (btn2) {
                m_presetLayer->loadPreset(m_preset);
            }
        }
    );
}

void PresetCell::onView(CCObject* sender) {
    m_presetLayer->viewPreset(m_preset);
}

void PresetCell::onDelete(CCObject* sender) {
    createQuickPopup(
        "Delete Set",
        "Are you sure you want to delete <cy>" +
            m_preset.name + "</c>?",
        "Cancel", "Yes",
        [this](auto, bool btn2) {
            if (btn2) {
                m_presetLayer->deletePreset(m_filename);
            }
        }
    );
}