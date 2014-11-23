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

// Created 17-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LightPointSample.cpp
This file defines the LightPointSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
LightPointSample auxiliary
----------------------------------------------------------------------------------------------------------------------*/

static float gLightAngle[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static float gLightTranslation = 50.0f;

/*----------------------------------------------------------------------------------------------------------------------
LightPointSample methods
----------------------------------------------------------------------------------------------------------------------*/

void LightPointSample::Load(IViewInstance window)
{
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
  
  mLightPoint[0] = sceneManager->CreateObject(IObject::eObjectTypeLightPoint);
  mLightPoint[0]->SetPosition(Vector3f(gLightTranslation, gLightTranslation, gLightTranslation));
  mLightPoint[0]->SetColor(Graphics::Color::eRed);
  mLightPoint[0]->SetAttenuation(0.0f, 0.2f, 1.0f);
  mLightPoint[0]->SetRange(350.0f);
  mSceneManager->GetWorld()->Load(mLightPoint[0]);
  
  mLightPoint[1] = sceneManager->CreateObject(IObject::eObjectTypeLightPoint);
  mLightPoint[1]->SetPosition(Vector3f(-gLightTranslation, gLightTranslation, gLightTranslation));
  mLightPoint[1]->SetColor(Graphics::Color::eGreen);
  mLightPoint[1]->SetAttenuation(0.0f, 0.2f, 1.0f);
  mLightPoint[1]->SetRange(350.0f);
  mSceneManager->GetWorld()->Load(mLightPoint[1]);

  mLightPoint[2] = sceneManager->CreateObject(IObject::eObjectTypeLightPoint);
  mLightPoint[2]->SetPosition(Vector3f(-gLightTranslation, gLightTranslation, -gLightTranslation));
  mLightPoint[2]->SetColor(Graphics::Color::eBlue);
  mLightPoint[2]->SetAttenuation(0.0f, 0.2f, 1.0f);
  mLightPoint[2]->SetRange(350.0f);
  mSceneManager->GetWorld()->Load(mLightPoint[2]);
  
  mLightPoint[3] = sceneManager->CreateObject(IObject::eObjectTypeLightPoint);
  mLightPoint[3]->SetPosition(Vector3f(gLightTranslation, gLightTranslation, -gLightTranslation));
  mLightPoint[3]->SetColor(Graphics::Color::eYellow);
  mLightPoint[3]->SetAttenuation(0.0f, 0.2f, 1.0f);
  mLightPoint[3]->SetRange(350.0f);
  mSceneManager->GetWorld()->Load(mLightPoint[3]);
}

void LightPointSample::Update()
{
  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());

  gLightAngle[0] += 0.001f  * step;
  gLightAngle[1] += 0.0015f * step;
  gLightAngle[2] += 0.0012f * step;
  gLightAngle[3] += 0.001f * step;

  mLightPoint[0]->Translate(Vector3f(0.0f, Math::Sin(gLightAngle[0]), 0.0f));
  mLightPoint[1]->Translate(Vector3f(0.0f, Math::Sin(gLightAngle[1]), 0.0f));
  mLightPoint[2]->Translate(Vector3f(0.0f, Math::Sin(gLightAngle[2]), 0.0f));
  mLightPoint[3]->Translate(Vector3f(0.0f, Math::Sin(gLightAngle[3]), 0.0f));
}
