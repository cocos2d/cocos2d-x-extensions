#ifndef __CCSCROLLLAYER__
#define __CCSCROLLLAYER__
//  CCScrollLayer.h
//
//  Copyright 2010 DK101
//  http://dk101.net/2010/11/30/implementing-page-scrolling-in-cocos2d/
//
//  Copyright 2010 Giv Parvaneh.
//  http://www.givp.org/blog/2010/12/30/scrolling-menus-in-cocos2d/
//
//  Copyright 2011 Stepan Generalov
//
//  Copyright 2011 Eli Yukelzon
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

// Original source: https://github.com/cocos2d/cocos2d-iphone-extensions/tree/master/Extensions/CCScrollLayer
// Last updated: October 1, 2011

// Last modified by Giap Nguyen on Feb 22, 2014

#include "cocos2d.h"

class CCScrollLayer;

class CCScrollLayerDelegate
{
public:
  /** Called when scroll layer begins scrolling.
   * Usefull to cancel CCTouchDispatcher standardDelegates.
   */
  virtual void scrollLayerScrollingStarted(CCScrollLayer* sender) {}
  
  /** Called at the end of moveToPage:
   * Doesn't get called in selectPage:
   */
  virtual void scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page) {}
};

/*
 It is a very clean and elegant subclass of CCLayer that lets you pass-in an array
 of layers and it will then create a smooth scroller.
 Complete with the "snapping" effect. You can create screens with anything that can be added to a CCLayer.
 
 */
class CCScrollLayer : 	public cocos2d::CCLayer
{
public:
  CCScrollLayer();
  ~CCScrollLayer();
  
  static CCScrollLayer* nodeWithLayers(cocos2d::CCArray* layers, int widthOffset);
  static CCScrollLayer* nodeWithLayers(cocos2d::CCArray* layers, int widthOffset, const char* pageSpriterameName);
  
  bool initWithLayers(cocos2d::CCArray* layers, int widthOffset);
  
  /** Updates all pages positions & adds them as children if needed.
   * Can be used to update position of pages after screen reshape, or
   * for update after dynamic page add/remove.
   */
  void updatePages();
  
  /** Adds new page and reorders pages trying to set given number for newly added page.
   * If number > pages count - adds new page to the right end of the scroll layer.
   * If number <= 0 - adds new page to the left end of the scroll layer.
   * @attention Designated addPage method.
   */
  void addPage(cocos2d::CCLayer* aPage, unsigned int pageNumber);
  
  /** Adds new page to the right end of the scroll layer. */
  void addPage(cocos2d::CCLayer* aPage);
  
  /** Removes page if it's one of scroll layers pages (not children)
   * Does nothing if page not found.
   */
  void removePage(cocos2d::CCLayer* aPage);
  
  /** Removes page with given number. Doesn nothing if there's no page for such number. */
  void removePageWithNumber(unsigned int pageNumber);
  
  /* Moves scrollLayer to page with given number & invokes delegate
   * method scrollLayer:scrolledToPageNumber: at the end of CCMoveTo action.
   * Does nothing if number >= totalScreens or < 0.
   */
  void moveToPage(unsigned int pageNumber);
  
  /* Immedeatly moves scrollLayer to page with given number without running CCMoveTo.
   * Does nothing if number >= totalScreens or < 0.
   */
  void selectPage(unsigned int pageNumber);
  
  CC_SYNTHESIZE(CCScrollLayerDelegate*, m_pDelegate, Delegate);
  
  /** Calibration property. Minimum moving touch length that is enough
   * to cancel menu items and start scrolling a layer.
   */
  CC_SYNTHESIZE(float, m_fMinimumTouchLengthToSlide, MinimumTouchLengthToSlide);
  
  /** Calibration property. Minimum moving touch length that is enough to change
   * the page, without snapping back to the previous selected page.
   */
  CC_SYNTHESIZE(float, m_fMinimumTouchLengthToChangePage, MinimumTouchLengthToChangePage);
  
  /** If YES - when starting scrolling CCScrollLayer will claim touches, that are
   * already claimed by others targetedTouchDelegates by calling CCTouchDispatcher#touchesCancelled
   * Usefull to have ability to scroll with touch above menus in pages.
   * If NO - scrolling will start, but no touches will be cancelled.
   * Default is YES.
   */
  CC_SYNTHESIZE(bool, m_bStealTouches, StealTouches);
  
  /** Whenever show or not white/grey dots under the scroll layer.
   * If yes - dots will be rendered in parents transform (rendered after scroller visit).
   */
  CC_SYNTHESIZE(bool, m_bShowPagesIndicator, ShowPagesIndicator);
  
  /** Position of dots center in parent coordinates.
   * (Default value is screenWidth/2, screenHeight/4)
   */
  CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCPoint, m_tPagesIndicatorPosition, PagesIndicatorPosition);
  
  /** Total pages available in scrollLayer. */
  unsigned int getTotalScreens() const;
  
  /** Current page number, that is shown. Belongs to the [0, totalScreen] interval. */
  CC_SYNTHESIZE_READONLY(unsigned int, m_uCurrentScreen, CurrentScreen);
  
  /** Offset, that can be used to let user see next/previous page. */
  CC_SYNTHESIZE(float, m_fPagesWidthOffset, PagesWidthOffset);
  
  /** Offset that can be used to let user see empty space over first or last page. */
  CC_SYNTHESIZE(float, m_fMarginOffset, MarginOffset);
  
  /** Array of pages CCLayer's  */
  CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pLayers, Pages);
  
  CC_SYNTHESIZE(float, actionDuration, ActionDuration);
  CC_SYNTHESIZE(float, bouncePeriod, BouncePeriod);
  CC_SYNTHESIZE(cocos2d::CCNode*, customPageIndicators, CustomPageIndicators);
protected:
  // The x coord of initial point the user starts their swipe.
  float m_fStartSwipe;
  
  // Internal state of scrollLayer (scrolling or idle).
  int m_iState;
  bool m_bStealingTouchInProgress;
  // Holds the touch that started the scroll
  cocos2d::CCTouch* m_pScrollTouch;
  
  void visit();
  void moveToPageEnded();
  unsigned int pageNumberForPosition(const cocos2d::CCPoint& position);
  cocos2d::CCPoint positionForPageWithNumber(unsigned int pageNumber);
  void claimTouch(cocos2d::CCTouch* pTouch);
  void cancelAndStoleTouch(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
  
  void registerWithTouchDispatcher();
  bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
  void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
  void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
  void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
  
  void updatePageIndicators();
};

#endif
