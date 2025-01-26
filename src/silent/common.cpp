// common.cpp

#include <cstdlib>
#include <random>

#include "common.hpp"

namespace sl
{
    u64 UUID()
    {
        return rand();
    }

    i32 RandomInt(i32 min, i32 max)
    {
        std::random_device device;
        std::mt19937 rng(device());

        std::uniform_int_distribution dis(min, max);
        return dis(rng);
    }
}
