#include "CIUtilities.hpp"
#include <random>

int changing_icons::utils::randomIntInRange(int min, int max) {
    std::random_device randdev;
    std::mt19937 rng(randdev());
    std::uniform_int_distribution<> range(min, max);
    return range(rng);
}