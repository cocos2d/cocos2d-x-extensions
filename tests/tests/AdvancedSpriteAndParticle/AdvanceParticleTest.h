#ifndef _PARTICLE_TEST_H_
#define _PARTICLE_TEST_H_

#include "../testBasic.h"

class ParticleTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class ParticleAnimation : public ParticleDemo
{
public:
    virtual void onEnter();
	virtual std::string title();
};

#endif
