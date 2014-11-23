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
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file MouseCameraSample.cpp
This file defines the MouseCameraSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
MouseCameraSample initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

MouseCameraSample::MouseCameraSample()
  : mCameraHandlerOwner(&mCameraHandler) {}

/*----------------------------------------------------------------------------------------------------------------------
MouseCameraSample methods
----------------------------------------------------------------------------------------------------------------------*/

void MouseCameraSample::Load(IViewInstance window)
{
  mView = window;

  // Create elements
  mCamera = mSceneManager->CreateObject(IObject::eObjectTypeCamera);
  
  mCamera->Translate(Vector3f(0.0f, 0.0f, -0.02f * mCamera->GetFar()));
  ILogicComponentInstance cameraLogicComponent = mSceneManager->CreateComponent(IObjectComponent::eComponentTypeLogic);
  mCameraHandler.SetCamera(mCamera);
  mCameraHandler.SetSpeed(20.0f);
  mCameraHandler.SetSensitivity(0.3f);
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);

  // Create a cube mesh
  IMeshInstance mesh = mSceneManager->CreateObject(IObject::eObjectTypeMesh);
  mesh->CreateCube(10.0f);
  mesh->Rotate(Vector3f(0.0f, 45.0f, 0.0f));
  IMaterialInstance material = mSceneManager->CreateMaterial();
  material->SetDiffuseTexture(mSceneManager->GetTexture2D("test.png"));
  mesh->SetMaterial(material);

  mSceneManager->GetWorld()->Load(mesh);
}
