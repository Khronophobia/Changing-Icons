#include "LoadPresetLayer.hpp"
#include "IconConfigLayer.hpp"
#include "SavePresetPopup.hpp"
#include <class/CIConfigManager.hpp>
#include <class/PresetCell.hpp>
#include <class/IconCell.hpp>
#include <CIConstants.hpp>
#include <properties/IconProperties.hpp>

using namespace geode::prelude;
using namespace changing_icons;

LoadPresetLayer* LoadPresetLayer::create(IconType type) {
    auto ret = new LoadPresetLayer();
    if (ret && ret->initAnchored(400.f, 300.f, type, "GJ_square04.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool LoadPresetLayer::setup(IconType type) {
    m_noElasticity = true;
    m_iconType = type;
    this->setTitle("Saved Lists");
    static_cast<AnchorLayoutOptions*>(m_closeBtn->getLayoutOptions())
        ->setOffset(ccp(10.f, -10.f));
    m_buttonMenu->updateLayout();

    auto loadBtnSpr = ButtonSprite::create("Load");
    loadBtnSpr->setScale(0.7f);
    auto loadBtn = CCMenuItemSpriteExtra::create(
        loadBtnSpr, this, menu_selector(LoadPresetLayer::onLoadSelected)
    );
    m_buttonMenu->addChildAtPosition(loadBtn, Anchor::BottomRight, ccp(-105.f, 20.f));

    auto saveCurrentBtnSpr = ButtonSprite::create("Save Current");
    saveCurrentBtnSpr->setScale(0.7f);
    auto saveCurrentBtn = CCMenuItemSpriteExtra::create(
        saveCurrentBtnSpr, this, menu_selector(LoadPresetLayer::onSaveCurrent)
    );
    m_buttonMenu->addChildAtPosition(saveCurrentBtn, Anchor::BottomLeft, ccp(105.f, 20.f));

    auto presetListBG = CCLayerColor::create();
    presetListBG->ignoreAnchorPointForPosition(false);
    presetListBG->setAnchorPoint(ccp(0.f, 0.5f));
    presetListBG->setOpacity(95);
    presetListBG->setContentSize(ccp(constants::PRESETCELL_WIDTH, 230.f));
    m_mainLayer->addChildAtPosition(presetListBG, Anchor::Left, ccp(20.f, 0.f));

    m_presetList = ScrollLayer::create(presetListBG->getContentSize(), false);
    presetListBG->addChild(m_presetList);

    m_selectedText = CCLabelBMFont::create("None Selected", "bigFont.fnt");
    m_selectedText->setColor(cc3x(0x00ff00));
    m_selectedText->setScale(0.4f);
    m_mainLayer->addChildAtPosition(m_selectedText, Anchor::Right, ccp(-105.f, 114.f));

    auto previewListBG = CCLayerColor::create();
    previewListBG->ignoreAnchorPointForPosition(false);
    previewListBG->setAnchorPoint(ccp(1.f, 0.5f));
    previewListBG->setOpacity(95);
    previewListBG->setContentSize(ccp(constants::ICONCELL_WIDTH, 216.f));
    m_mainLayer->addChildAtPosition(previewListBG, Anchor::Right, ccp(-20.f, -7.f));

    m_previewList = ScrollLayer::create(previewListBG->getContentSize(), false);
    previewListBG->addChild(m_previewList);

    loadPresets();

    return true;
}

void LoadPresetLayer::loadPresets() {
    std::vector<ghc::filesystem::path> presetPaths;
    if (auto res = file::readDirectory(CIManager::getPresetDir(m_iconType))) {
        presetPaths = res.value();
    } else {
        log::error("Error reading preset directory: {}", res.error());
        return;
    }
    if (presetPaths.empty()) return;

    auto content = m_presetList->m_contentLayer;
    auto height = std::max(m_presetList->getContentHeight(), presetPaths.size() * constants::PRESETCELL_HEIGHT);
    content->setContentHeight(height);
    content->removeAllChildren();

    int index = 0;
    for (auto const& path : presetPaths) {
        CIPreset preset;
        if (auto res = file::readJson(path)) {
            preset = res.value().as<CIPreset>();
        } else {
            log::error("Error loading '{}': {}", path.filename(), res.error());
            continue;
        }
        auto cell = PresetCell::create(
            this, index, m_iconType, preset, path.filename()
        );
        cell->setPositionY(height - (index + 1) * constants::PRESETCELL_HEIGHT);
        content->addChild(cell);

        ++index;
    }

    m_presetList->moveToTop();
}

void LoadPresetLayer::selectPreset(CIPreset const& preset) {
    auto content = m_previewList->m_contentLayer;
    auto height = std::max(m_previewList->getContentHeight(), preset.iconSet.size() * constants::ICONCELL_HEIGHT);
    content->setContentHeight(height);
    content->removeAllChildren();

    int index = 0;
    for (auto const& icon : preset.iconSet) {
        auto cell = IconCell::create(index, m_iconType, icon);
        cell->setPositionY(height - (index + 1) * constants::ICONCELL_HEIGHT);
        content->addChild(cell);

        ++index;
    }
    m_previewList->moveToTop();

    m_selectedText->setString(preset.name.c_str());
    m_selectedText->limitLabelWidth(constants::ICONCELL_WIDTH, 0.4f, 0.1f);
    m_selectedPreset = preset;
}

void LoadPresetLayer::deletePreset(ghc::filesystem::path const& filename) {
    std::error_code error;
    if (!ghc::filesystem::remove(CIManager::getPresetDir(m_iconType) / filename, error)) {
        Notification::create(error.message(), NotificationIcon::Error)->show();
        return;
    }
    loadPresets();
}

void LoadPresetLayer::onLoadSelected(CCObject*) {
    if (!m_selectedPreset) {
        Notification::create("No List Selected")->show();
        return;
    }
    
    createQuickPopup(
        "Load List",
        "Are you sure you want to load <cy>" + m_selectedPreset->name + "</c>?",
        "Cancel", "Yes",
        [this](auto, bool btn2) {
            if (btn2) {
                CIManager::get()->getConfigLayer()->replaceList(m_selectedPreset->iconSet);
                LoadPresetLayer::onClose(nullptr);
            }
        }
    );
}

void LoadPresetLayer::onSaveCurrent(CCObject*) {
    if (CIManager::get()->getConfigLayer()->getCurrentConfig().iconSet.empty()) {
        Notification::create("Current list is empty")->show();
        return;
    }
    SavePresetPopup::create(this, m_iconType)->show();
}