#include "CCScrollLayer.h"
//#include "CCTouchHandler.h"
//#include "GLES-Render.h"


#include <assert.h>

USING_NS_CC;

enum
{
  kCCScrollLayerStateIdle,
  kCCScrollLayerStateSliding,
};

CCScrollLayer::CCScrollLayer()
: m_pDelegate(NULL), m_pLayers(NULL), m_bStealingTouchInProgress(false), m_pScrollTouch(NULL), m_iState(kCCScrollLayerStateIdle)

{
}

CCScrollLayer::~CCScrollLayer()
{
  CC_SAFE_RELEASE(m_pLayers);
  m_pDelegate = NULL;
  this->setCustomPageIndicators(NULL);
}

unsigned int CCScrollLayer::getTotalScreens() const
{
  return m_pLayers->count();
}

CCScrollLayer* CCScrollLayer::nodeWithLayers(CCArray* layers, int widthOffset)
{
  CCScrollLayer* pRet = new CCScrollLayer();
  if (pRet && pRet->initWithLayers(layers, widthOffset))
  {
    pRet->autorelease();
    return pRet;
  }
  else
  {
    CC_SAFE_DELETE(pRet);
    return NULL;
  }
}

CCScrollLayer* CCScrollLayer::nodeWithLayers(cocos2d::CCArray *layers, int widthOffset, const char * pageSpriterameName)
{
  CCScrollLayer* layer = CCScrollLayer::nodeWithLayers(layers, widthOffset);
  if (layers == NULL) {
    CCLog("null");
  }
  layer->setCustomPageIndicators(CCNode::create());
  
  // use default position
  layer->customPageIndicators->setPosition(layer->getPagesIndicatorPosition());
  
  // create sprites
  float n = (float)layer->getTotalScreens(); // << total point
  float d = 37.78f;    //<< distance between points
  for (int i = 0; i < layer->getTotalScreens(); ++i)
  {
    CCSprite *pageSprite = CCSprite::create(pageSpriterameName);
    pageSprite->setPosition(ccp(d*((float)i - 0.5f*(n-1.0f)), 0));
    layer->customPageIndicators->addChild(pageSprite);
  }
  layer->addChild(layer->getCustomPageIndicators());
  layer->schedule(schedule_selector(CCScrollLayer::updatePageIndicators));
  
  return layer;
}

CCScrollLayer* CCScrollLayer::nodeWithLayers(cocos2d::CCArray *layers, int widthOffset, float duration, float bouncePeriod)
{
  CCScrollLayer *layer = CCScrollLayer::nodeWithLayers(layers, widthOffset);
  layer->setActionDuration(duration);
  layer->setBouncePeriod(bouncePeriod);
  return layer;
}

bool CCScrollLayer::initWithLayers(CCArray* layers, int widthOffset)
{
  if (!CCLayer::init())
    return false;
  CC_ASSERT(layers && layers->count());
  
  setTouchEnabled(true);
  
  m_bStealTouches = true;
  
  // Set default minimum touch length to scroll.
  m_fMinimumTouchLengthToSlide = 30.0f;
  m_fMinimumTouchLengthToChangePage = 100.0f;
  
  m_fMarginOffset = CCDirector::sharedDirector()->getWinSize().width;
  
  // Show indicator by default.
  m_bShowPagesIndicator = true;
  m_tPagesIndicatorPosition = ccp(0.5f * m_obContentSize.width, ceilf(m_obContentSize.height / 8.0f));
  
  // Set up the starting variables
  m_uCurrentScreen = 0;
  
  // Save offset.
  m_fPagesWidthOffset = (float)widthOffset;
  
  // Save array of layers.
  //		m_pLayers = CCArray::createWithArray(layers);
  m_pLayers = CCArray::createWithCapacity(layers->count());
  m_pLayers->addObjectsFromArray(layers);
  
  layers->release();
  m_pLayers->retain();
  
  updatePages();
  
  return true;
}

void CCScrollLayer::updatePages()
{
  // Loop through the array and add the screens if needed.
  int i = 0;
  CCObject* object = NULL;
  CCARRAY_FOREACH(m_pLayers, object)
  {
    CCLayer* layer = (CCLayer*)object;
    layer->setAnchorPoint(ccp(0,0));
    layer->setContentSize(CCDirector::sharedDirector()->getWinSize());
    layer->setPosition(ccp((i * (m_obContentSize.width - m_fPagesWidthOffset)), 0));
    if (!layer->getParent())
      addChild(layer);
    i++;
  }
}

// CCLayer Methods ReImpl

