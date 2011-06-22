/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada


http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CC_ADVANCE_PARTICLE_EXAMPLE_H__
#define __CC_ADVANCE_PARTICLE_EXAMPLE_H__

#include "platform/CCArchOptimalParticleSystem.h"
#include "AdvanceParticleQuadSystem.h"

namespace cocos2d {
    
//! @brief A rain particle system
class CC_DLL CCParticleAniamtion : public AdvanceParticleQuadSystem
{
public:
    CCParticleAniamtion(){}
    virtual ~CCParticleAniamtion(){}
    bool init(){ return initWithTotalParticles(10); }
    virtual bool initWithTotalParticles(int numberOfParticles);
    static CCParticleAniamtion * node()
    {
        CCParticleAniamtion *pRet = new CCParticleAniamtion();
        if (pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet)
        return NULL;
    }
};

}// namespace cocos2d

#endif //__CCPARTICLE_EXAMPLE_H__
