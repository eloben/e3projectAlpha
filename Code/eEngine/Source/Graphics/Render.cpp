/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Render

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 24-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Render.cpp
This file defines the Engine library render accessor methods.
*/

#include <EnginePch.h>
#include "RenderManager.h"

using namespace E;

namespace E 
{
  namespace Graphics
  {
    /*----------------------------------------------------------------------------------------------------------------------
    RenderManagerProvider

    Please note that these methods have the following usage contract: 

    1. IRenderManager depends directly on IDevice present currently in the Graphics DLL. For this reason to avoid
    _CrtIsValidHeapPointer assertions the Graphics DLL global allocator is used inside the Engine DLL. A different 
    allocator could be also used instead e.g. the Engine DLL global allocator such as:

    Graphics::Global::SetAllocator(Memory::Global::GetAllocator());

    However using it causes memory leaks on exit due to GCRef from the Graphics DLL that are still being held by the
    Engine DLL after the first one has been unloaded. This behavior is not wrong, actually there is a lot of sources
    against gracefully DLL termination to avoid long application finalization times. The important matter is that on
    exit the leaking DLL has a constant number of leaks, otherwise the leak would be in fact a sign of an internal 
    memory issue that should be addressed.
    ----------------------------------------------------------------------------------------------------------------------*/
    class RenderManagerProvider
    {
    public:
      IRenderManagerInstance GetRenderManager();

    private:
      typedef Memory::GCConcreteFactory<RenderManager> RenderManagerFactory;

      RenderManagerFactory   mRenderManagerFactory;
      IRenderManagerInstance mRenderManager;

      E_DECLARE_SINGLETON_ONLY(RenderManagerProvider);
    };
  }
}

/*----------------------------------------------------------------------------------------------------------------------
Render::Global methods
----------------------------------------------------------------------------------------------------------------------*/

Graphics::IRenderManagerInstance Graphics::Global::GetRenderManager()
{
  return Singleton<RenderManagerProvider>::GetInstance().GetRenderManager();
}

/*----------------------------------------------------------------------------------------------------------------------
Render::DeviceProvider private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::RenderManagerProvider::RenderManagerProvider()
{
  E_DEBUG_MSG("Graphics::RenderManagerProvider allocator: 0x%p", Memory::Global::GetAllocator());
}

Graphics::RenderManagerProvider::~RenderManagerProvider()
{
  mRenderManagerFactory.CleanUp();
}

/*----------------------------------------------------------------------------------------------------------------------
Render::RenderManagerProvider accessors
----------------------------------------------------------------------------------------------------------------------*/

Graphics::IRenderManagerInstance Graphics::RenderManagerProvider::GetRenderManager()
{
  if (mRenderManager == nullptr) mRenderManager = mRenderManagerFactory.Create();
  E_ASSERT_PTR(mRenderManager);
  return mRenderManager;
}
