#include "PlayLayer.hpp"
#include "PlayerObject.hpp"
#include <class/CIConfigManager.hpp>

using namespace geode::prelude;
using namespace changing_icons;

void CIPlayLayer::setupHasCompleted() {
    if (Mod::get()->getSettingValue<bool>("disable-locked-icons")) {
        for (auto const& pair : CIManager::get()->getConfigMap()) {
            CIManager::get()->refreshUnlockedIcons(pair.first);
        }
        log::info("Unlocked icons refreshed");
    }

    PlayLayer::setupHasCompleted();

    static_cast<CIPlayerObject*>(m_player1)->m_fields->m_levelStarted = true;
    static_cast<CIPlayerObject*>(m_player2)->m_fields->m_levelStarted = true;

    auto startMode = m_levelSettings->m_startMode;
    auto ciPlayer1 = static_cast<CIPlayerObject*>(m_player1);
    auto ciPlayer2 = static_cast<CIPlayerObject*>(m_player2);
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