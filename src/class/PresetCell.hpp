#pragma once
#include <properties/Preset.hpp>

namespace changing_icons {
    class LoadPresetLayer;

    class PresetCell : public cocos2d::CCLayerColor {
    protected:
        LoadPresetLayer* m_presetLayer;
        CIPreset m_preset;
        int m_index;
        IconType m_iconType;
        cocos2d::CCLabelBMFont* m_presetName;
        ghc::filesystem::path m_filename;

        bool init(LoadPresetLayer* presetLayer, int index, IconType type, CIPreset const& preset, ghc::filesystem::path const& filename);
    public:
        static PresetCell* create(LoadPresetLayer* presetLayer, int index, IconType type, CIPreset const& preset, ghc::filesystem::path const& filename);

        void onLoad(CCObject*);
        void onView(CCObject*);
        void onDelete(CCObject*);
    };
}