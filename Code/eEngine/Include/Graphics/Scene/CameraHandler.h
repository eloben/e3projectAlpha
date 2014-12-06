/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

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

// Created 18-Sep-2014 by Elías Lozada-Benavente
// Original 15-Nov-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file CameraHandler.h
This file declares the CameraHandler class.
*/

#ifndef E3_CAMERA_HANDLER_H
#define E3_CAMERA_HANDLER_H

#include <Graphics/Scene/ICamera.h>
#include <Graphics/Scene/ILogicComponent.h>

namespace E 
{
  namespace Application
  {
    // Forward reference
    class InputManager;
}

namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
CameraHandler
----------------------------------------------------------------------------------------------------------------------*/
class CameraHandler : public ILogicHandler
{
public:
  enum MoveDirection
  {
    eMoveDirectionNone = 0,
    eMoveDirectionForward      = 1,
    eMoveDirectionBackwards    = 2,
    eMoveDirectionLeft   = 4,
    eMoveDirectionRight  = 8
  };

  E_API CameraHandler();

  // Accessors
  E_API void                    SetCamera(const ICameraInstance& camera);
  E_API void                    SetSensitivity(F32 sensitivity);
  E_API void                    SetSpeed(F32 unitsPerSecond);

  E_API void                    Move(MoveDirection dir);
  E_API void                    SetActive(bool b);

  // Callback methods
  void                          OnLoad();
  void                          OnUnload();
  void                          OnUpdate(const TimeValue& deltaTime);

private:
  U8                 mMove;
  ICameraInstance               mCamera;
  Application::InputManager&    mInputManager;
  Vector2i                      mLastCursorPosition;
  Vector3f                      mTranslation;
  F32                           mSpeed;
  F32                           mSensitivity;
  bool mActive;
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(CameraHandler)
};

/*----------------------------------------------------------------------------------------------------------------------
IWindowHandler types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<CameraHandler> CameraHandlerInstance;
typedef Memory::GCStaticPtr<CameraHandler> CameraHandlerOwner;
}
}
}

#endif
