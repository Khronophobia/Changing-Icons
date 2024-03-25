#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include "SavePresetLayer.hpp"

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
    this->setTitle("Save List");
    m_closeBtn->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::TopLeft)
            ->setOffset(ccp(10.f, -10.f))
    );
    // m_mainLayer->updateLayout();

    m_textInput = TextInput::create(200.f, "List Name");
    m_textInput->setMaxCharCount(20);
    m_textInput->setCommonFilter(CommonFilter::Name);
    m_buttonMenu->addChildAtPosition(m_textInput, Anchor::Center);

    auto saveBtnSpr = ButtonSprite::create("Save");
    saveBtnSpr->setScale(0.8f);
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveBtnSpr, this, menu_selector(SavePresetLayer::onSave)
    );
    m_buttonMenu->addChildAtPosition(saveBtn, Anchor::Bottom, ccp(0.f, 20.f));

    return true;
}

void SavePresetLayer::onSave(CCObject*) {
    auto const& name = m_textInput->getString();
    if (name.empty()) {
        Notification::create("Name must not be empty", NotificationIcon::Error)->show();
        return;
    }

    auto presetDir = Mod::get()->getConfigDir() / "icon-sets";
    switch (m_iconType) {
        default:
        case IconType::Cube: presetDir /= "cube"; break;
        case IconType::Ship: presetDir /= "ship"; break;
        case IconType::Ball: presetDir /= "ball"; break;
        case IconType::Ufo: presetDir /= "ufo"; break;
        case IconType::Wave: presetDir /= "wave"; break;
        case IconType::Robot: presetDir /= "robot"; break;
        case IconType::Spider: presetDir /= "spider"; break;
        case IconType::Swing: presetDir /= "swing"; break;
        case IconType::Jetpack: presetDir /= "jetpack"; break;
    }
    if (auto res = file::createDirectoryAll(presetDir); res.isErr()) {
        log::error("{}", res.error());
        Notification::create(res.error(), NotificationIcon::Error)->show();
        return;
    }
    presetDir /= name + ".json";
    if (ghc::filesystem::exists(presetDir)) {
        createQuickPopup(
            "Overwrite List",
            "A list with the name <cy>" + name +
            "</c> already exists. Are you sure you want to overwrite it?",
            "Cancel", "Yes",
            [this, presetDir](auto, bool btn2) {
                if (btn2) {
                    savePreset(presetDir);
                    SavePresetLayer::onClose(nullptr);
                }
            }
        );
        return;
    }
    savePreset(presetDir);
    SavePresetLayer::onClose(nullptr);
}

void SavePresetLayer::savePreset(ghc::filesystem::path const& path) const {
    if (auto res = file::writeToJson(path, m_iconSet); res.isErr()) {
        log::error("{}", res.error());
        Notification::create(res.error(), NotificationIcon::Error)->show();
        return;
    }
    Notification::create("Save successful", NotificationIcon::Success)->show();
}