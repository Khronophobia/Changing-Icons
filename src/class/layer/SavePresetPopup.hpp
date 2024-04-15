#pragma once
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

namespace changing_icons {
    struct IconProperties;
    class LoadPresetLayer;

    class SavePresetPopup : public geode::Popup<LoadPresetLayer*, IconType>, TextInputDelegate {
    protected:
        LoadPresetLayer* m_presetLayer;
        IconType m_iconType;
        geode::ScrollLayer* m_iconList;
        geode::TextInput* m_textInput;
        cocos2d::CCLabelBMFont* m_filenameHint;
        cocos2d::CCLabelBMFont* m_inputWarning;

        bool setup(LoadPresetLayer* presetLayer, IconType type) override;
        void loadIconList();
        std::optional<std::string> checkForNameError(std::string_view name);
        std::string convertToFileName(std::string name);
        geode::Result<> savePreset(ghc::filesystem::path const& path) const;
    public:
        static SavePresetPopup* create(LoadPresetLayer* presetLayer, IconType const type);
        void onSave(CCObject*);

        void textChanged(CCTextInputNode* input) override;
    };
}