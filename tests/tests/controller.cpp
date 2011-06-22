#include "controller.h"
#include "testResource.h"

#define LINE_SPACE          40

static CCPoint s_tCurPos = CCPointZero;

static ccDeviceOrientation s_eOrientation = CCDeviceOrientationPortrait;
static void ChangeOrientation(ccDeviceOrientation eOrientation)
{
    if (s_eOrientation != eOrientation)
    {
        s_eOrientation = eOrientation;
        CCDirector::sharedDirector()->setDeviceOrientation(eOrientation);
    }
}

static TestScene* CreateTestScene(int nIdx)
{
    CCDirector::sharedDirector()->purgeCachedData();

    TestScene* pScene = NULL;

    switch (nIdx)
    {
    case TEST_NDCX_LIST:
        // pScene = new NdCxListTestScene(); break;
    case TEST_ADVANCE_SPRITE:
        // pScene = new AdvanceSpriteTestScene(); break;
    case TEST_ADVANCE_PARTICLE:
        // pScene = new AdvanceParticleScene(); break;
    default:
        break;
    }

    return pScene;
}

TestController::TestController()
: m_tBeginPos(CCPointZero)
{
    // change to default orientation
    ChangeOrientation(CCDeviceOrientationPortrait);

    // add close menu
    CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(s_pPathClose, s_pPathClose, this, menu_selector(TestController::closeCallback) );
    CCMenu* pMenu =CCMenu::menuWithItems(pCloseItem, NULL);
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    pMenu->setPosition( CCPointZero );
    pCloseItem->setPosition(CCPointMake( s.width - 30, s.height - 30));

    // add menu items for tests
    m_pItmeMenu = CCMenu::menuWithItems(NULL);
    for (int i = 0; i < TESTS_COUNT; ++i)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_AIRPLAY)
		CCLabelBMFont* label = CCLabelBMFont::bitmapFontAtlasWithString(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
#else
        CCLabelTTF* label = CCLabelTTF::labelWithString(g_aTestNames[i].c_str(), "Arial", 24);
#endif		
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(TestController::menuCallback));

        m_pItmeMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( CCPointMake( s.width / 2, (s.height - (i + 1) * LINE_SPACE) ));

        // record the pointer of the menu item
        m_pMenuItems[i] = pMenuItem;
    }

    m_pItmeMenu->setContentSize(CCSizeMake(s.width, (TESTS_COUNT + 1) * (LINE_SPACE)));
    m_pItmeMenu->setPosition(s_tCurPos);
    addChild(m_pItmeMenu);

    setIsTouchEnabled(true);

    addChild(pMenu, 1);
}

TestController::~TestController()
{
}

void TestController::menuCallback(CCObject * pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    // create the test scene and run it
    TestScene* pScene = CreateTestScene(nIdx);
    if (pScene)
    {
        pScene->runThisTest();
        pScene->release();
    }
}

void TestController::closeCallback(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();
}

void TestController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->locationInView( touch->view() );	
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
}

void TestController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    float nMoveY = touchLocation.y - m_tBeginPos.y;

    CCPoint curPos  = m_pItmeMenu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (nextPos.y < 0.0f)
    {
        m_pItmeMenu->setPosition(CCPointZero);
        return;
    }

    if (nextPos.y > ((TESTS_COUNT + 1)* LINE_SPACE - winSize.height))
    {
        m_pItmeMenu->setPosition(ccp(0, ((TESTS_COUNT + 1)* LINE_SPACE - winSize.height)));
        return;
    }

    m_pItmeMenu->setPosition(nextPos);
    m_tBeginPos = touchLocation;
    s_tCurPos   = nextPos;
}
