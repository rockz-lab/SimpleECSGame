#pragma once

#include <cstdint>
#include <bitset>


using eID = std::uint16_t;
using CompType = std::uint16_t;
const CompType maxCompType = 50;
using Signature = std::bitset<maxCompType>;

const eID maxEntities = 1000;