#pragma once
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

namespace changing_icons {
    struct IconProperties;

    class SavePresetLayer : public geode::Popup<IconType>, TextInputDelegate {
    protected:
        IconType m_iconType;
        std::vector<IconProperties> const& m_iconSet;
        geode::TextInput* m_textInput;
        cocos2d::CCLabelBMFont* m_filenameHint;
        cocos2d::CCLabelBMFont* m_inputWarning;

        bool setup(IconType type) override;
        std::optional<std::string> checkForNameError(std::string_view name);
        std::string convertToFileName(std::string name);
        geode::Result<> savePreset(ghc::filesystem::path const& path) const;
    public:
        SavePresetLayer(std::vector<IconProperties> const& iconSet) : m_iconSet(iconSet) {}
        static SavePresetLayer* create(IconType const type, std::vector<IconProperties> const& set);
        void onSave(CCObject*);

        void textChanged(CCTextInputNode* input) override;
    };
}