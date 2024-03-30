#include "GJBaseGameLayer.hpp"
#include <class/CIConfigManager.hpp>

using namespace geode::prelude;
using namespace changing_icons;

void CIBaseGameLayer::setupLevelStart(LevelSettingsObject* p0) {
    GJBaseGameLayer::setupLevelStart(p0);

    if (!m_fields->m_ciHasInit) {
        m_fields->m_player1CIProperties = {
            {IconType::Cube, setupCIValues(IconType::Cube)},
            {IconType::Ship,
            (m_player1->m_isPlatformer ? setupCIValues(IconType::Jetpack) : setupCIValues(IconType::Ship))},
            {IconType::Ball, setupCIValues(IconType::Ball)},
            {IconType::Ufo, setupCIValues(IconType::Ufo)},
            {IconType::Wave, setupCIValues(IconType::Wave)},
            {IconType::Robot, setupCIValues(IconType::Robot)},
            {IconType::Spider, setupCIValues(IconType::Spider)},
            {IconType::Swing, setupCIValues(IconType::Swing)}
        };
        log::info("Player 1 CI values initialized");

        m_fields->m_player2CIProperties = {
            {IconType::Cube, setupCIValues(IconType::Cube)},
            {
                IconType::Ship,
                (m_player1->m_isPlatformer ? setupCIValues(IconType::Jetpack) : setupCIValues(IconType::Ship))
            },
            {IconType::Ball, setupCIValues(IconType::Ball)},
            {IconType::Ufo, setupCIValues(IconType::Ufo)},
            {IconType::Wave, setupCIValues(IconType::Wave)},
            {IconType::Robot, setupCIValues(IconType::Robot)},
            {IconType::Spider, setupCIValues(IconType::Spider)},
            {IconType::Swing, setupCIValues(IconType::Swing)}
        };
        log::info("Player 2 CI values initialized");

        m_fields->m_ciHasInit = true;
    }
}

CITempProperties CIBaseGameLayer::setupCIValues(IconType type) {
    auto gm = GameManager::get();
    auto const& config = CIConfigManager::get()->getConfig(type);
    auto const& globalConfig = CIConfigManager::get()->getGlobalConfig();

    auto order = config.order;
    auto disabled = config.disabled;
    auto useAll = config.useAll;
    auto includePlayerIcon = config.includePlayerIcon;
    auto mirrorEnd = config.mirrorEnd;
    if (globalConfig.globalOverrides.find(type) != globalConfig.globalOverrides.end()) {
        log::info("Gamemode {} listed in global overrides", static_cast<int>(type));
        if (globalConfig.override.order) order = globalConfig.override.order.value();
        if (globalConfig.override.disabled) disabled = globalConfig.override.disabled.value();
        if (globalConfig.override.useAll) useAll = globalConfig.override.useAll.value();
        if (globalConfig.override.includePlayerIcon) includePlayerIcon = globalConfig.override.includePlayerIcon.value();
    }
    int index;
    if (useAll) {
        switch (order) {
            case IconOrder::Random: [[fallthrough]];
            case IconOrder::Forward: index = 1; break;
            case IconOrder::Backward: index = gm->countForType(type); break;
        }
    } else {
        switch (order) {
            case IconOrder::Random: [[fallthrough]];
            case IconOrder::Forward: index = 0; break;
            case IconOrder::Backward: index = config.iconSet.size() - 1; break;
        }
    }
    auto iconSet = config.iconSet;
    if (includePlayerIcon && !iconSet.empty()) {
        int playerIconID;
        switch (type) {
            default:
            case IconType::Cube: playerIconID = gm->getPlayerFrame(); break;
            case IconType::Ship: playerIconID = gm->getPlayerShip(); break;
            case IconType::Ball: playerIconID = gm->getPlayerBall(); break;
            case IconType::Ufo: playerIconID = gm->getPlayerBird(); break;
            case IconType::Wave: playerIconID = gm->getPlayerDart(); break;
            case IconType::Robot: playerIconID = gm->getPlayerRobot(); break;
            case IconType::Spider: playerIconID = gm->getPlayerSpider(); break;
            case IconType::Swing: playerIconID = gm->getPlayerSwing(); break;
            case IconType::Jetpack: playerIconID = gm->getPlayerJetpack(); break;
        }
        iconSet.push_back(IconProperties{ .iconID = playerIconID });
    }

    return CITempProperties{
        .current = index,
        .index = index,
        .iconSet = iconSet,
        .order = order,
        .useAll = useAll,
        .mirrorEnd = mirrorEnd,
        .disabled = disabled
    };
}