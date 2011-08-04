#ifndef __CCSCROLLLAYER__
#define __CCSCROLLLAYER__

#define SCROLLTESTCODE

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

#include <cocos2d.h>

namespace cocos2d {
/* 
 It is a very clean and elegant subclass of CCLayer that lets you pass-in an array 
 of layers and it will then create a smooth scroller. 
 Complete with the "snapping" effect. You can create screens with anything that can be added to a CCLayer.
 
 */ 


class CCScrollLayer : 	public CCLayer
{
	// Holds the current width of the screen substracting offset.
	float scrollWidth_;

	// The x coord of initial point the user starts their swipe.
	float startSwipe_;

	// Internal state of scrollLayer (scrolling or idle).
	int state_; 
public:
	static CCScrollLayer*  nodeWithLayers(CCArray *layers , int widthOffset); 
	CCScrollLayer(CCArray* layers, int widthOffset);
	// Holds the current page being displayed.
	CC_SYNTHESIZE(int, currentScreen_, CurrentScreen);

	// A count of the total screens available.
	CC_SYNTHESIZE(int, totalScreens_, TotalScreens);

	// For what distance user must slide finger to start scrolling menu.
	CC_SYNTHESIZE(float, minimumTouchLengthToSlide_, MinimumTouchLengthToSlide); 

	// For what distance user must slide finger to change the page.
	CC_SYNTHESIZE(float, minimumTouchLengthToChangePage_, MinimumTouchLengthToChangePage); 

	// Whenever show or not gray/white dots under scrolling content.
	CC_SYNTHESIZE(bool, showPagesIndicator_, ShowPagesIndicator);

protected:
	void claimTouch(CCTouch * aTouch);
	void cancelAndStoleTouch(CCTouch *touch , CCEvent * event);
	bool ccTouchBegan(CCTouch * touch, CCEvent * event);
	void ccTouchMoved(CCTouch * touch , CCEvent * event);
	void ccTouchEnded(CCTouch * touch , CCEvent * event);
	void registerWithTouchDispatcher();
	void moveToPage(int page);
	void visit();
};

#ifdef SCROLLTESTCODE
void addScrollTest(cocos2d::CCLayer* l, cocos2d::SelectorProtocol* target, cocos2d::SEL_MenuHandler selector);
#endif

}

#endif
