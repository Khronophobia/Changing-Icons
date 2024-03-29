#pragma once
#include <Geode/binding/FLAlertLayer.hpp>

namespace changing_icons {
    class TriTogglerInfoLayer : public FLAlertLayer {
    protected:
        bool init() override;
    public:
        static TriTogglerInfoLayer* create();
        void registerWithTouchDispatcher() override;
    };
}