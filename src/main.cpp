#include <class/CIManager.hpp>

$on_mod(Loaded) {
    // Instantiate the manager
    ci::CIManager::get();
}

$on_mod(DataSaved) {
    ci::CIManager::get()->saveData();
}