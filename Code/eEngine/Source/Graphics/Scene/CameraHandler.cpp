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

/** @file CameraHandler.cpp
This file defines the CameraHandler class.
*/

#include <EnginePch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
CameraHandler initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::CameraHandler::CameraHandler()
  : mInputManager(Input::Global::GetInputManager())
  , mSpeed(0.01f)
  , mSensitivity(0.25f) {}

/*----------------------------------------------------------------------------------------------------------------------
CameraHandler accessors
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::CameraHandler::SetCamera(const ICameraInstance& camera)
{
  mCamera = camera;
}

void Graphics::Scene::CameraHandler::SetSensitivity(F32 sensitivity)
{
  mSensitivity = sensitivity;
}

void Graphics::Scene::CameraHandler::SetSpeed(F32 unitsPerSecond)
{
  mSpeed = unitsPerSecond * 0.001f;
}

/*----------------------------------------------------------------------------------------------------------------------
CameraHandler methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::CameraHandler::OnLoad() {}

void Graphics::Scene::CameraHandler::OnUnload() {}

void Graphics::Scene::CameraHandler::OnUpdate(const TimeValue& deltaTime)
{
  E_ASSERT_PTR(mCamera);
  if (mCamera)
  {
    F32 timeStep = static_cast<F32>(deltaTime.GetMilliseconds());
    F32 forwardSpeed = 0.0f;
    F32 strafeSpeed = 0.0f;
    if (mInputManager->IsKeyDown('W')) forwardSpeed += mSpeed;
    if (mInputManager->IsKeyDown('S')) forwardSpeed -= mSpeed;
    if (mInputManager->IsKeyDown('A')) strafeSpeed -= mSpeed;
    if (mInputManager->IsKeyDown('D')) strafeSpeed += mSpeed;

    // Calculate translation delta
    Vector3f  translationDelta = mCamera->GetAxisZ() * forwardSpeed + mCamera->GetAxisX() * strafeSpeed * Math::kSqrt2Div2f;
    // Apply time step
    translationDelta *= timeStep;
    // Update camera position
    mCamera->Translate(translationDelta);

    if (mInputManager->IsKeyDown(MK_LBUTTON))
    {	
      // Calculate rotation delta
      const Vector2i& currentMousePosition = mInputManager->GetCursonPosition();
      Vector3f rotationDelta(
        static_cast<F32>(currentMousePosition.y - mLastCursorPosition.y),
       // 0.0f,
        static_cast<F32>(currentMousePosition.x - mLastCursorPosition.x),
        0.0f);

      if (!rotationDelta.IsZero())
      {
        // Do not scale rotation delta by timeStep: the API is giving translation units in pixels hence moving the mouse
        // X pixels should rotate the camera Y degrees, regardless of how many frames or how long it took to move.
        rotationDelta *= mSensitivity;
        // Update camera orientation
        mCamera->Rotate(rotationDelta);
      }
    }
    // Update cursor position
    mLastCursorPosition = mInputManager->GetCursonPosition();
  }
}
