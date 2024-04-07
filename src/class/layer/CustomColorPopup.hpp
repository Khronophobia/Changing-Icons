#pragma once

namespace changing_icons {
    class AddIconLayer;

    class CustomColorPopup : public geode::Popup<AddIconLayer*> {
    protected:
        AddIconLayer* m_instance;

        bool setup(AddIconLayer* instance) override;
    public:
        static CustomColorPopup* create(AddIconLayer* instance);
    };
}