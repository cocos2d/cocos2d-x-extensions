#ifndef CCScale9Sprite_h__
#define CCScale9Sprite_h__

#include "cocos2d.h"

//  Copyright Eli Yukelzon, base on code by Jose Antonio Andújar Clavell 
//
//  License http://creativecommons.org/licenses/by/3.0/

namespace cocos2d {
class CCScale9Sprite : 	public CCNode ,CCRGBAProtocol
{

	CCSpriteBatchNode *scale9Image;
	CCSprite *topLeft;
	CCSprite *top;
	CCSprite *topRight;
	CCSprite *left;
	CCSprite *centre;
	CCSprite *right;
	CCSprite *bottomLeft;
	CCSprite *bottom;
	CCSprite *bottomRight;
	CCSize baseSize;
	CCRect resizableRegion;

	// texture RGBA

	CC_PROPERTY(GLubyte, m_cOpacity, Opacity); 
	CC_PROPERTY(ccColor3B, m_tColor, Color);
	CC_PROPERTY(bool, m_bIsOpacityModifyRGB, IsOpacityModifyRGB);
public:
	static CCScale9Sprite* scale9SpriteWithFile(const char* file, const CCRect & centreRegion);
	CCScale9Sprite(const char* file, const CCRect & centreRegion);
	void draw();
	void setContentSize(const CCSize &size);
	virtual CCRGBAProtocol* convertToRGBAProtocol() { return (CCRGBAProtocol *)this; }

};
}
#endif // CCScale9Sprite_h__
