#include "CCRadioMenu.h"
using namespace cocos2d;
void CCRadioMenu::setSelectedRadioItem(CCMenuItem * item)
{
	m_pSelectedItem->unselected();
	m_pSelectedItem = item;
}
CCRadioMenu * CCRadioMenu::radioMenuWithItems(CCMenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
	CCRadioMenu *pRet = new CCRadioMenu();
	if (pRet && pRet->initWithItems(item, args))
	{
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet)
		return NULL;
}

bool CCRadioMenu::ccTouchBegan(CCTouch *touch ,CCEvent *event) {

	if ( m_eState != kCCMenuStateWaiting ) return false;

	CCMenuItem *curSelection = itemForTouch(touch);
	if(!curSelection)return false;
	curSelection->selected();
	_curHighlighted = curSelection;

	if (_curHighlighted) {
		if (m_pSelectedItem != curSelection && m_pSelectedItem) {
			m_pSelectedItem->unselected();
		}
		m_eState = kCCMenuStateTrackingTouch;
		return true;
	}
	return false;

}

void CCRadioMenu::ccTouchEnded(CCTouch *touch ,CCEvent *event) {
	CCMenuItem *curSelection = itemForTouch(touch);
	if (curSelection != _curHighlighted && curSelection != NULL) {
		if(m_pSelectedItem) m_pSelectedItem->selected();
		_curHighlighted->unselected();
		_curHighlighted = NULL;
		m_eState = kCCMenuStateWaiting;
		return;
	} 

	m_pSelectedItem = _curHighlighted;
	_curHighlighted -> activate();
	_curHighlighted = NULL;

	m_eState = kCCMenuStateWaiting;

}

void CCRadioMenu::ccTouchCancelled(CCTouch *touch ,CCEvent *event) {
	m_pSelectedItem->selected();
	_curHighlighted->unselected();
	_curHighlighted = NULL;

	m_eState = kCCMenuStateWaiting;

}

void CCRadioMenu::ccTouchMoved(CCTouch *touch, CCEvent * event)
{
	
	CCMenuItem *curSelection = itemForTouch(touch);
	if (curSelection != _curHighlighted && curSelection != NULL) {       
		_curHighlighted->unselected();
		curSelection->selected();
		_curHighlighted = curSelection;        
		return;
	}

}