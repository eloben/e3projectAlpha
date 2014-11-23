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

/** @file ChildMeshSample.cpp
This file defines the ChildMeshSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
ChildMeshSample methods
----------------------------------------------------------------------------------------------------------------------*/

ChildMeshSample::ChildMeshSample()
  : mScale(1)
  , mSpeed(0.1f)
  , mSelectedOperation(0)
  , mSelectedAxis(0) {}

void ChildMeshSample::Load(IViewInstance window)
{
  mView = window;
  ISceneManagerInstance sceneManager = Global::GetSceneManager();

  // Create elements
  mCamera = sceneManager->CreateObject(IObject::eObjectTypeCamera);
  
  mCamera->Translate(Vector3f(0.0f, 50.0f, -0.5f * mCamera->GetFar()));
  mView->SetCamera(mCamera);

  // Create a cube mesh
  mMesh = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  mMesh->CreateCube(200.0f);
  IMaterialInstance material = mSceneManager->CreateMaterial();
  material->SetDiffuseTexture(mSceneManager->GetTexture2D("test.png"));
  mMesh->SetMaterial(material);
  
  // Create a child sphere mesh
  /*IMeshInstance*/ childMesh = sceneManager->CreateObject(IObject::eObjectTypeMesh);
  childMesh->CreateSphere(75.0f, 16, 16);
  childMesh->Translate(Vector3f(0.0f, 200.0f, 0.0f));
  childMesh->SetMaterial(material);
  // Do not load child manually!
  
  // Add parent
  mMesh->AddChild(childMesh);

  /*
  // Children test operations
  mMesh->RemoveChildren();
  mMesh->AddChild(childMesh);
  mMesh->RemoveChild(childMesh);
  mMesh->AddChild(childMesh);

  // Invalid operations
  IMeshInstance nullInstance;
  childMesh->SetParent(nullInstance); // Modify parent of a child
  mMesh->SetParent(childMesh);        // Set parent to an orphan.
  */

  mSceneManager->GetWorld()->Load(mMesh);
}

void ChildMeshSample::Update()
{
  Input::IInputManagerInstance pInputManager = Input::Global::GetInputManager();

  F32 step = mSpeed * static_cast<F32>(mTimer.Reset().GetMilliseconds());
  if (pInputManager->IsKeyDown('W')) mCamera->Translate(Vector3f(0.0f, step, 0.0f));
  if (pInputManager->IsKeyDown('S')) mCamera->Translate(Vector3f(0.0f, -step, 0.0f));
  if (pInputManager->IsKeyDown('A')) mCamera->Translate(Vector3f(-step, 0.0f, 0.0f));
  if (pInputManager->IsKeyDown('D')) mCamera->Translate(Vector3f(step, 0.0f, 0.0f));
  if (pInputManager->IsKeyDown('Q')) mCamera->Translate(Vector3f(0.0f, 0.0f, step));
  if (pInputManager->IsKeyDown('E')) mCamera->Translate(Vector3f(0.0f, 0.0f, -step));
  if (pInputManager->IsKeyDown('R'))
  {
    mOrientation.Set(0);
    mScale.Set(1);
    mPosition.Set(0);
  }

  if (pInputManager->IsKeyReleased('1')) mSelectedAxis = 0;
  if (pInputManager->IsKeyReleased('2')) mSelectedAxis = 1;
  if (pInputManager->IsKeyReleased('3')) mSelectedAxis = 2;
  if (pInputManager->IsKeyReleased('4')) mSelectedOperation = 0;
  if (pInputManager->IsKeyReleased('5')) mSelectedOperation = 1;
  if (pInputManager->IsKeyReleased('6')) mSelectedOperation = 2;

  if (pInputManager->IsKeyDown(VK_ADD) || pInputManager->IsKeyDown(VK_SUBTRACT))
  {
    if (pInputManager->IsKeyDown(VK_SUBTRACT)) step = -step;

    // Rotation
    if (mSelectedOperation == 0)
    {
      switch (mSelectedAxis)
      {
      case 0: mOrientation.x += step; break;
      case 1: mOrientation.y += step; break;
      case 2: mOrientation.z += step; break;
      }
    }
    // Scaling
    else if (mSelectedOperation == 1)
    {
      step *= 0.1f;

      switch (mSelectedAxis)
      {
      case 0: mScale.x += step * 0.1f; break;
      case 1: mScale.y += step * 0.1f; break;
      case 2: mScale.z += step * 0.1f; break;
      }
    }
    // Translation
    else
    {
      switch (mSelectedAxis)
      {
      case 0: mPosition.x += step; break;
      case 1: mPosition.y += step; break;
      case 2: mPosition.z += step; break;
      }
    }
  }

  // Update mesh
  mMesh->SetOrientation(mOrientation);
  mMesh->SetPosition(mPosition);
  mMesh->SetScale(mScale);
}
