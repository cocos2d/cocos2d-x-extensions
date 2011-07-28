#include "CCScrollLayer.h"

#include <assert.h>

namespace cocos2d {
	enum 
	{
		kCCScrollLayerStateIdle,
		kCCScrollLayerStateSliding,
	}; 

	CCScrollLayer*  CCScrollLayer::nodeWithLayers(CCArray *layers , int widthOffset) {
		CCScrollLayer *pRet = new CCScrollLayer(layers, widthOffset);
		pRet->autorelease();
		return pRet;

	}

	CCScrollLayer::CCScrollLayer(CCArray* layers, int widthOffset) {
		assert(layers->count()); // "CCScrollLayer#initWithLayers:widthOffset: you must provide at least one layer!"

		// Enable touches.
		setIsTouchEnabled(true);
		// Set default minimum touch length to scroll.
		setMinimumTouchLengthToChangePage(100.0f);
		setMinimumTouchLengthToSlide(30.0f);
		

		// Show indicator by default.
		setShowPagesIndicator(true);

		// Set up the starting variables
		currentScreen_ = 1;

		// offset added to show preview of next/previous screens
		scrollWidth_ = CCDirector::sharedDirector()->getWinSize().width - widthOffset;

		// Loop through the array and add the screens
		
		for (size_t i=0;i<layers->count();i++)
		{
			CCLayer *l = (CCLayer *)layers->objectAtIndex(i);
			l->setAnchorPoint (ccp(0,0));
			l->setPosition ( ccp((i*scrollWidth_),0));
			addChild(l);
		}

		// Setup a count of the available screens
		totalScreens_ = layers->count(); 
	}

	// Register with more priority than CCMenu's but don't swallow touches
	void CCScrollLayer::registerWithTouchDispatcher()
	{	
		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 1, false);
	}

	void CCScrollLayer::visit()
	{
		CCLayer::visit();//< Will draw after glPopScene. 

		if (showPagesIndicator_)
		{
			// Prepare Points Array
			CGFloat n = (CGFloat)totalScreens_; //< Total points count in CGFloat.
			CGFloat pY = ceilf ( getContentSize().height / 8.0f ); //< Points y-coord in parent coord sys.
			CGFloat d = 16.0f; //< Distance between points.
			CCPoint* points = (CCPoint*)alloca(totalScreens_ * sizeof(CCPoint));	
			for (int i=0; i < totalScreens_; ++i)
			{
				CGFloat pX = 0.5f * getContentSize().width + d * ( (CGFloat)i - 0.5f*(n-1.0f) );
				points[i] = ccp (pX, pY);
			}

			// Set GL Values
			glEnable(GL_POINT_SMOOTH);
			GLboolean blendWasEnabled = glIsEnabled( GL_BLEND );
			glEnable(GL_BLEND);
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glPointSize( 6.0f * CC_CONTENT_SCALE_FACTOR() );

			// Draw Gray Points
			glColor4ub(0x96,0x96,0x96,0xFF);
			ccDrawPoints( points, totalScreens_ );

			// Draw White Point for Selected Page
			glColor4ub(0xFF,0xFF,0xFF,0xFF);
			ccDrawPoint(points[currentScreen_ - 1]);

			// Restore GL Values
			glPointSize(1.0f);
			glDisable(GL_POINT_SMOOTH);
			if (! blendWasEnabled)
				glDisable(GL_BLEND);
		}
	}

// Pages Control 

	void CCScrollLayer::moveToPage(int page)
	{
		CCMoveTo* changePage = CCMoveTo::actionWithDuration(0.3f,  ccp(-((page-1)*scrollWidth_),0));
		runAction(changePage);
		currentScreen_ = page;
	}

// Hackish Stuff

	void CCScrollLayer::claimTouch(CCTouch * aTouch)
	{
		CCTouchHandler* handler = CCTouchDispatcher::sharedDispatcher()->findHandler(this);
		
		if(handler){
			CCTargetedTouchHandler* targetedHandler = dynamic_cast<CCTargetedTouchHandler*>(handler);
			if(targetedHandler){
				if (!targetedHandler->getClaimedTouches()->containsObject(aTouch))
				{
					targetedHandler->getClaimedTouches()->addObject(aTouch);
				}
				else 
				{
					CCLOGERROR("CCScrollLayer#claimTouch is already claimed!");
				}
			}
		}
	}

	void CCScrollLayer::cancelAndStoleTouch(CCTouch *touch , CCEvent * event)
	{
		// Throw Cancel message for everybody in TouchDispatcher.
		CCSet* touchSet = new CCSet();
		touchSet->addObject(touch);
		touchSet->autorelease();
		CCTouchDispatcher::sharedDispatcher()->touchesCancelled(touchSet,event);

		//< after doing this touch is already removed from all targeted handlers

		// Squirrel away the touch
		claimTouch(touch);
	}

