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
  : mInputManager(Application::Global::GetInputManager())
  , mSceneManager(Global::GetSceneManager())
  , mCameraHandlerOwner(&mCameraHandler) {}

DebugWindow::~DebugWindow() {}

void DebugWindow::Initialize(U32 width, U32 height)
{
/*
  / ** /
  // Create debug window
  Win::Application::WindowDescriptor debugViewDesc;
  debugViewDesc.windowTitle = "Debug View";
  debugViewDesc.width = ownerApplication.GetMainWindowDescriptor().width / kInvDebugViewToParentSizeRatio;
  debugViewDesc.height = ownerApplication.GetMainWindowDescriptor().height / kInvDebugViewToParentSizeRatio;
  debugViewDesc.windowMode = Win::Application::eWindowModeDefault;
  debugViewDesc.pWindowHandler = &GDebugWindow::GetInstance();
  ownerApplication.CreateChildWindow(debugViewDesc);*/

  // Create main window
  Application::Application& app = Application::Global::GetApplication();
  mpWindow = app.CreateChildWindow(width, height);
  mpWindow->GetUpdateEventCallback() += this;
  mpWindow->GetKeyDownEventCallback() += this;
  mpWindow->GetFocusEventCallback() += this;
  mpWindow->GetFocusLostEventCallback() += this;

  const Application::IWindow::Descriptor& windowDesc = mpWindow->GetDescriptor();
  mSceneManager->SetView(
    Graphics::Scene::ISceneManager::eViewID1,
    windowDesc.handle,
    windowDesc.width,
    windowDesc.height,
    false);

  mView = mSceneManager->GetView(ISceneManager::eViewID1);
  ThrowIf(mView == nullptr, Exception::eExceptionTypeEngineTest);

  mCamera = mSceneManager->CreateObject(IObject::eObjectTypeCamera);
  //mCamera->SetProjectionType(ICamera::eProjectionTypeOrthographic);
  mCamera->SetProjectionType(ICamera::eProjectionTypePerspective);
  mCamera->SetViewportDimensions(width, height); // create viewport double size to match the main viewport
  mCamera->Translate(Vector3f(0.0f, 0.0f, -40.0f));
  ILogicComponentInstance cameraLogicComponent = mSceneManager->CreateComponent(IObjectComponent::eComponentTypeLogic);
  mCameraHandler.SetCamera(mCamera);
  mCameraHandler.SetSpeed(50.0f);
  cameraLogicComponent->SetHandler(mCameraHandlerOwner);
  mCamera->AddComponent(cameraLogicComponent);
  mView->SetCamera(mCamera);
}

/*----------------------------------------------------------------------------------------------------------------------
DebugWindow private methods
----------------------------------------------------------------------------------------------------------------------*/

void DebugWindow::OnEvent(const Application::FocusEvent&)
{
  mInputManager.SetActiveWindow(mpWindow);
  mCameraHandler.SetActive(true);
}

void DebugWindow::OnEvent(const Application::FocusLostEvent&)
{
  mCameraHandler.SetActive(false);
}
void DebugWindow::OnEvent(const Application::UpdateEvent&)
{

}

void DebugWindow::OnEvent(const Application::KeyDownEvent& event)
{
  if (event.key == Application::Input::eKeyEscape)
  {
    Application::Global::GetApplication().DestroyWindow(mpWindow);
  }
  if (event.key == Application::Input::eKeyR)
  {
    mCamera->SetOrientation(Vector3f(0.0f, 0.0f, 0.0f));
    mCamera->SetPosition(Vector3f(0.0f, 0.0f, -50.0f));
  }

//   switch (event.key)
//   {
//   case Application::Input::eKeyW: mCameraHandler.Move(CameraHandler::eMoveDirectionForward);
//     break;
//   case Application::Input::eKeyS: mCameraHandler.Move(CameraHandler::eMoveDirectionBackwards);
//     break;
//   case Application::Input::eKeyA: mCameraHandler.Move(CameraHandler::eMoveDirectionLeft);
//     break;
//   case Application::Input::eKeyD: mCameraHandler.Move(CameraHandler::eMoveDirectionRight);
//     break;
//   }
}
