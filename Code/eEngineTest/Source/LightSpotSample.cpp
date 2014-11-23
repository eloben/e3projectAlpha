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

// Created 16-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LightSpotSample.cpp
This file defines the LightSpotSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;


/*----------------------------------------------------------------------------------------------------------------------
LightSpotSample methods
----------------------------------------------------------------------------------------------------------------------*/

void LightSpotSample::Load(IViewInstance window)
{
  mCutOffAngle = 45.0f;
  mLightTranslation = 50.0f;

  mView = window;
  ISceneManagerInstance sceneManager = Global::GetSceneManager();

  // Create elements
  mCamera = sceneManager->CreateObject(IObject::eObjectTypeCamera);
  
  mCamera->Translate(Vector3f(25.0f, 25.0f, -50.0f));
  mCamera->Rotate(Vector3f(10.0f, -25.0f, 0.0f));
  ILogicComponentInstance cameraLogicComponent = sceneManager->CreateComponent(IObjectComponent::eComponentTypeLogic);
  mCameraHandler.SetCamera(mCamera);
  mCameraHandler.SetSpeed(50.0f);
  mCameraHandler.SetSensitivity(0.015f);
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
  
  // Create a cube mesh
  IMeshInstance groundPlane = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  IMaterialInstance groundMaterial = mSceneManager->CreateMaterial();
  groundMaterial->SetDiffuseColor(Graphics::Color::eWhite);
  groundMaterial->SetSpecularColor(Graphics::Color::eWhite);
  groundPlane->CreateQuad(100.0f);
  groundPlane->Rotate(Vector3f(90.0f, 0.0f, 0.0f));
  groundPlane->SetMaterial(groundMaterial);
  mSceneManager->GetWorld()->Load(groundPlane);
 
  IMeshInstance cube = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  cube->CreateCube(10.0f);
  cube->Translate(Vector3f(-20.0f, 5.0f, 0.0f));
  IMaterialInstance cubeMaterial = mSceneManager->CreateMaterial();
  cubeMaterial->SetDiffuseColor(Graphics::Color::eWhite);
  cubeMaterial->SetSpecularColor(Graphics::Color::eWhite);
  cubeMaterial->SetSmoothness(0.02f);
  cube->SetMaterial(cubeMaterial);
  mSceneManager->GetWorld()->Load(cube);
 
  IMeshInstance sphere = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  sphere->CreateSphere(7.5f, 32, 32);
  sphere->Translate(Vector3f(0.0f, 7.5f, 0.0f));
  IMaterialInstance sphereMaterial = mSceneManager->CreateMaterial();
  sphereMaterial->SetDiffuseColor(Graphics::Color::eWhite);
  sphereMaterial->SetSpecularColor(Graphics::Color::eWhite);
  sphereMaterial->SetSmoothness(0.02f);
  sphere->SetMaterial(sphereMaterial);
  mSceneManager->GetWorld()->Load(sphere);
  
  IMeshInstance cylinder = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  cylinder->CreateCylinder(4.0f, 4.0f, 20.0f, 16, 1);
  cylinder->Translate(Vector3f(20.0f, 10.0f, 0.0f));
  IMaterialInstance cylinderMaterial = mSceneManager->CreateMaterial();
  cylinderMaterial->SetDiffuseColor(Graphics::Color::eWhite);
  cylinderMaterial->SetSpecularColor(Graphics::Color::eWhite);
  cylinderMaterial->SetSmoothness(0.02f);
  cylinder->SetMaterial(cylinderMaterial);
  mSceneManager->GetWorld()->Load(cylinder);
  
  mLightSpot[0] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[0]->SetPosition(Vector3f(0, 40.0f, -mLightTranslation));
  mLightSpot[0]->SetColor(Graphics::Color::eRed);
  mLightSpot[0]->Rotate(Vector3f(60.0f, 0.0f, 0.0f));
  mLightSpot[0]->SetAttenuation(0.0f, 0.3f, 1.0f);
  mLightSpot[0]->SetRange(100.0f);
  mSceneManager->GetWorld()->Load(mLightSpot[0]);
  
  mLightSpot[1] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[1]->SetPosition(Vector3f(0, 40.0f, mLightTranslation));
  mLightSpot[1]->SetColor(Graphics::Color::eGreen);
  mLightSpot[1]->Rotate(Vector3f(60.0f, 0.0f, 0.0f));
  mLightSpot[1]->SetAttenuation(0.0f, 0.3f, 1.0f);
  mLightSpot[1]->SetRange(100.0f);
  mSceneManager->GetWorld()->Load(mLightSpot[1]);

  mLightSpot[2] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[2]->SetPosition(Vector3f(mLightTranslation, 40.0f, 0.0f));
  mLightSpot[2]->SetColor(Graphics::Color::eBlue);
  mLightSpot[2]->Rotate(Vector3f(60.0f, 0.0f, 0.0f));
  mLightSpot[2]->SetAttenuation(0.0f, 0.3f, 1.0f);
  mLightSpot[2]->SetRange(100.0f);
  mSceneManager->GetWorld()->Load(mLightSpot[2]);

  mLightSpot[3] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[3]->SetPosition(Vector3f(-mLightTranslation, 40.0f, 0.0f));
  mLightSpot[3]->SetColor(Graphics::Color::eYellow);
  mLightSpot[3]->Rotate(Vector3f(60.0f, 0.0f, 0.0f));
  mLightSpot[3]->SetAttenuation(0.0f, 0.3f, 1.0f);
  mLightSpot[3]->SetRange(100.0f);
  mSceneManager->GetWorld()->Load(mLightSpot[3]);
}

void LightSpotSample::Update()
{
  Input::IInputManagerInstance pInputManager = Input::Global::GetInputManager();

  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (pInputManager->IsKeyDown(VK_ADD) || pInputManager->IsKeyDown(VK_SUBTRACT))
  {
    if (pInputManager->IsKeyDown(VK_SUBTRACT)) step = -step;
    mCutOffAngle = Math::Clamp(mCutOffAngle + step * 0.02f, 0.0f, 60.0f);
    for (U32 i = 0; i < 4; ++i) mLightSpot[i]->SetCutOffAngle(mCutOffAngle);
  }
  
  mLightSpot[0]->Rotate(Vector3f(0.0f, 0.1f * step, 0.0f));
  mLightSpot[1]->Rotate(Vector3f(0.0f, 0.075f * step, 0.0f));
  mLightSpot[2]->Rotate(Vector3f(0.0f, 0.055f * step, 0.0f));
  mLightSpot[3]->Rotate(Vector3f(0.0f, 0.035f * step, 0.0f));
}
