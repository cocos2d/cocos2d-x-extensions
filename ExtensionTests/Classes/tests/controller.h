#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cocos2d.h"

class TestController : public cocos2d::CCLayer
{
public:
    TestController();
    ~TestController();

    void menuCallback(cocos2d::CCObject * pSender);
    void closeCallback(cocos2d::CCObject * pSender);

    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

private:
    cocos2d::CCPoint m_tBeginPos;
    cocos2d::CCMenu* m_pItmeMenu;
};

#endif
