#pragma once
#include <Geode/ui/Popup.hpp>

namespace changing_icons {
    class CIConfigManager;

    class GlobalConfigLayer : public geode::Popup<> {
    protected:
        CIConfigManager* m_configManager;

        bool setup();
    public:
        static GlobalConfigLayer* create();
    };
}