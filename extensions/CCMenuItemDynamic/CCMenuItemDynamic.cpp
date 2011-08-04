#include "CcMenuItemDynamic.h"
#include "CCScale9Sprite.h"
using namespace cocos2d;
CCMenuItemSprite * CCMenuItemDynamic::dynamicItemWithText(const char* text,  const char* font, const CCSize &minSize,  const char*normalImage, const char *selectedImage, SelectorProtocol* target, SEL_MenuHandler selector, const CCRect& centerRegion, GLubyte opacity) {
	  //Create the label
	  CCLabelBMFont *label = CCLabelBMFont ::labelWithString(text ,font);

	  //Calculate the Dynamic Button size
	  float minWidth = label->getContentSize().width>minSize.width?label->getContentSize().width:minSize.width;
	  float minHeight = label->getContentSize().height>minSize.height?label->getContentSize().height:minSize.height;

	  CCSize size(minWidth+30,minHeight+24);

	  label->setPosition(ccp(size.width*0.5f, size.height*0.5f));

	  //Sprite normal
	  CCScale9Sprite *normalSprite = CCScale9Sprite::scale9SpriteWithFile(normalImage, centerRegion);	
	  normalSprite ->setContentSize(size);
	  normalSprite ->setOpacity(opacity);
	  //Sprite selected
	  CCScale9Sprite *selectedSprite = NULL;
	  if(selectedImage){
		  selectedSprite = CCScale9Sprite::scale9SpriteWithFile(selectedImage, centerRegion);	
		  selectedSprite ->setContentSize(size);
		  selectedSprite->setOpacity(opacity);
	  }

	  //Create the CCMenuItemSprite
	  CCMenuItemSprite* returnItem = CCMenuItemSprite::itemFromNormalSprite(normalSprite,selectedSprite,target, selector);
	  returnItem->addChild(label);
	  return returnItem;

}

