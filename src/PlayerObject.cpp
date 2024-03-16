#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(ModPlayerObject, PlayerObject) {
    void toggleFlyMode(bool p0, bool p1) {
        PlayerObject::toggleFlyMode(p0, p1);
    }
    void toggleRollMode(bool p0, bool p1) {
        PlayerObject::toggleRollMode(p0, p1);
    }
    void toggleBirdMode(bool p0, bool p1) {
        PlayerObject::toggleBirdMode(p0, p1);
    }
    void toggleDartMode(bool p0, bool p1) {
        PlayerObject::toggleDartMode(p0, p1);
    }
    void toggleRobotMode(bool p0, bool p1) {
        PlayerObject::toggleRobotMode(p0, p1);
    }
    void toggleSpiderMode(bool p0, bool p1) {
        PlayerObject::toggleSpiderMode(p0, p1);
    }
    void toggleSwingMode(bool p0, bool p1) {
        PlayerObject::toggleSwingMode(p0, p1);
    }
};