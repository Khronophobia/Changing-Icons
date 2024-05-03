#include <Geode/Geode.hpp>
#include <class/CIConfigManager.hpp>
#include "OpenConfigSetting.hpp"

using namespace geode::prelude;
using namespace changing_icons;

$on_mod(Loaded) {
    CIManager::get();

    Mod::get()->addCustomSetting<OpenConfigSettingValue>("open-config", 0);

    Mod::get()->setSavedValue("version", 0);
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Cube)); res.isErr()) {
        log::error("Creating cube preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Ship)); res.isErr()) {
        log::error("Creating ship preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Ball)); res.isErr()) {
        log::error("Creating ball preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Ufo)); res.isErr()) {
        log::error("Creating UFO preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Wave)); res.isErr()) {
        log::error("Creating wave preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Robot)); res.isErr()) {
        log::error("Creating robot preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Spider)); res.isErr()) {
        log::error("Creating spider preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Swing)); res.isErr()) {
        log::error("Creating swing preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIManager::getPresetDir(IconType::Jetpack)); res.isErr()) {
        log::error("Creating jetpack preset directory failed: {}", res.error());
    }
}