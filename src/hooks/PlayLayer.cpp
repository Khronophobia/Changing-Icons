#include "PlayLayer.hpp"
#include "PlayerObject.hpp"
#include <class/CIConfigManager.hpp>
#include <Random.hpp>

using namespace geode::prelude;
using namespace changing_icons;

$override
void CIPlayLayer::setupHasCompleted() {
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
    auto dualMode = m_levelSettings->m_startDual;
    switch (startMode) {
    default: break;
    case 0:
        m_player1->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        if (dualMode)
            m_player2->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        break;
    case 1:
        m_player1->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        m_player2->updatePlayerFrame(GameManager::get()->getPlayerFrame());

        if (m_player1->m_isPlatformer)
            m_player1->updatePlayerJetpackFrame(GameManager::get()->getPlayerJetpack());
        else
            m_player1->updatePlayerShipFrame(GameManager::get()->getPlayerShip());
        
        if (dualMode) {
            if (m_player2->m_isPlatformer)
                m_player2->updatePlayerJetpackFrame(GameManager::get()->getPlayerJetpack());
            else
                m_player2->updatePlayerShipFrame(GameManager::get()->getPlayerShip());
        }

        break;
    case 2:
        m_player1->updatePlayerRollFrame(GameManager::get()->getPlayerBall());
        if (dualMode)
            m_player2->updatePlayerRollFrame(GameManager::get()->getPlayerBall());
        break;
    case 3:
        m_player1->updatePlayerFrame(GameManager::get()->getPlayerFrame());
        m_player1->updatePlayerBirdFrame(GameManager::get()->getPlayerBird());
        if (dualMode) {
            m_player2->updatePlayerFrame(GameManager::get()->getPlayerFrame());
            m_player2->updatePlayerBirdFrame(GameManager::get()->getPlayerBird());
        }
        break;
    case 4:
        m_player1->updatePlayerDartFrame(GameManager::get()->getPlayerDart());
        if (dualMode)
            m_player2->updatePlayerDartFrame(GameManager::get()->getPlayerDart());
        break;
    case 5:
        m_player1->updatePlayerRobotFrame(
            ciPlayer1->getNextIconCI(IconType::Robot, GameManager::get()->getPlayerRobot())
        );
        m_player1->updateGlowColor();
        if (dualMode) {
            m_player2->updatePlayerRobotFrame(
                ciPlayer2->getNextIconCI(IconType::Robot, GameManager::get()->getPlayerRobot())
            );
            m_player2->updateGlowColor();
        }
        break;
    case 6:
        m_player1->updatePlayerSpiderFrame(
            ciPlayer1->getNextIconCI(IconType::Spider, GameManager::get()->getPlayerSpider())
        );
        m_player1->updateGlowColor();
        if (dualMode) {
            m_player2->updatePlayerSpiderFrame(
                ciPlayer2->getNextIconCI(IconType::Spider, GameManager::get()->getPlayerSpider())
            );
            m_player2->updateGlowColor();
        }
        break;
    case 7:
        m_player1->updatePlayerSwingFrame(GameManager::get()->getPlayerSwing());
        if (dualMode)
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

    std::vector<IconProperties> iconSet;
    if (useAll)
        iconSet = CIManager::get()->generateIconKitList(type);
    else
        iconSet = config.iconSet;
    
    if (order == IconOrder::Shuffle && !iconSet.empty())
        std::shuffle(iconSet.begin(), iconSet.end(), Random::mt);

    if (includePlayerIcon && !iconSet.empty() && !useAll) {
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
    
    int index;
    switch (order) {
        case IconOrder::Random: [[fallthrough]];
        case IconOrder::Shuffle: [[fallthrough]];
        case IconOrder::Forward: index = 0; break;
        case IconOrder::Backward: index = iconSet.size() - 1; break;
    }

    auto properties = CITempProperties{
        .current = index,
        .index = index,
        .iconSet = iconSet,
        .order = order,
        .mirrorEnd = mirrorEnd,
        .disabled = disabled
    };

    return {type, properties};
}