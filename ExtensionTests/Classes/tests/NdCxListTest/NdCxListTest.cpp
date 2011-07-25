#include "NdCxListTest.h"
#include "NdControls/NdCxList.h"

using namespace cocos2d;
using namespace NdCxControl;

struct MyItemListener : public NdCxListItemClickListener
{
    virtual void onClick(int index, NdCxListItem *item)
    {
        CCLog("item %d clicked!", index);
    }
};

static MyItemListener* s_listener = NULL;

void NdCxListTest::onEnter()
{
    CCLayer::onEnter();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ccColor4B bg_color = ccc4(230,232,250,255);
    CCSize lsSize = CCSizeMake(winSize.width, winSize.height - (40*2));
    NdCxList* listview = new NdCxList(30, bg_color, lsSize);
    listview->autorelease();

    CCPoint lvPos = CCPointMake(0, 10);

    ccColor3B itemColor1 = ccc3(0xE8,0xE8,0xE8);
    NdCxListItem* lvitem = NdCxListItem::itemWithColor(itemColor1);
    listview->addChild(lvitem, false);
    lvitem = NdCxListItem::itemWithColor(itemColor1);
    listview->addChild(lvitem, false);
    lvitem = NdCxListItem::itemWithColor(itemColor1);
    listview->addChild(lvitem, false);

    s_listener = new MyItemListener();
    listview->registerItemClickListener(s_listener);
    listview->setPosition(lvPos);
    addChild(listview);
}

void NdCxListTest::onExit()
{
    removeAllChildrenWithCleanup(true);
    CC_SAFE_DELETE(s_listener);
}

void NdCxListTestScene::runThisTest()
{
    CCLayer* pLayer = new NdCxListTest();
    addChild(pLayer);

    CCDirector::sharedDirector()->replaceScene(this);
}
