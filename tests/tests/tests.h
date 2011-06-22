#ifndef _TESTS_H_
#define _TESTS_H_

#include "AdvanceSpriteAndParticle/AdvanceParticleTest.h"

enum
{
    TEST_NDCX_LIST = 0,
    TEST_ADVANCE_SPRITE,
    TEST_ADVANCE_PARTICLE,
    TESTS_COUNT,
};

const std::string g_aTestNames[TESTS_COUNT] = {
    "NdCxListTest",
    "AdvanceSpriteTest",
    "AdvanceParticleTest",
};

#endif
