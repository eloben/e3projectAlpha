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

// Created 10-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file PhongSample.cpp
This file defines the PhongSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
PhongSample methods
----------------------------------------------------------------------------------------------------------------------*/

void PhongSample::Load(IViewInstance window)
{
  mView = window;
  ISceneManagerInstance sceneManager = Global::GetSceneManager();

  // Create elements
  mCamera = sceneManager->CreateObject(IObject::eObjectTypeCamera);
  
  mCamera->Translate(Vector3f(0.0f, 0.0f, -120.0f));
  ILogicComponentInstance cameraLogicComponent = sceneManager->CreateComponent(IObjectComponent::eComponentTypeLogic);
  mCameraHandler.SetCamera(mCamera);
  mCameraHandler.SetSpeed(50.0f);
  mCameraHandler.SetSensitivity(0.015f);
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
  
  U32 rowCount = 4;
  U32 colCount = 5;
  F32 offset = 30.0f;
  F32 offsetX = -offset * (colCount - 1) * 0.5f;
  F32 offsetY = offset * (rowCount - 1) * 0.5f;
  
  for (U32 i = 0; i < colCount; ++i)
  {
    for (U32 j = 0; j < rowCount; ++j)
    {
      IMeshInstance sphere = sceneManager->CreateObject(IObject::eObjectTypeMesh);
      sphere->CreateSphere(8.0f /*+ i * 0.5f*/, 32, 32);
      sphere->Translate(Vector3f(offsetX + i * offset, offsetY - j * offset, 0.0f));
      IMaterialInstance sphereMaterial = mSceneManager->CreateMaterial();
      sphereMaterial->SetDiffuseColor(Graphics::Color(0.85f, 0.0f, 0.0f));
      sphereMaterial->SetSmoothness(0.05f + (j * colCount + i) * 0.05f);
      sphere->SetMaterial(sphereMaterial);
      mSceneManager->GetWorld()->Load(sphere);
    }
  }

  mLight = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight->SetColor(Graphics::Color(0.9f, 0.9f, 0.9f));
  mSceneManager->GetWorld()->Load(mLight);
}

void PhongSample::Update()
{
  Input::IInputManagerInstance pInputManager = Input::Global::GetInputManager();

  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (pInputManager->IsKeyDown(VK_ADD) || pInputManager->IsKeyDown(VK_SUBTRACT))
  {
    if (pInputManager->IsKeyDown(VK_SUBTRACT)) step = -step;
    mLight->Rotate(Vector3f(0.0f, step * 0.1f, 0.0f));
  }
}
