/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Graphics

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

// Created 15-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Graphics.cpp
This file defines the Engine library scene global accessors and the SharedPtr deleter specialization for Graphics shared
pointer types.
*/
#include <EnginePch.h>
#include "SceneManager.h"

using namespace E;

namespace E 
{
  namespace Graphics
  {
  namespace Scene
  {
    /*----------------------------------------------------------------------------------------------------------------------
    SceneManagerProvider
    ----------------------------------------------------------------------------------------------------------------------*/
    class SceneManagerProvider
    {
    public:
      ISceneManagerInstance GetSceneManager();

    private:
      typedef Memory::GCConcreteFactory<SceneManager> SceneManagerFactory;

      SceneManagerFactory   mSceneManagerFactory;
      ISceneManagerInstance mSceneManager;

      E_DECLARE_SINGLETON_ONLY(SceneManagerProvider);
    };
  }}
}

/*----------------------------------------------------------------------------------------------------------------------
Graphics::Global methods
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::ISceneManagerInstance Graphics::Scene::Global::GetSceneManager()
{
  return Singleton<SceneManagerProvider>::GetInstance().GetSceneManager();
}

/*----------------------------------------------------------------------------------------------------------------------
Graphics::DeviceProvider private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::SceneManagerProvider::SceneManagerProvider() {}

Graphics::Scene::SceneManagerProvider::~SceneManagerProvider()
{
  mSceneManagerFactory.CleanUp();
}

/*----------------------------------------------------------------------------------------------------------------------
Graphics::Scene::SceneManagerProvider accessors
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::ISceneManagerInstance Graphics::Scene::SceneManagerProvider::GetSceneManager()
{
  if (mSceneManager == nullptr) mSceneManager = mSceneManagerFactory.Create();
  E_ASSERT_PTR(mSceneManager);
  return mSceneManager;
}
