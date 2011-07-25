#include "AdvanceParticleTest.h"
#include "AdvanceSpriteAndParticle/CCAdvanceParticleExample.h"

using namespace cocos2d;

enum 
{
	kTagLabelAtlas = 1,
};

//------------------------------------------------------------------
//
//Advance Particle Animation
//
//------------------------------------------------------------------
void ParticleAnimation::onEnter()
{
    ParticleDemo::onEnter();

    CCSprite *tSprite = CCSprite::spriteWithFile("Images/spritesheet1.png");
    m_emitter = CCParticleAniamtion::node();
    m_emitter->retain();
    m_background->addChild(m_emitter, 10);
    
    m_emitter->setLife(3.0f);
    m_emitter->setLifeVar(2.5f);
    
    // angle
	m_emitter->setAngle(90);
	m_emitter->setAngleVar(360);
    
    // speed of particles
	m_emitter->setSpeed(50);
	m_emitter->setSpeedVar(35);
    
    ((AdvanceParticleQuadSystem *)m_emitter)->setDisplayFrameForAnimation(tSprite->displayedFrame(), 32, 32, 10, false);
    
    m_emitter->setEmissionRate(10000);
    
    ((AdvanceParticleQuadSystem *)m_emitter)->setStartSize(32.0f);
    ((AdvanceParticleQuadSystem *)m_emitter)->setEndSize(32.0);
    setEmitterPosition();
}

std::string ParticleAnimation::title()
{
    return "Advance Particle Animaiton";
}

//------------------------------------------------------------------
//
// ParticleDemo
//
//------------------------------------------------------------------

enum
{
	IDC_NEXT = 100,
	IDC_BACK,
	IDC_RESTART,
	IDC_TOGGLE
};

static int sceneIdx = -1; 

#define MAX_LAYER	1

CCLayer* createParticleLayer(int nIndex)
{
	switch(nIndex)
	{
        case 0: return new ParticleAnimation();
	}

	return NULL;
}


CCLayer* nextParticleAction()
{
	sceneIdx++;
	sceneIdx = sceneIdx % MAX_LAYER;

	CCLayer* pLayer = createParticleLayer(sceneIdx);
	pLayer->autorelease();

	return pLayer;
}

CCLayer* backParticleAction()
{
	sceneIdx--;
	int total = MAX_LAYER;
	if( sceneIdx < 0 )
		sceneIdx += total;	
	
	CCLayer* pLayer = createParticleLayer(sceneIdx);
	pLayer->autorelease();

	return pLayer;
}

CCLayer* restartParticleAction()
{
	CCLayer* pLayer = createParticleLayer(sceneIdx);
	pLayer->autorelease();

	return pLayer;
} 


