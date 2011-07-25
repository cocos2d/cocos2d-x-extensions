#ifndef _PARTICLE_TEST_H_
#define _PARTICLE_TEST_H_

#include "../testBasic.h"

class AdvanceParticleTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class ParticleDemo : public cocos2d::CCLayerColor
{
protected:
    cocos2d::CCParticleSystem*	m_emitter;
    cocos2d::CCSprite*			m_background;

public:
    ParticleDemo(void);
    ~ParticleDemo(void);

    virtual void onEnter(void);

    virtual std::string title();

    void restartCallback(cocos2d::CCObject* pSender);
    void nextCallback(cocos2d::CCObject* pSender);
    void backCallback(cocos2d::CCObject* pSender);
    void toggleCallback(cocos2d::CCObject* pSender);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    void step(cocos2d::ccTime dt);
    void setEmitterPosition();
};

class ParticleAnimation : public ParticleDemo
{
public:
    virtual void onEnter();
	virtual std::string title();
};

#endif
