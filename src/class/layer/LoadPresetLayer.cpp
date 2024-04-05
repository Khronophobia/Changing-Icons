#include "LoadPresetLayer.hpp"
#include "IconConfigLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <class/PresetCell.hpp>
#include <class/IconCell.hpp>
#include <CIConstants.hpp>

using namespace geode::prelude;
using namespace changing_icons;

LoadPresetLayer* LoadPresetLayer::create(IconConfigLayer* configLayer, IconType type) {
    auto ret = new LoadPresetLayer();
    if (ret && ret->initAnchored(270.f, 300.f, configLayer, type, "GJ_square04.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool LoadPresetLayer::setup(IconConfigLayer* configLayer, IconType type) {
    this->setTitle("Load List");
    m_noElasticity = true;
    m_configLayer = configLayer;
    m_iconType = type;
    static_cast<AnchorLayoutOptions*>(m_closeBtn->getLayoutOptions())
        ->setOffset(ccp(10.f, -10.f));
    m_buttonMenu->updateLayout();

    m_setsListBg = CCLayerColor::create({0, 0, 0, 95});
    m_setsListBg->ignoreAnchorPointForPosition(false);
    m_setsListBg->setContentSize(ccp(constants::PRESETCELL_WIDTH, 250.f));
    m_mainLayer->addChildAtPosition(m_setsListBg, Anchor::Center, ccp(0.f, -10.f));

    m_setsList = ScrollLayer::create(m_setsListBg->getContentSize(), false);
    m_setsListBg->addChild(m_setsList);

    m_setsScrollbar = Scrollbar::create(m_setsList);
    m_setsListBg->addChildAtPosition(
        m_setsScrollbar, Anchor::Right, ccp(5.f, 0.f), false
    );

    refreshSets(true);

    m_presetViewBg = CCLayerColor::create({0, 0, 0, 95});
    m_presetViewBg->ignoreAnchorPointForPosition(false);
    m_presetViewBg->setContentSize(ccp(constants::ICONCELL_WIDTH, 216.f));
    m_mainLayer->addChildAtPosition(m_presetViewBg, Anchor::Center);

    m_presetViewList = ScrollLayer::create(m_presetViewBg->getContentSize(), false);
    m_presetViewBg->addChild(m_presetViewList);

    m_presetScrollbar = Scrollbar::create(m_presetViewList);

    auto presetBackSpr = ButtonSprite::create("Back");
    presetBackSpr->setScale(0.75f);
    m_presetBackBtn = CCMenuItemSpriteExtra::create(
        presetBackSpr,
        this,
        menu_selector(LoadPresetLayer::onBack)
    );
    m_buttonMenu->addChildAtPosition(m_presetBackBtn, Anchor::Bottom, ccp(0.f, 20.f));

    m_presetBackBtn->setVisible(false);
    m_presetViewBg->setVisible(false);
    m_presetViewList->m_disableMovement = true;

    return true;
}

void LoadPresetLayer::refreshSets(bool toTop) {
    auto res = file::readDirectory(CIManager::getPresetDir(m_iconType));
    if (res.isErr()) {
        log::error("{}", res.error());
        return;
    }
    if (res.value().empty()) return;

    auto list = res.value();

    auto content = m_setsList->m_contentLayer;
    auto height = std::max(m_setsList->getContentHeight(), list.size() * constants::PRESETCELL_HEIGHT);
    content->setContentHeight(height);
    content->removeAllChildren();
    size_t i = 0;
    for (auto const& path : list) {
        auto res = file::readJson(path);
        if (res.isErr()) {
            log::error("Error loading '{}': {}", path.filename(), res.error());
            continue;
        }
        log::info("Loaded '{}'", path.filename());
        auto const& preset = res.value().as<CIPreset>();
        auto cell = PresetCell::create(
            this,
            i,
            m_iconType,
            preset,
            path.filename()
        );
        cell->setPositionY(height - (i + 1) * constants::PRESETCELL_HEIGHT);
        content->addChild(cell);
        i++;
    }
    if (toTop) {
        m_setsList->moveToTop();
        return;
    }
    auto maxScrollPos = m_setsList->getContentHeight() - content->getContentHeight();
    content->setPositionY(std::clamp<float>(
        content->getPositionY() + constants::PRESETCELL_HEIGHT,
        maxScrollPos,
        0.f
    ));
}

void LoadPresetLayer::viewPreset(CIPreset const& preset) {
    this->setTitle(preset.name);

    m_setsScrollbar->removeFromParent();
    m_setsListBg->setVisible(false);
    m_setsList->m_disableMovement = true;

    m_presetViewBg->setVisible(true);
    m_presetViewList->m_disableMovement = false;
    m_presetViewBg->addChildAtPosition(m_presetScrollbar, Anchor::Right, ccp(5.f, 0.f));
    m_presetBackBtn->setVisible(true);

    auto height = std::max(m_presetViewList->getContentHeight(), preset.iconSet.size() * constants::ICONCELL_HEIGHT);
    m_presetViewList->m_contentLayer->setContentHeight(height);
    m_presetViewList->m_contentLayer->removeAllChildren();
    size_t i = 0;
    for (auto const& icon : preset.iconSet) {
        auto cell = IconCell::create(
            i,
            m_iconType,
            icon
        );
        cell->setPositionY(height - (i + 1) * constants::ICONCELL_HEIGHT);
        m_presetViewList->m_contentLayer->addChild(cell);
        i++;
    }
    m_presetViewList->moveToTop();
}

void LoadPresetLayer::loadPreset(CIPreset const& preset) {
    m_configLayer->replaceList(preset.iconSet);
    LoadPresetLayer::onClose(nullptr);
}

void LoadPresetLayer::deletePreset(ghc::filesystem::path const& filename) {
    auto presetDir = CIManager::getPresetDir(m_iconType) / filename.filename();
    std::error_code error;
    if (!ghc::filesystem::remove(presetDir, error)) {
        log::error("Error deleting {}: {}", filename.filename(), error);
        return;
    }
    refreshSets();
}

void LoadPresetLayer::onBack(CCObject*) {
    this->setTitle("Load List");
    m_presetScrollbar->removeFromParent();
    m_presetBackBtn->setVisible(false);
    m_presetViewBg->setVisible(false);
    m_presetViewList->m_disableMovement = true;

    m_setsListBg->setVisible(true);
    m_setsList->m_disableMovement = false;
    m_setsListBg->addChildAtPosition(
        m_setsScrollbar, Anchor::Right, ccp(5.f, 0.f), false
    );
}