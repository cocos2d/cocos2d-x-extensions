#include "CCScale9Sprite.h"

enum positions {
	pCentre = 0,
	pTop,
	pLeft,
	pRight,
	pBottom,
	pTopRight,
	pTopLeft,
	pBottomRight,
	pBottomLeft
};	

using namespace cocos2d ;

CCScale9Sprite* CCScale9Sprite::scale9SpriteWithFile(const char* file, const CCRect & centreRegion) {
	CCScale9Sprite *pRet = new CCScale9Sprite(file,centreRegion);
	pRet->autorelease();
	return pRet;
}

CCScale9Sprite::CCScale9Sprite(const char* file, const CCRect & centreRegion) {
	scale9Image = CCSpriteBatchNode::batchNodeWithFile(file ,9);
	CCSize imageSize = scale9Image->getTexture()->getContentSize();
	CCLOG(">>>>>>>>> Image size %0.2f x %0.2f",imageSize.width,imageSize.height);
	//Set up centre sprite
	centre = CCSprite::spriteWithBatchNode(scale9Image , centreRegion);
	scale9Image->addChild(centre , 0 ,pCentre);

	//top
	top = CCSprite::spriteWithBatchNode(scale9Image,CCRect(centreRegion.origin.x,
	 0,
	 centreRegion.size.width,
	 centreRegion.origin.y));
	

	scale9Image->addChild(top ,1,pTop);

	//bottom
	bottom = CCSprite::spriteWithBatchNode(scale9Image,CCRect(centreRegion.origin.x,
	 centreRegion.origin.y + centreRegion.size.height,
	 centreRegion.size.width,
	 imageSize.height - (centreRegion.origin.y + centreRegion.size.height))
	);

	scale9Image->addChild(bottom ,1 ,pBottom);

	//left
	left = CCSprite::spriteWithBatchNode(scale9Image,CCRect(0,
	 centreRegion.origin.y,
	 centreRegion.origin.x,
	 centreRegion.size.height)
	);

	scale9Image->addChild(left ,1,pLeft);

	//right
	right = CCSprite::spriteWithBatchNode(scale9Image,CCRect(centreRegion.origin.x + centreRegion.size.width,
	 centreRegion.origin.y,
	 imageSize.width - (centreRegion.origin.x + centreRegion.size.width),
	 centreRegion.size.height)
	);

	scale9Image->addChild(right,1, pRight);

	//top left
	topLeft = CCSprite::spriteWithBatchNode(scale9Image,CCRect(0,
	 0,
	 centreRegion.origin.x,
	 centreRegion.origin.y)
	);

	scale9Image->addChild(topLeft ,2 ,pTopLeft);

	//top right
	topRight = CCSprite ::spriteWithBatchNode(scale9Image,CCRect(centreRegion.origin.x + centreRegion.size.width,
	 0,
	 imageSize.width - (centreRegion.origin.x + centreRegion.size.width),
	 centreRegion.origin.y)
	);

	scale9Image->addChild(topRight, 2, pTopRight);

	//bottom left
	bottomLeft = CCSprite::spriteWithBatchNode(scale9Image,CCRect(0,
	 centreRegion.origin.y + centreRegion.size.height,
	 centreRegion.origin.x,
	 imageSize.height - (centreRegion.origin.y + centreRegion.size.height))
	);

	scale9Image->addChild(bottomLeft ,2 ,pBottomLeft);

	//bottom right
	bottomRight = CCSprite ::spriteWithBatchNode(scale9Image,CCRect(centreRegion.origin.x + centreRegion.size.width,
	 centreRegion.origin.y + centreRegion.size.height,
	 imageSize.width - (centreRegion.origin.x + centreRegion.size.width),
	 imageSize.height - (centreRegion.origin.y + centreRegion.size.height))
	);

	scale9Image->addChild(bottomRight ,2 ,pBottomRight);

	baseSize = imageSize;
	resizableRegion = centreRegion;
	setContentSize(imageSize);
	addChild(scale9Image);
	
}