ParticleDemo::ParticleDemo(void)
{
	initWithColor( ccc4(127,127,127,255) );

	m_emitter = NULL;

	setIsTouchEnabled( true );
	
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCLabelTTF* label = CCLabelTTF::labelWithString(title().c_str(), "Arial", 28);
	addChild(label, 100, 1000);
	label->setPosition( CCPointMake(s.width/2, s.height-50) );
	
	CCLabelTTF *tapScreen = CCLabelTTF::labelWithString("(Tap the Screen)", "Arial", 20);
	tapScreen->setPosition( CCPointMake(s.width/2, s.height-80) );
	addChild(tapScreen, 100);
	
	CCMenuItemImage* item1 = CCMenuItemImage::itemFromNormalImage("b1.png", "b2.png", this, menu_selector(ParticleDemo::backCallback) );
	CCMenuItemImage* item2 = CCMenuItemImage::itemFromNormalImage("r1.png", "r2.png", this, menu_selector(ParticleDemo::restartCallback) );
	CCMenuItemImage* item3 = CCMenuItemImage::itemFromNormalImage("f1.png", "f2.png",  this, menu_selector(ParticleDemo::nextCallback) );
	
	CCMenuItemToggle* item4 = CCMenuItemToggle::itemWithTarget(	this, 
																menu_selector(ParticleDemo::toggleCallback), 
																CCMenuItemFont::itemFromString( "Free Movement" ),
                                                                CCMenuItemFont::itemFromString( "Relative Movement" ),
																CCMenuItemFont::itemFromString( "Grouped Movement" ),
																NULL );
	
	CCMenu *menu = CCMenu::menuWithItems(item1, item2, item3, item4, NULL);
		
	menu->setPosition( CCPointZero );
	item1->setPosition( CCPointMake( s.width/2 - 100,30) );
	item2->setPosition( CCPointMake( s.width/2, 30) );
	item3->setPosition( CCPointMake( s.width/2 + 100,30) );
	item4->setPosition( CCPointMake( 0, 100) );
	item4->setAnchorPoint( CCPointMake(0,0) );

	addChild( menu, 100 );	
	
    CCLabelAtlas* labelAtlas = CCLabelAtlas::labelWithString("0000", "fonts/fps_images.png", 16, 24, '.');
    addChild(labelAtlas, 100, kTagLabelAtlas);
	labelAtlas->setPosition( CCPointMake(s.width-66,50) );
	
	// moving background
	m_background = CCSprite::spriteWithFile("Images/background3.png");
	addChild(m_background, 5);
	m_background->setPosition( CCPointMake(s.width/2, s.height-180) );

	CCActionInterval* move = CCMoveBy::actionWithDuration(4, CCPointMake(300,0) );
	CCActionInterval* move_back = move->reverse();
	CCFiniteTimeAction* seq = CCSequence::actions( move, move_back, NULL);
	m_background->runAction( CCRepeatForever::actionWithAction((CCActionInterval*)seq) );
	
	
	schedule( schedule_selector(ParticleDemo::step) );
}

ParticleDemo::~ParticleDemo(void)
{
	m_emitter->release(); 
}

void ParticleDemo::onEnter(void)
{
	CCLayer::onEnter();

	CCLabelTTF* pLabel = (CCLabelTTF*)(this->getChildByTag(1000));
	pLabel->setString(title().c_str());
}

std::string ParticleDemo::title()
{
	return "No title";
}

void ParticleDemo::registerWithTouchDispatcher()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

bool ParticleDemo::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	return true;
}

void ParticleDemo::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	return ccTouchEnded(touch, event);
}

void ParticleDemo::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint location = touch->locationInView( touch->view() );
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

    CCPoint pos = CCPointZero;
    if (m_background)
    {
	    pos = m_background->convertToWorldSpace(CCPointZero);
    }
	m_emitter->setPosition( ccpSub(convertedLocation, pos) );	
}

void ParticleDemo::step(ccTime dt)
{
    if (m_emitter)
    {
	    CCLabelAtlas* atlas = (CCLabelAtlas*)getChildByTag(kTagLabelAtlas);
        char str[5] = {0};
        sprintf(str, "%04d", m_emitter->getParticleCount());
	    atlas->setString(str);
    }
}

void ParticleDemo::toggleCallback(CCObject* pSender)
{
	if( m_emitter->getPositionType() == kCCPositionTypeGrouped )
		m_emitter->setPositionType( kCCPositionTypeFree );
    else if (m_emitter->getPositionType() == kCCPositionTypeFree)
        m_emitter->setPositionType(kCCPositionTypeRelative);
	else if (m_emitter->getPositionType() == kCCPositionTypeRelative)
		m_emitter->setPositionType( kCCPositionTypeGrouped );
}

void ParticleDemo::restartCallback(CCObject* pSender)
{
	m_emitter->resetSystem(); 
}

void ParticleDemo::nextCallback(CCObject* pSender)
{
	CCScene* s = new AdvanceParticleTestScene();
	s->addChild( nextParticleAction() );
	CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ParticleDemo::backCallback(CCObject* pSender)
{
	CCScene* s = new AdvanceParticleTestScene();
	s->addChild( backParticleAction() );
	CCDirector::sharedDirector()->replaceScene(s);
    s->release();
} 

void ParticleDemo::setEmitterPosition()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	m_emitter->setPosition( CCPointMake(s.width / 2, s.height / 2) );
}

void AdvanceParticleTestScene::runThisTest()
{
    addChild(nextParticleAction());

    CCDirector::sharedDirector()->replaceScene(this);
}
