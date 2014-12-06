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

// Created 12-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file MaterialSample.cpp
This file defines the MaterialSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
MaterialSample methods
----------------------------------------------------------------------------------------------------------------------*/

void MaterialSample::Load(IViewInstance window)
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
      //sphereMaterial->SetDiffuseColor(Graphics::Color(0.05f + (j * colCount + i) * 0.05f, 0.0f, 0.0f));
      sphereMaterial->Set(static_cast<IMaterial::PresetID>(i));
      sphereMaterial->SetSmoothness(0.1f + (j * colCount + i) * 0.01f);
      sphere->SetMaterial(sphereMaterial);
      mSceneManager->GetWorld()->Load(sphere);
    }
  }

  mLight = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight->SetColor(Graphics::Color(0.9f, 0.9f, 0.9f));
  mSceneManager->GetWorld()->Load(mLight);
  
  ILightInstance directionalLight2 = sceneManager->CreateObject(IObject::eObjectTypeLight);
  directionalLight2->Rotate(Vector3f(0.0f, -90.0f, 45.0f));
  directionalLight2->SetColor(Graphics::Color(0.9f, 0.0f, 0.0f));
  mSceneManager->GetWorld()->Load(directionalLight2);
  
  ILightInstance directionalLight3 = sceneManager->CreateObject(IObject::eObjectTypeLight);
  directionalLight3->Rotate(Vector3f(0.0f, 90.0f, -45.0f));
  directionalLight3->SetColor(Graphics::Color(0.1f, 0.8f, 0.2f));
  mSceneManager->GetWorld()->Load(directionalLight3);
  
  ILightInstance directionalLight4 = sceneManager->CreateObject(IObject::eObjectTypeLight);
  directionalLight4->Rotate(Vector3f(45.0f, 90.0f, 45.0f));
  directionalLight4->SetColor(Graphics::Color(0.4f, 0.4f, 0.6f));
  mSceneManager->GetWorld()->Load(directionalLight4);
}

void MaterialSample::Update()
{
  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (mInputManager.IsKeyDown(VK_ADD) || mInputManager.IsKeyDown(VK_SUBTRACT))
  {
    if (mInputManager.IsKeyDown(VK_SUBTRACT)) step = -step;
    mLight->Rotate(Vector3f(0.0f, step * 0.1f, 0.0f));
  }
}
