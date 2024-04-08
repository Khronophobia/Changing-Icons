#include "PlayLayer.hpp"
#include "PlayerObject.hpp"
#include <class/CIConfigManager.hpp>
#include <Random.hpp>

using namespace geode::prelude;
using namespace changing_icons;

$override
void CIPlayLayer::setupHasCompleted() {
    for (auto const& pair : CIManager::get()->getConfigMap()) {
        CIManager::get()->refreshUnlockedIcons(pair.first);
    }
    log::info("Unlocked icons refreshed");

    auto ciPlayer1 = static_cast<CIPlayerObject*>(m_player1);
    auto ciPlayer2 = static_cast<CIPlayerObject*>(m_player2);
    ciPlayer1->m_fields->m_ciProperties = {
        setupCIValues(IconType::Cube),
        setupCIValues(IconType::Ship),
        setupCIValues(IconType::Ball),
        setupCIValues(IconType::Ufo),
        setupCIValues(IconType::Wave),
        setupCIValues(IconType::Robot),
        setupCIValues(IconType::Spider),
        setupCIValues(IconType::Swing),
        setupCIValues(IconType::Jetpack)
    };
    ciPlayer2->m_fields->m_ciProperties = {
        setupCIValues(IconType::Cube),
        setupCIValues(IconType::Ship),
        setupCIValues(IconType::Ball),
        setupCIValues(IconType::Ufo),
        setupCIValues(IconType::Wave),
        setupCIValues(IconType::Robot),
        setupCIValues(IconType::Spider),
        setupCIValues(IconType::Swing),
        setupCIValues(IconType::Jetpack)
    };
    log::info("CI values initialized");

    PlayLayer::setupHasCompleted();

    ciPlayer1->m_fields->m_levelStarted = true;
    ciPlayer2->m_fields->m_levelStarted = true;

    auto startMode = m_levelSettings->m_startMode;
    switch (startMode) {
    default: break;
    case 0:
        m_player1->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        m_player2->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        break;
    case 1:
        m_player1->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        m_player2->updatePlayerFrame(GameManager::get()->getPlayerFrame());

        if (m_player1->m_isPlatformer)
            m_player1->updatePlayerJetpackFrame(GameManager::get()->getPlayerJetpack());
        else
            m_player1->updatePlayerShipFrame(GameManager::get()->getPlayerShip());

        if (m_player2->m_isPlatformer)
            m_player2->updatePlayerJetpackFrame(GameManager::get()->getPlayerJetpack());
        else
            m_player2->updatePlayerShipFrame(GameManager::get()->getPlayerShip());

        break;
    case 2:
        m_player1->updatePlayerRollFrame(GameManager::get()->getPlayerBall());
        m_player2->updatePlayerRollFrame(GameManager::get()->getPlayerBall());
        break;
    case 3:
        m_player1->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        m_player2->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        m_player1->updatePlayerBirdFrame(GameManager::get()->getPlayerBird());
        m_player2->updatePlayerBirdFrame(GameManager::get()->getPlayerBird());
        break;
    case 4:
        m_player1->updatePlayerDartFrame(GameManager::get()->getPlayerDart());
        m_player2->updatePlayerDartFrame(GameManager::get()->getPlayerDart());
        break;
    case 5:
        m_player1->updatePlayerRobotFrame(
            ciPlayer1->getNextIconCI(IconType::Robot, GameManager::get()->getPlayerRobot())
        );
        m_player2->updatePlayerRobotFrame(
            ciPlayer1->getNextIconCI(IconType::Robot, GameManager::get()->getPlayerRobot())
        );
        break;
    case 6:
        m_player1->updatePlayerSpiderFrame(
            ciPlayer1->getNextIconCI(IconType::Spider, GameManager::get()->getPlayerSpider())
        );
        m_player2->updatePlayerSpiderFrame(
            ciPlayer1->getNextIconCI(IconType::Spider, GameManager::get()->getPlayerSpider())
        );
        break;
    case 7:
        m_player1->updatePlayerSwingFrame(GameManager::get()->getPlayerSwing());
        m_player2->updatePlayerSwingFrame(GameManager::get()->getPlayerSwing());
        break;
    }
}

std::pair<IconType, CITempProperties> CIPlayLayer::setupCIValues(IconType type) {
    auto gm = GameManager::get();
    auto const& config = CIManager::get()->getConfig(type);
    auto const& globalConfig = CIManager::get()->getGlobalConfig();

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

    auto iconSet = config.iconSet;
    
    if (order == IconOrder::Shuffle && !iconSet.empty())
        std::shuffle(iconSet.begin(), iconSet.end(), Random::mt);

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

    auto const& unlockedIcons = CIManager::get()->getUnlockedIcons(type);
    
    int index;
    if (useAll) {
        switch (order) {
            case IconOrder::Random: [[fallthrough]];
            case IconOrder::Shuffle: [[fallthrough]];
            case IconOrder::Forward:
                if (Mod::get()->getSettingValue<bool>("disable-locked-icons") && !unlockedIcons.empty())
                    index = 0;
                else
                    index = 1;
                break;
            case IconOrder::Backward:
                if (Mod::get()->getSettingValue<bool>("disable-locked-icons") && !unlockedIcons.empty())
                    index = unlockedIcons.size() - 1;
                else
                    index = gm->countForType(type);
                break;
        }
    } else {
        switch (order) {
            case IconOrder::Random: [[fallthrough]];
            case IconOrder::Shuffle: [[fallthrough]];
            case IconOrder::Forward: index = 0; break;
            case IconOrder::Backward: index = iconSet.size() - 1; break;
        }
    }

    auto properties = CITempProperties{
        .current = index,
        .index = index,
        .iconSet = iconSet,
        .order = order,
        .useAll = useAll,
        .mirrorEnd = mirrorEnd,
        .disabled = disabled
    };

    return {type, properties};
}