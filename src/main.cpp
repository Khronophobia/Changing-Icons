#include <Geode/Geode.hpp>
#include <class/CIConfigManager.hpp>

using namespace geode::prelude;
using namespace changing_icons;

$on_mod(Loaded) {
    Mod::get()->setSavedValue("version", 0);
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Cube)); res.isErr()) {
        log::error("Creating cube preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Ship)); res.isErr()) {
        log::error("Creating ship preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Ball)); res.isErr()) {
        log::error("Creating ball preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Ufo)); res.isErr()) {
        log::error("Creating UFO preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Wave)); res.isErr()) {
        log::error("Creating wave preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Robot)); res.isErr()) {
        log::error("Creating robot preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Spider)); res.isErr()) {
        log::error("Creating spider preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Swing)); res.isErr()) {
        log::error("Creating swing preset directory failed: {}", res.error());
    }
    if (auto res = file::createDirectoryAll(CIConfigManager::getPresetDir(IconType::Jetpack)); res.isErr()) {
        log::error("Creating jetpack preset directory failed: {}", res.error());
    }
}