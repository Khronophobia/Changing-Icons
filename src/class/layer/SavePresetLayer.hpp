#pragma once
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <CIConfigProperties.hpp>

namespace changing_icons {
    class SavePresetLayer : public geode::Popup<IconType> {
    protected:
        IconType m_iconType;
        std::vector<IconProperties> const& m_iconSet;
        geode::TextInput* m_textInput;

        bool setup(IconType type) override;
        void savePreset(ghc::filesystem::path const& path) const;
    public:
        SavePresetLayer(std::vector<IconProperties> const& iconSet) : m_iconSet(iconSet) {}
        static SavePresetLayer* create(IconType const type, std::vector<IconProperties> const& set);
        void onSave(CCObject*);
    };
}