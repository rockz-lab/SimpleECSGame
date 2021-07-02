#pragma once

#include <cstdint>
#include <bitset>


using eID = int;
using CompType = std::uint16_t;
const CompType maxCompType = 50;
using Signature = std::bitset<maxCompType>;

const eID maxEntities = 50000;

const eID InvalidIndex = -1;