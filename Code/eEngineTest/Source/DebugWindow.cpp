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

// Created 21-Oct-2014 by Elías Lozada-Benaventee
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file DebugWindow.cpp
This file defines the DebugWindow class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
----------------------------------------------------------------------------------------------------------------------*/

static const U32 kInvDebugViewToParentSizeRatio = 2;
static const F32 kDebugViewToParentSizeRatio = 1.0f / kInvDebugViewToParentSizeRatio;

/*----------------------------------------------------------------------------------------------------------------------
DebugWindow initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

DebugWindow::DebugWindow()
  : mSceneManager(Global::GetSceneManager())
  , mCameraHandlerOwner(&mCameraHandler) {}

DebugWindow::~DebugWindow() {}

void DebugWindow::Initialize(Win::Application& ownerApplication)
{
  // Create debug window
  Win::Application::WindowDescriptor debugViewDesc;
  debugViewDesc.windowTitle = "Debug View";
  debugViewDesc.windowWidth = ownerApplication.GetMainWindowDescriptor().windowWidth / kInvDebugViewToParentSizeRatio;
  debugViewDesc.windowHeight = ownerApplication.GetMainWindowDescriptor().windowHeight / kInvDebugViewToParentSizeRatio;
  debugViewDesc.windowMode = Win::Application::eWindowModeDefault;
  debugViewDesc.pWindowHandler = &GDebugView::GetInstance();
  ownerApplication.CreateChildWindow(debugViewDesc);
}

/*----------------------------------------------------------------------------------------------------------------------
DebugWindow methods
----------------------------------------------------------------------------------------------------------------------*/

void DebugWindow::OnWindowCreate(Ptr windowHandle, U32 windowWidth, U32 windowHeight, bool fullScreen)
{
  // Create debug window
  mSceneManager->SetView(
    Graphics::Scene::ISceneManager::eViewID1, 
    windowHandle, 
    windowWidth, 
    windowHeight, 
    fullScreen);

  mView = mSceneManager->GetView(ISceneManager::eViewID1);
  ThrowIf(mView == nullptr, Exception::eExceptionTypeEngineTest);

  mCamera = mSceneManager->CreateObject(IObject::eObjectTypeCamera);
  mCamera->SetProjectionType(ICamera::eProjectionTypeOrthographic);
  mCamera->SetViewportDimensions(windowWidth, windowHeight); // create viewport double size to match the main viewport
  mCamera->Translate(Vector3f(0.0f, 0.0f, -1.0f));
  ILogicComponentInstance cameraLogicComponent = mSceneManager->CreateComponent(IObjectComponent::eComponentTypeLogic);
  mCameraHandler.SetCamera(mCamera);
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
}

void DebugWindow::OnWindowDestroy()
{
  mView = nullptr;
}

void DebugWindow::OnWindowUpdate() {}
