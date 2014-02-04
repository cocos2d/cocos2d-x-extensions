/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#if CCX_SUPPORT_MULTITHREAD

#include "ccxThread.h"

#include <Windows.h>

NS_CC_BEGIN;

class CCXLock::Impl
{
public:
    Impl()              { InitializeCriticalSection(&m_cs); }
    ~Impl()             { DeleteCriticalSection(&m_cs); }

    CRITICAL_SECTION m_cs;
};

CCXLock::CCXLock()
: m_pImp(new CCXLock::Impl)
{
}

CCXLock::~CCXLock()
{
    CC_SAFE_DELETE(m_pImp);
}

void CCXLock::lock()
{
    if (m_pImp)
    {
        EnterCriticalSection(&m_pImp->m_cs);
    }
}

void CCXLock::unlock()
{
    if (m_pImp)
    {
        LeaveCriticalSection(&m_pImp->m_cs);
    }
}

NS_CC_END;

#endif  // CCX_SUPPORT_MULTITHREAD
