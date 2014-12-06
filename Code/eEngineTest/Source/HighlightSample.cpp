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

/** @file HighlightSample.cpp
This file defines the HighlightSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
HighlightSample methods
----------------------------------------------------------------------------------------------------------------------*/

void HighlightSample::Load(IViewInstance window)
{
  mView = window;
  ISceneManagerInstance sceneManager = Global::GetSceneManager();

  // Create elements
  mCamera = sceneManager->CreateObject(IObject::eObjectTypeCamera);
  
  mCamera->Translate(Vector3f(0.0f, 0.0f, -7.5f));
  ILogicComponentInstance cameraLogicComponent = sceneManager->CreateComponent(IObjectComponent::eComponentTypeLogic);
  mCameraHandler.SetCamera(mCamera);
  mCameraHandler.SetSpeed(1.0f);
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
  
  // Create a cube mesh
  mCube = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  mCube->CreateCube(2.0f);
  mCubeMaterial = mSceneManager->CreateMaterial();
  mCubeMaterial->SetDiffuseColor(Graphics::Color(0.5f, 0.4f, 0.0f));
  mCubeMaterial->SetDiffuseTexture(mSceneManager->GetTexture2D("seafloor.dds"));
  mCubeMaterial->SetSpecularColor(Graphics::Color(0.05f, 0.04f, 0.04f));
  mCubeMaterial->SetSmoothness(0.56f);
  mCube->SetMaterial(mCubeMaterial);
  mSceneManager->GetWorld()->Load(mCube);

  mLight = sceneManager->CreateObject(IObject::eObjectTypeLight);
  mLight->SetColor(Graphics::Color::eWhite);
  mSceneManager->GetWorld()->Load(mLight);
}

void HighlightSample::Update()
{
  F32 step = static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (mInputManager.IsKeyUp(VK_ADD))
  {
    mCubeMaterial->SetSmoothness(mCubeMaterial->GetSmoothness() + 0.05f);
    mSceneManager->GetWorld()->Unload(mCube);
    mSceneManager->GetWorld()->Load(mCube);
  }
  else if (mInputManager.IsKeyUp(VK_SUBTRACT))
  {
    mCubeMaterial->SetSmoothness(mCubeMaterial->GetSmoothness() - 0.05f);
    mSceneManager->GetWorld()->Unload(mCube);
    mSceneManager->GetWorld()->Load(mCube);
  }

  mCube->Rotate(Vector3f(0.0f, step * 0.025f, 0.0f));
}
