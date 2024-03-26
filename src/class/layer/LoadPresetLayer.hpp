#pragma once
#include <Geode/ui/Popup.hpp>

namespace changing_icons {
    struct CIPreset;
    class IconConfigLayer;

    class LoadPresetLayer : public geode::Popup<IconConfigLayer*, IconType> {
    protected:
        IconConfigLayer* m_configLayer;
        IconType m_iconType;
        cocos2d::CCLayerColor* m_setsListBg;
        geode::ScrollLayer* m_setsList;
        geode::Ref<geode::Scrollbar> m_setsScrollbar;
        cocos2d::CCLayerColor* m_presetViewBg;
        geode::ScrollLayer* m_presetViewList;
        geode::Ref<geode::Scrollbar> m_presetScrollbar;
        CCMenuItemSpriteExtra* m_presetBackBtn;

        bool setup(IconConfigLayer* configLayer, IconType type) override;
        void refreshSets(bool toTop = false);
        void refreshLoadBtn();
    public:
        static LoadPresetLayer* create(IconConfigLayer* configLayer, IconType type);
        void viewPreset(CIPreset const& preset);
        void loadPreset(CIPreset const& preset);
        void deletePreset(ghc::filesystem::path const& filename);

        void onBack(CCObject*);
    };
}