void CCScrollLayer::visit()
{
  CCLayer::visit();	//< Will draw after glPopScene.
  
  if (m_bShowPagesIndicator)
  {
    int totalScreens = getTotalScreens();
    
    // Prepare Points Array
    float n = (float)totalScreens; //< Total points count in float.
    float pY = m_tPagesIndicatorPosition.y; //< Points y-coord in parent coord sys.
    float d = 16.0f; //< Distance between points.
    CCPoint* points = new CCPoint[totalScreens];
    for (int i = 0; i < totalScreens; ++i)
    {
      float pX = m_tPagesIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
      points[i] = ccp(pX, pY);
    }
    
    // Set GL Values
    //			glEnable(GL_POINT_SMOOTH);
    GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ccPointSize(6.0f * CC_CONTENT_SCALE_FACTOR());
    
    // Draw Gray Points
    //			ccDrawColor4B(0x96,0x96,0x96,0xFF);
    //			ccDrawPoints( points, totalScreens );
    
    // Draw White Point for Selected Page
    //			ccDrawColor4B(0xFF,0xFF,0xFF,0xFF);
    //			ccDrawPoint(points[m_uCurrentScreen]);
    
    // Restore GL Values
    ccPointSize(3.0f);
    //			glDisable(GL_POINT_SMOOTH);
    if (! blendWasEnabled)
      glDisable(GL_BLEND);
    
    delete [] points;
  }
}

// Moving To / Selecting Pages

void CCScrollLayer::moveToPageEnded()
{
  if (m_pDelegate)
    m_pDelegate->scrollLayerScrolledToPageNumber(this, m_uCurrentScreen);
}

unsigned int CCScrollLayer::pageNumberForPosition(const CCPoint& position)
{
  float pageFloat = - m_obPosition.x / (m_obContentSize.width - m_fPagesWidthOffset);
  int pageNumber = (int)ceilf(pageFloat);
  if ((float)pageNumber - pageFloat  >= 0.5f)
    pageNumber--;
  
  pageNumber = MAX(0, pageNumber);
  pageNumber = MIN((int)m_pLayers->count() - 1, pageNumber);
  
  return (unsigned int)pageNumber;
}


CCPoint CCScrollLayer::positionForPageWithNumber(unsigned int pageNumber)
{
  return ccp(pageNumber * -1.f * (m_obContentSize.width - m_fPagesWidthOffset), 0.0f);
}

void CCScrollLayer::moveToPage(unsigned int pageNumber)
{
  if (pageNumber >= m_pLayers->count())
  {
    CCLOGERROR("CCScrollLayer::moveToPage: %d - wrong page number, out of bounds.", pageNumber);
    return;
  }
  
  actionDuration = 1.0f;
  bouncePeriod = 0.7f;
  CCAction* changePage =
  CCSequence::create(
                     
                     CCCallFunc::create(this, callfunc_selector(CCScrollLayer::moveToPageEnded)),
                     CCEaseElasticOut::create(
                                              CCMoveTo::create(actionDuration, positionForPageWithNumber(pageNumber)), bouncePeriod),
                     NULL);
  
  runAction(changePage);
  m_uCurrentScreen = pageNumber;
}

void CCScrollLayer::selectPage(unsigned int pageNumber)
{
  if (pageNumber >= m_pLayers->count())
  {
    CCLOGERROR("CCScrollLayer::selectPage: %d - wrong page number, out of bounds.", pageNumber);
    return;
  }
  
  setPosition(positionForPageWithNumber(pageNumber));
  m_uCurrentScreen = pageNumber;
  
}

// Dynamic Pages Control

void CCScrollLayer::addPage(CCLayer* aPage)
{
  addPage(aPage, m_pLayers->count());
}

void CCScrollLayer::addPage(CCLayer* aPage, unsigned int pageNumber)
{
  pageNumber = MIN(pageNumber, m_pLayers->count());
  pageNumber = MAX(pageNumber, 0);
  
  m_pLayers->insertObject(aPage, pageNumber);
  
  updatePages();
  moveToPage(m_uCurrentScreen);
}

void CCScrollLayer::removePage(CCLayer* aPage)
{
  m_pLayers->removeObject(aPage);
  removeChild(aPage, true);
  
  updatePages();
  
  m_uCurrentScreen = MIN(m_uCurrentScreen, m_pLayers->count() - 1);
  moveToPage(m_uCurrentScreen);
}

void CCScrollLayer::removePageWithNumber(unsigned int pageNumber)
{
  if (pageNumber < m_pLayers->count())
    removePage((CCLayer*)(m_pLayers->objectAtIndex(pageNumber)));
}

// Touches

// Register with more priority than CCMenu's but don't swallow touches
void CCScrollLayer::registerWithTouchDispatcher()
{
  CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, false);
}

