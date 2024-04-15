#pragma once
#include <class/IconCellDelegate.hpp>

namespace changing_icons {
    struct CIPreset;
    class IconConfigLayer;

    class LoadPresetLayer : public geode::Popup<IconType>{
    protected:
        IconType m_iconType;
        geode::ScrollLayer* m_presetList;
        geode::ScrollLayer* m_previewList;
        cocos2d::CCLabelBMFont* m_selectedText;
        std::optional<CIPreset> m_selectedPreset;

        bool setup(IconType type) override;
        void loadPresets();
    public:
        static LoadPresetLayer* create(IconType type);
        void selectPreset(CIPreset const& preset);
        void deletePreset(ghc::filesystem::path const& filename);

        void onLoadSelected(CCObject*);
        void onSaveCurrent(CCObject*);

        friend class SavePresetPopup;
    };
}