// Touches 

	bool CCScrollLayer::ccTouchBegan(CCTouch * touch, CCEvent * event)
	{
		CCPoint touchPoint = touch->locationInView(touch->view());
		touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

		startSwipe_ = touchPoint.x;
		state_ = kCCScrollLayerStateIdle;
		return true;
	}

	void CCScrollLayer::ccTouchMoved(CCTouch * touch , CCEvent * event)
	{
		CCPoint touchPoint = touch->locationInView(touch->view());
		touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);


		// If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
		// Of course only if we not already in sliding mode
		if ( (state_ != kCCScrollLayerStateSliding) 
			&& (fabsf(touchPoint.x-startSwipe_) >= minimumTouchLengthToSlide_) )
		{
			state_ = kCCScrollLayerStateSliding;

			// Avoid jerk after state change.
			startSwipe_ = touchPoint.x;

			cancelAndStoleTouch( touch , event);		
		}

		if (state_ == kCCScrollLayerStateSliding)
			setPosition(ccp((-(currentScreen_-1)*scrollWidth_)+(touchPoint.x-startSwipe_),0));	

	}

	void CCScrollLayer::ccTouchEnded(CCTouch * touch , CCEvent * event)
	{
		CCPoint touchPoint = touch->locationInView(touch->view());
		touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

		float newX = touchPoint.x;	

		if ( (newX - startSwipe_) < -minimumTouchLengthToChangePage_ && (currentScreen_+1) <= totalScreens_ )
		{		
			moveToPage(currentScreen_+1);		
		}
		else if ( (newX - startSwipe_) > minimumTouchLengthToChangePage_ && (currentScreen_-1) > 0 )
		{		
			moveToPage(currentScreen_-1);		
		}
		else
		{		
			moveToPage(currentScreen_);		
		}	
	} 

#ifdef SCROLLTESTCODE
	void addScrollTest(CCLayer* l, SelectorProtocol* target, SEL_MenuHandler selector){
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		/////////////////////////////////////////////////
		// PAGE 1
		////////////////////////////////////////////////
		// create a blank layer for page 1
		CCLayer *pageOne = CCLayer::node();

		// create a label for page 1
		CCLabelTTF *label = CCLabelTTF::labelWithString("Page 1" , "Arial Rounded MT Bold" ,44);
		label->setPosition ( ccp( screenSize.width /2 , screenSize.height/2 ));

		// add label to page 1 layer
		pageOne->addChild(label);

		/////////////////////////////////////////////////
		// PAGE 2
		////////////////////////////////////////////////
		// create a blank layer for page 2
		CCLayer *pageTwo = CCLayer::node();;

		// create a custom font menu for page 2
		CCLabelTTF *labelTwo = CCLabelTTF::labelWithString("Press me!" , "Arial Rounded MT Bold" ,44);;		
		CCMenuItemLabel *titem = CCMenuItemLabel ::itemWithLabel(labelTwo , target, selector);
		CCMenu *menu = CCMenu::menuWithItems(titem, NULL);
		menu->setPosition(ccp(screenSize.width/2, screenSize.height/2));

		// add menu to page 2
		pageTwo->addChild(menu);


		/////////////////////////////////////////////////
		// PAGE 3
		////////////////////////////////////////////////
		CCLayer *pageThree = CCLayerColor ::layerWithColor(ccc4(255, 0, 0, 128));

		////////////////////////////////////////////////

		// now create the scroller and pass-in the pages (set widthOffset to 0 for fullscreen pages)
		CCArray* layers = CCArray::array();
		layers->addObject(pageOne), layers->addObject(pageTwo), layers->addObject(pageThree);
		CCScrollLayer *scroller = CCScrollLayer ::nodeWithLayers(layers, 230);

		// finally add the scroller to your scene
		l->addChild(scroller);
	}

#endif

}


