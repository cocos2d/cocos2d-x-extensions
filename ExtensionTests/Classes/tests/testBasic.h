#ifndef _TEST_BASIC_H_
#define _TEST_BASIC_H_

#include "cocos2d.h"

class TestScene : public cocos2d::CCScene
{
public:
    TestScene();
    virtual void onEnter();

    // The CallBack for back to the main menu scene
    virtual void MainMenuCallback(cocos2d::CCObject* pSender);
    virtual void runThisTest() = 0;
};

#endif