/** Hackish stuff - stole touches from other CCTouchDispatcher targeted delegates.
 Used to claim touch without receiving ccTouchBegan. */
void CCScrollLayer::claimTouch(CCTouch* pTouch)
{
  CCTargetedTouchHandler* handler = (CCTargetedTouchHandler*)CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this);
  if (handler)
  {
    CCSet* claimedTouches = handler->getClaimedTouches();
    if (!claimedTouches->containsObject(pTouch))
    {
      claimedTouches->addObject(pTouch);
    }
    else
    {
      CCLOGERROR("CCScrollLayer::claimTouch is already claimed!");
    }
  }
}

void CCScrollLayer::cancelAndStoleTouch(CCTouch* pTouch, CCEvent* pEvent)
{
  // Throw Cancel message for everybody in TouchDispatcher.
  CCSet* touchSet = new CCSet();
  touchSet->addObject(pTouch);
  touchSet->autorelease();
  m_bStealingTouchInProgress = true;
  CCDirector::sharedDirector()->getTouchDispatcher()->touchesCancelled(touchSet, pEvent);
  m_bStealingTouchInProgress = false;
  
  //< after doing this touch is already removed from all targeted handlers
  
  // Squirrel away the touch
  claimTouch(pTouch);
}

void CCScrollLayer::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
{
  // Do not cancel touch, if this method is called from cancelAndStoleTouch:
  if (m_bStealingTouchInProgress)
    return;
  
  if (m_pScrollTouch == pTouch)
  {
    m_pScrollTouch = NULL;
    selectPage(m_uCurrentScreen);
  }
}

bool CCScrollLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
  if (!m_pScrollTouch)
    m_pScrollTouch = pTouch;
  else
    return false;
  
  CCPoint touchPoint = pTouch->getLocationInView();
  touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
  
  m_fStartSwipe = touchPoint.x;
  m_iState = kCCScrollLayerStateIdle;
  
  return true;
}

void CCScrollLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
  if(m_pScrollTouch != pTouch)
    return;
  
  CCPoint touchPoint = pTouch->getLocationInView();
  touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
  
  // If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
  // Of course only if we not already in sliding mode
  if ((m_iState != kCCScrollLayerStateSliding)
			&& (fabsf(touchPoint.x - m_fStartSwipe) >= m_fMinimumTouchLengthToSlide))
  {
    m_iState = kCCScrollLayerStateSliding;
    
    // Avoid jerk after state change.
    m_fStartSwipe = touchPoint.x;
    
    if (m_bStealTouches)
      cancelAndStoleTouch(pTouch, pEvent);
    
    if (m_pDelegate)
      m_pDelegate->scrollLayerScrollingStarted(this);
  }
  
  if (m_iState == kCCScrollLayerStateSliding)
  {
    float desiredX = (m_uCurrentScreen * -1.f * (m_obContentSize.width - m_fPagesWidthOffset)) + touchPoint.x - m_fStartSwipe;
    unsigned int page = pageNumberForPosition(ccp(desiredX, 0));
    float offset = desiredX - positionForPageWithNumber(page).x;
    if ((page == 0 && offset > 0) || (page == m_pLayers->count() - 1 && offset < 0))
      offset -= m_fMarginOffset * (offset / CCDirector::sharedDirector()->getWinSize().width);
    else
      offset = 0;
    setPosition(ccp(desiredX - offset, 0));
  }
}

void CCScrollLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
  if(m_pScrollTouch != pTouch)
    return;
  
  m_pScrollTouch = NULL;
  
  CCPoint touchPoint = pTouch->getLocationInView();
  touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
  
  unsigned int selectedPage = m_uCurrentScreen;
  float delta = touchPoint.x - m_fStartSwipe;
  if (fabs(delta) >= m_fMinimumTouchLengthToChangePage)
  {
    selectedPage = pageNumberForPosition(m_obPosition);
    if (selectedPage == m_uCurrentScreen)
    {
      if (delta < 0.f && selectedPage < m_pLayers->count() - 1)
        selectedPage++;
      else if (delta > 0.f && selectedPage > 0)
        selectedPage--;
    }
  }
  moveToPage(selectedPage);
}

void CCScrollLayer::updatePageIndicators()
{
  customPageIndicators->setPosition(ccp(getPagesIndicatorPosition().x - getPosition().x, getPagesIndicatorPosition().y));
  for (int i = 0; i < customPageIndicators->getChildren()->count(); i++)
  {
    CCSprite *indicator = (CCSprite*)customPageIndicators->getChildren()->objectAtIndex(i);
    
    // none current page are semi-transparent
    if (i == this->getCurrentScreen())
    {
      indicator->setOpacity(255);
    } else
    {
      indicator->setOpacity(100);
    }
  }
}
