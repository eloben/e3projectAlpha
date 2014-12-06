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

// Created 18-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LightSpotShadowSample.cpp
This file defines the LightSpotShadowSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
LightSpotShadowSample methods
----------------------------------------------------------------------------------------------------------------------*/

void LightSpotShadowSample::Load(IViewInstance window)
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
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
  
  // Create a cube mesh
  IMeshInstance groundPlane = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  IMaterialInstance groundMaterial = mSceneManager->CreateMaterial();
  //groundMaterial->Set(IMaterial::eAluminium);
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
  
  IShadowComponentInstance lightSpotShadowComponent0 = sceneManager->CreateComponent(IObjectComponent::eComponentTypeShadow);
  mLightSpot[0] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[0]->SetPosition(Vector3f(50, 40.0f, 0.0f));
  mLightSpot[0]->SetColor(Graphics::Color::eRed);
  mLightSpot[0]->Rotate(Vector3f(45.0f, -90.0f, 0.0f));
  mLightSpot[0]->SetAttenuation(0.0f, 0.02f, 1.0f);
  mLightSpot[0]->SetRange(200.0f);
  mLightSpot[0]->SetCutOffAngle(mCutOffAngle);
  mLightSpot[0]->AddComponent(lightSpotShadowComponent0);
  mSceneManager->GetWorld()->Load(mLightSpot[0]);
  
  IShadowComponentInstance lightSpotShadowComponent1 = sceneManager->CreateComponent(IObjectComponent::eComponentTypeShadow);
  mLightSpot[1] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[1]->SetPosition(Vector3f(-50, 40.0f, 50.0f));
  mLightSpot[1]->SetColor(Graphics::Color::eBlue);
  mLightSpot[1]->Rotate(Vector3f(45.0f, 135.0f, 0.0f));
  mLightSpot[1]->SetAttenuation(0.0f, 0.02f, 1.0f);
  mLightSpot[1]->SetRange(200.0f);
  mLightSpot[1]->SetCutOffAngle(mCutOffAngle);
  mLightSpot[1]->AddComponent(lightSpotShadowComponent1);
  mSceneManager->GetWorld()->Load(mLightSpot[1]);

  IShadowComponentInstance lightSpotShadowComponent2 = sceneManager->CreateComponent(IObjectComponent::eComponentTypeShadow);
  mLightSpot[2] = sceneManager->CreateObject(IObject::eObjectTypeLightSpot);
  mLightSpot[2]->SetPosition(Vector3f(-50, 40.0f, -50.0f));
  mLightSpot[2]->SetColor(Graphics::Color::eGreen);
  mLightSpot[2]->Rotate(Vector3f(45.0f, 45.0f, 0.0f));
  mLightSpot[2]->SetAttenuation(0.0f, 0.02f, 1.0f);
  mLightSpot[2]->SetRange(200.0f);
  mLightSpot[2]->SetCutOffAngle(mCutOffAngle);
  mLightSpot[2]->AddComponent(lightSpotShadowComponent2);
  mSceneManager->GetWorld()->Load(mLightSpot[2]);
}

void LightSpotShadowSample::Update()
{
  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (mInputManager.IsKeyDown(VK_ADD) || mInputManager.IsKeyDown(VK_SUBTRACT))
  {
    if (mInputManager.IsKeyDown(VK_SUBTRACT)) step = -step;
    //mSceneManager->GetRenderer()->SetSlopeScaledDepthBias(0.0001f * step);
   
    //mCutOffAngle = Math::Clamp(mCutOffAngle + step * 0.02f, 0.0f, 60.0f);
    for (U32 i = 0; i < 3; ++i) 
    {
      mLightSpot[i]->Rotate(Vector3f(0.0f, 0.025f * step, 0.0f));
      mLightSpot[i]->Translate(Vector3f(0.0f, 0.0f, -0.025f * step));
    }
  }
}
