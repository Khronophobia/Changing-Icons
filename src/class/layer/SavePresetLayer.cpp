#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include "SavePresetLayer.hpp"
#include <class/CIConfigManager.hpp>
#include <CIConstants.hpp>
#include <CIConfigProperties.hpp>

using namespace geode::prelude;
using namespace changing_icons;

SavePresetLayer* SavePresetLayer::create(IconType type, std::vector<IconProperties> const& set) {
    auto ret = new SavePresetLayer(set);
    if (ret && ret->initAnchored(250.f, 120.f, type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SavePresetLayer::setup(IconType type) {
    m_noElasticity = true;
    m_iconType = type;
    this->setTitle("Save Set");
    m_closeBtn->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::TopLeft)
            ->setOffset(ccp(10.f, -10.f))
    );
    // m_mainLayer->updateLayout();

    m_textInput = TextInput::create(200.f, "Name");
    m_textInput->setMaxCharCount(20);
    m_textInput->setCommonFilter(CommonFilter::Any);
    m_textInput->setDelegate(this);
    m_buttonMenu->addChildAtPosition(m_textInput, Anchor::Center);

    m_filenameHint = CCLabelBMFont::create("", "bigFont.fnt");
    m_filenameHint->setVisible(false);
    m_mainLayer->addChildAtPosition(m_filenameHint, Anchor::Center, ccp(0.f, -20.f));

    m_inputWarning = CCLabelBMFont::create("Trailing whitespaces will be trimmed", "bigFont.fnt");
    m_inputWarning->setVisible(false);
    m_inputWarning->setColor(cc3x(0xff0));
    m_inputWarning->limitLabelWidth(200.f, 0.4f, 0.1f);
    m_mainLayer->addChildAtPosition(m_inputWarning, Anchor::Center, ccp(0.f, 24.f));

    auto saveBtnSpr = ButtonSprite::create("Save");
    saveBtnSpr->setScale(0.8f);
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveBtnSpr, this, menu_selector(SavePresetLayer::onSave)
    );
    m_buttonMenu->addChildAtPosition(saveBtn, Anchor::Bottom, ccp(0.f, 20.f));

    return true;
}

std::string SavePresetLayer::convertToFileName(std::string name) {
    for (auto character : constants::ILLEGAL_CHARS) {
        name = string::replace(name, character, "_");
    }
    name = string::trimRight(name);
    return name;
}

std::optional<std::string> SavePresetLayer::checkForNameError(std::string_view name) {
    if (name.empty()) return "Name must not be empty";
    if (name.starts_with(" ")) return "Name cannot start with a space";

    return std::nullopt;
}

void SavePresetLayer::onSave(CCObject*) {
    auto const& name = convertToFileName(m_textInput->getString());
    if (auto const& err = checkForNameError(name)) {
        Notification::create(err.value(), NotificationIcon::Error)->show();
        return;
    }

    auto presetDir = CIConfigManager::getPresetDir(m_iconType);
    presetDir /= name + ".json";
    if (ghc::filesystem::exists(presetDir)) {
        createQuickPopup(
            "Overwrite List",
            "A set with the name <cy>" + name +
            ".json</c> already exists. Do you want to overwrite it?",
            "No", "Yes",
            [this, presetDir](auto, bool btn2) {
                if (btn2) {
                    if (savePreset(presetDir).isErr()) return;
                    SavePresetLayer::onClose(nullptr);
                }
            }
        );
        return;
    }
    if (savePreset(presetDir).isErr()) return;
    SavePresetLayer::onClose(nullptr);
}

geode::Result<> SavePresetLayer::savePreset(ghc::filesystem::path const& path) const {
    auto contents = CIPreset{
        .name = string::trimRight(m_textInput->getString()),
        .iconSet = m_iconSet,
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

void SavePresetLayer::textChanged(CCTextInputNode* input) {
    auto const& str = convertToFileName(input->getString());
    if (checkForNameError(str)) {
        m_filenameHint->setVisible(false);
        return;
    }
    if (string::endsWith(input->getString(), " ")) m_inputWarning->setVisible(true);
    else m_inputWarning->setVisible(false);
    m_filenameHint->setVisible(true);
    m_filenameHint->setCString(fmt::format("Will be saved as: {}.json", str).c_str());
    m_filenameHint->limitLabelWidth(200.f, 0.4f, 0.1f);
}