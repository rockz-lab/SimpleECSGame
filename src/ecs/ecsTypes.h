#pragma once

#include <cstdint>
#include <bitset>


using eID = std::uint32_t;
using CompType = std::uint16_t;
const CompType maxCompType = 100;
using Signature = std::bitset<maxCompType>;

const eID maxEntities = 10000;