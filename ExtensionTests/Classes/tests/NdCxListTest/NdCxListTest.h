#ifndef __NDCXLIST_TEST_H__
#define __NDCXLIST_TEST_H__

#include "../testBasic.h"

class NdCxListTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class NdCxListTest : public cocos2d::CCLayer
{
public:
    virtual void onEnter();
    virtual void onExit();
};

#endif // __NDCXLIST_TEST_H__
