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

// Created 19-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LightSample.cpp
This file defines the LightSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
LightSample methods
----------------------------------------------------------------------------------------------------------------------*/

void LightSample::Load(IViewInstance window)
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
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
  
  // Create a cube mesh
  IMeshInstance groundPlane = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  IMaterialInstance groundMaterial = mSceneManager->CreateMaterial();
  groundMaterial->SetDiffuseColor(Graphics::Color(0.2f, 0.1f, 0.1f, 1.0f));
  groundMaterial->SetSpecularColor(Graphics::Color::eWhite);
  groundPlane->CreateQuad(100.0f);
  groundPlane->Rotate(Vector3f(90.0f, 0.0f, 0.0f));
  groundPlane->SetMaterial(groundMaterial);
  mSceneManager->GetWorld()->Load(groundPlane);
 
  IMeshInstance cube = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  cube->CreateCube(10.0f);
  cube->Translate(Vector3f(-20.0f, 5.0f, 0.0f));
  IMaterialInstance cubeMaterial = mSceneManager->CreateMaterial();
  cubeMaterial->SetDiffuseColor(Graphics::Color::eRed);
  cubeMaterial->SetSpecularColor(Graphics::Color::eWhite);
  cubeMaterial->SetSmoothness(0.02f);
  cube->SetMaterial(cubeMaterial);
  mSceneManager->GetWorld()->Load(cube);
 
  IMeshInstance sphere = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  sphere->CreateSphere(7.5f, 32, 32);
  sphere->Translate(Vector3f(0.0f, 7.5f, 0.0f));
  IMaterialInstance sphereMaterial = mSceneManager->CreateMaterial();
  sphereMaterial->SetDiffuseColor(Graphics::Color::eRed);
  sphereMaterial->SetSpecularColor(Graphics::Color::eWhite);
  sphereMaterial->SetSmoothness(0.02f);
  sphere->SetMaterial(sphereMaterial);
  mSceneManager->GetWorld()->Load(sphere);
  
  IMeshInstance cylinder = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  cylinder->CreateCylinder(4.0f, 4.0f, 20.0f, 16, 1);
  cylinder->Translate(Vector3f(20.0f, 10.0f, 0.0f));
  IMaterialInstance cylinderMaterial = mSceneManager->CreateMaterial();
  cylinderMaterial->SetDiffuseColor(Graphics::Color::eRed);
  cylinderMaterial->SetSpecularColor(Graphics::Color::eWhite);
  cylinderMaterial->SetSmoothness(0.02f);
  cylinder->SetMaterial(cylinderMaterial);
  mSceneManager->GetWorld()->Load(cylinder);
  
  mLight[0] = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight[0]->Rotate(Vector3f(60.0f, 45.0f, 45.0f));
  mLight[0]->SetColor(Graphics::Color(0.8f, 0.1f, 0.1f));
  mSceneManager->GetWorld()->Load(mLight[0]);
  
  mLight[1] = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight[1]->Rotate(Vector3f(120.0f, 45.0f, 45.0f));
  mLight[1]->SetColor(Graphics::Color(0.1f, 0.8f, 0.1f));
  mSceneManager->GetWorld()->Load(mLight[1]);
  
  mLight[2] = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight[2]->Rotate(Vector3f(120.0f, -45.0f, 45.0f));
  mLight[2]->SetColor(Graphics::Color(0.1f, 0.1f, 0.8f));
  mSceneManager->GetWorld()->Load(mLight[2]);
  
  mLight[3] = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight[3]->Rotate(Vector3f(60.0f, -45.0f, 45.0f));
  mLight[3]->SetColor(Graphics::Color(0.4f, 0.4f, 0.1f));
  mSceneManager->GetWorld()->Load(mLight[3]);
}

void LightSample::Update()
{
  Input::IInputManagerInstance pInputManager = Input::Global::GetInputManager();

  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (pInputManager->IsKeyDown(VK_ADD) || pInputManager->IsKeyDown(VK_SUBTRACT))
  {
    if (pInputManager->IsKeyDown(VK_SUBTRACT)) step = -step;
    mLight[0]->Rotate(Vector3f(0.0f, step * 0.1f, 0.0f));
  }

  mLight[0]->Rotate(Vector3f(0.0f, 0.035f * step, 0.0f));
  mLight[1]->Rotate(Vector3f(0.0f, 0.025f * step, 0.0f));
  mLight[2]->Rotate(Vector3f(0.0f, 0.040f * step, 0.0f));
  mLight[3]->Rotate(Vector3f(0.0f, 0.020f * step, 0.0f));
}
