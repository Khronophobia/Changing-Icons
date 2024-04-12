#pragma once
#include <Geode/loader/SettingNode.hpp>

namespace changing_icons {
    class OpenConfigSettingValue;

    class OpenConfigSettingNode : public geode::SettingNode {
    protected:
        bool init(OpenConfigSettingValue* value, float width);
    public:
        void onConfig(CCObject* sender);
        void commit() override {}
        bool hasUncommittedChanges() override {
            return false;
        }
        bool hasNonDefaultValue() override {
            return false;
        }
        void resetToDefault() override {}

        static OpenConfigSettingNode* create(OpenConfigSettingValue* value, float width);
    };

    class OpenConfigSettingValue : public geode::SettingValue {
    public:
        OpenConfigSettingValue(std::string const& key, std::string const& modID, int placeholder)
        : geode::SettingValue(key, modID) {}

        bool load(matjson::Value const& json) override {
            return true;
        };
        bool save(matjson::Value& json) const override {
            return true;
        }
        geode::SettingNode* createNode(float width) override {
            return OpenConfigSettingNode::create(this, width);
        }
    };
}