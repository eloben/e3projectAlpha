/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Input

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

/** @file Input.cpp
This file defines the Input library accessor methods.
*/

#include <EnginePch.h>
#include "InputManager.h"

namespace E 
{
  namespace Input
  {
    /*----------------------------------------------------------------------------------------------------------------------
    InputManagerProvider
    ----------------------------------------------------------------------------------------------------------------------*/
    class InputManagerProvider
    {
    public:
      IInputManagerInstance GetInputManager();
      
    private:
      typedef Memory::GCConcreteFactory<InputManager> InputManagerFactory;

      InputManagerFactory   mInputManagerFactory;
      IInputManagerInstance mInputManager;

      E_DECLARE_SINGLETON_ONLY(InputManagerProvider);
    };
  }
}

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Input::Global methods
----------------------------------------------------------------------------------------------------------------------*/

Input::IInputManagerInstance Input::Global::GetInputManager()
{
  return Singleton<InputManagerProvider>::GetInstance().GetInputManager();
}

/*----------------------------------------------------------------------------------------------------------------------
Input::DeviceProvider private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Input::InputManagerProvider::InputManagerProvider() {}

Input::InputManagerProvider::~InputManagerProvider()
{
  mInputManagerFactory.CleanUp();
}

/*----------------------------------------------------------------------------------------------------------------------
Input::InputManagerProvider accessors
----------------------------------------------------------------------------------------------------------------------*/

Input::IInputManagerInstance Input::InputManagerProvider::GetInputManager()
{
  if (mInputManager == nullptr) mInputManager = mInputManagerFactory.Create();
  E_ASSERT_PTR(mInputManager);
  return mInputManager;
}