void CCScale9Sprite::setContentSize(const CCSize &size)
{

	CCNode::setContentSize(size);
	setAnchorPoint(ccp(0.5f,0.5f));

	CCLOG("scale9 set content size %0.2f %0.2f",size.width,size.height);
	CCLOG("leftCap %0.2f rightCap %0.2f",topLeft->getContentSize().width,topRight->getContentSize().width);

	float sizableWidth = size.width - topLeft->getContentSize().width - topRight->getContentSize().width;
	float sizableHeight = size.height - topLeft->getContentSize().height - bottomRight->getContentSize().height;
	float horizontalScale = sizableWidth/centre->getContentSize().width;
	float verticalScale = sizableHeight/centre->getContentSize().height;
	centre->setScaleX(horizontalScale);
	centre->setScaleY(verticalScale);
	float rescaledWidth = centre->getContentSize().width * horizontalScale;
	float rescaledHeight = centre->getContentSize().height * verticalScale;

	float despx = size.width*0.5f;
	float despy = size.height*0.5f;

	//Position corners
	topLeft->setPosition(ccp(-rescaledWidth/2 - topLeft->getContentSize().width/2 +despx, rescaledHeight/2 + topLeft->getContentSize().height*0.5f +despy) );
	topRight->setPosition(ccp(rescaledWidth/2 + topRight->getContentSize().width/2 +despx, rescaledHeight/2 + topRight->getContentSize().height*0.5f +despy));
	bottomLeft->setPosition(ccp(-rescaledWidth/2 - bottomLeft->getContentSize().width/2 +despx, -rescaledHeight/2 - bottomLeft->getContentSize().height*0.5f +despy));
	bottomRight->setPosition(ccp(rescaledWidth/2 + bottomRight->getContentSize().width/2 +despx, -rescaledHeight/2 + -bottomRight->getContentSize().height*0.5f +despy));
	top->setScaleX(horizontalScale);
	top->setPosition(ccp(0+despx,rescaledHeight/2 + topLeft->getContentSize().height*0.5f +despy));
	bottom->setScaleX (horizontalScale);
	bottom->setPosition(ccp(0+despx,-rescaledHeight/2 - bottomLeft->getContentSize().height*0.5f +despy));
	left->setScaleY (verticalScale);
	left->setPosition(ccp(-rescaledWidth/2 - topLeft->getContentSize().width/2 +despx, 0+despy));
	right->setScaleY (verticalScale);
	right->setPosition(ccp(rescaledWidth/2 + topRight->getContentSize().width/2 +despx, 0+despy));
	centre->setPosition(ccp(despx, despy));

	CCLOG("Scale9 setContentSize %02.f x %02.f <%0.2f x %0.2f>",sizableWidth,sizableHeight,horizontalScale,verticalScale);
}

void CCScale9Sprite::draw() {
	scale9Image->draw();
}	


//LabelBMFont - CCRGBAProtocol protocol
void CCScale9Sprite::setColor(ccColor3B var)
{
	m_tColor = var;
	if (scale9Image->getChildren() && scale9Image->getChildren()->count() != 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(scale9Image->getChildren(), child)
		{
			CCSprite* pNode = (CCSprite*) child;
			if (pNode)
			{
				pNode->setColor(m_tColor);
			}
		}
	}
}
ccColor3B CCScale9Sprite::getColor()
{
	return m_tColor;
}
void CCScale9Sprite::setOpacity(GLubyte var)
{
	m_cOpacity = var;

	if (scale9Image->getChildren() && scale9Image->getChildren()->count() != 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(scale9Image->getChildren(), child)
		{
			CCNode* pNode = (CCNode*) child;
			if (pNode)
			{
				CCRGBAProtocol *pRGBAProtocol = pNode->convertToRGBAProtocol();
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setOpacity(m_cOpacity);
				}
			}
		}
	}
}
GLubyte CCScale9Sprite::getOpacity()
{
	return m_cOpacity;
}
void CCScale9Sprite::setIsOpacityModifyRGB(bool var)
{
	m_bIsOpacityModifyRGB = var;
	if (scale9Image->getChildren() && scale9Image->getChildren()->count() != 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(scale9Image->getChildren(), child)
		{
			CCNode* pNode = (CCNode*) child;
			if (pNode)
			{
				CCRGBAProtocol *pRGBAProtocol = pNode->convertToRGBAProtocol();
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setIsOpacityModifyRGB(m_bIsOpacityModifyRGB);
				}
			}
		}
	}
}
bool CCScale9Sprite::getIsOpacityModifyRGB()
{
	return m_bIsOpacityModifyRGB;
}
