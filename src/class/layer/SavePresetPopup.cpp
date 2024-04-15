#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include "SavePresetPopup.hpp"
#include "LoadPresetLayer.hpp"
#include "IconConfigLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <class/IconCell.hpp>
#include <CIConstants.hpp>
#include <properties/IconProperties.hpp>
#include <properties/Preset.hpp>

using namespace geode::prelude;
using namespace changing_icons;

SavePresetPopup* SavePresetPopup::create(LoadPresetLayer* presetLayer, IconType type) {
    auto ret = new SavePresetPopup();
    if (ret && ret->initAnchored(240.f, 300.f, presetLayer, type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SavePresetPopup::setup(LoadPresetLayer* presetLayer, IconType type) {
    m_noElasticity = true;
    m_presetLayer = presetLayer;
    m_iconType = type;
    this->setTitle("Save Current List");
    m_closeBtn->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::TopLeft)
            ->setOffset(ccp(10.f, -10.f))
    );

    auto listBG = CCLayerColor::create();
    listBG->setOpacity(95);
    listBG->ignoreAnchorPointForPosition(false);
    listBG->setContentSize(ccp(constants::ICONCELL_WIDTH, 252.f));
    listBG->setScale(0.75f);
    m_mainLayer->addChildAtPosition(listBG, Anchor::Center, ccp(0.f, 24.f));

    m_iconList = ScrollLayer::create(listBG->getContentSize(), false);
    listBG->addChild(m_iconList);

    loadIconList();

    m_textInput = TextInput::create(250.f, "Name");
    m_textInput->setMaxCharCount(20);
    m_textInput->setCommonFilter(CommonFilter::Any);
    m_textInput->setDelegate(this);
    m_textInput->setScale(0.8f);
    m_mainLayer->addChildAtPosition(m_textInput, Anchor::Bottom, ccp(0.f, 56.f));

    m_filenameHint = CCLabelBMFont::create("", "bigFont.fnt");
    m_filenameHint->setVisible(false);
    m_mainLayer->addChildAtPosition(m_filenameHint, Anchor::Bottom, ccp(0.f, 40.f));

    m_inputWarning = CCLabelBMFont::create("Trailing whitespaces will be trimmed", "bigFont.fnt");
    m_inputWarning->setVisible(false);
    m_inputWarning->setColor(cc3x(0xff0));
    m_inputWarning->limitLabelWidth(200.f, 0.3f, 0.1f);
    m_mainLayer->addChildAtPosition(m_inputWarning, Anchor::Bottom, ccp(0.f, 74.f));

    auto saveBtnSpr = ButtonSprite::create("Save");
    saveBtnSpr->setScale(0.8f);
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveBtnSpr, this, menu_selector(SavePresetPopup::onSave)
    );
    m_buttonMenu->addChildAtPosition(saveBtn, Anchor::Bottom, ccp(0.f, 20.f));

    return true;
}

void SavePresetPopup::loadIconList() {
    auto const& iconSet = CIManager::get()->getConfigLayer()->getCurrentConfig().iconSet;

    auto content = m_iconList->m_contentLayer;
    auto height = std::max(m_iconList->getContentHeight(), iconSet.size() * constants::ICONCELL_HEIGHT);
    content->setContentHeight(height);

    int index = 0;
    for (auto const& icon : iconSet) {
        auto cell = IconCell::create(index, m_iconType, icon);

        cell->setPositionY(height - (index + 1) * constants::ICONCELL_HEIGHT);
        content->addChild(cell);

        ++index;
    }
    m_iconList->moveToTop();
}

std::string SavePresetPopup::convertToFileName(std::string name) {
    for (auto character : constants::ILLEGAL_CHARS) {
        name = string::replace(name, std::string(1, character), "_");
    }
    name = string::trimRight(name);
    return name;
}

std::optional<std::string> SavePresetPopup::checkForNameError(std::string_view name) {
    if (name.empty()) return "Name must not be empty";
    if (name.starts_with(" ")) return "Name cannot start with a space";

    return std::nullopt;
}

void SavePresetPopup::onSave(CCObject*) {
    auto const& name = convertToFileName(m_textInput->getString());
    if (auto const& err = checkForNameError(name)) {
        Notification::create(err.value(), NotificationIcon::Error)->show();
        return;
    }

    auto presetDir = CIManager::getPresetDir(m_iconType);
    presetDir /= name + ".json";
    if (ghc::filesystem::exists(presetDir)) {
        createQuickPopup(
            "Overwrite File",
            "A file with the name <cy>" + name +
            ".json</c> already exists. Do you want to overwrite it?",
            "No", "Yes",
            [this, presetDir](auto, bool btn2) {
                if (btn2) {
                    if (!savePreset(presetDir)) return;
                    SavePresetPopup::onClose(nullptr);
                    m_presetLayer->loadPresets();
                }
            }
        );
        return;
    }
    if (!savePreset(presetDir)) return;
    SavePresetPopup::onClose(nullptr);
    m_presetLayer->loadPresets();
}

geode::Result<> SavePresetPopup::savePreset(ghc::filesystem::path const& path) const {
    auto const& currentConfig = CIManager::get()->getConfigLayer()->getCurrentConfig();
    auto contents = CIPreset{
        .name = string::trimRight(m_textInput->getString()),
        .iconSet = currentConfig.iconSet,
        .formatVersion = 0,
    };
    auto res = file::writeToJson(path, contents);
    if (res.isErr()) {
        log::error("{}", res.error());
        Notification::create(res.error(), NotificationIcon::Error)->show();
    } else {
        Notification::create("Save successful", NotificationIcon::Success)->show();
    }
    return res;
}

void SavePresetPopup::textChanged(CCTextInputNode* input) {
    auto const& str = convertToFileName(input->getString());
    if (checkForNameError(str)) {
        m_filenameHint->setVisible(false);
        return;
    }
    if (string::endsWith(input->getString(), " ")) m_inputWarning->setVisible(true);
    else m_inputWarning->setVisible(false);
    m_filenameHint->setVisible(true);
    m_filenameHint->setCString(fmt::format("Will be saved as: {}.json", str).c_str());
    m_filenameHint->limitLabelWidth(200.f, 0.3f, 0.1f);
}