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

// Created 21-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file SampleApplication.cpp
This file defines the SampleApplication class.
*/

#include <EngineTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
SampleApplication initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

SampleApplication::SampleApplication()
  : mInputManager(Application::Global::GetInputManager())
  , mSceneManager(Graphics::Scene::Global::GetSceneManager())
  , mCurrentSampleIndex(0) {}

SampleApplication::~SampleApplication()
{
  mSceneManager->Finalize();
}

void SampleApplication::Initialize(U32 width, U32 height)
{
  // Configure render settings
  Graphics::IRenderManager::Settings& renderSettings = Graphics::Global::GetRenderManager()->GetSettings();
  renderSettings.dataRootDirectory = "..\\..\\..\\Data\\";
  renderSettings.shaderFolder = "Shaders\\";
  renderSettings.textureFolder = "Textures\\";
  renderSettings.verticalSync = true;

  // Initialize scene manager
  mSceneManager->Initialize();
  ThrowIf(!mSceneManager->IsReady(), Exception::eExceptionTypeEngineTest);

  // Create main window
  Application::Application& app = Application::Global::GetApplication();
  mpWindow = app.CreateMainWindow(width, height);
  mpWindow->GetUpdateEventCallback() += this;
  mpWindow->GetKeyDownEventCallback() += this;
  mpWindow->GetFocusEventCallback() += this;
  mpWindow->GetFocusLostEventCallback() += this;

  const Application::IWindow::Descriptor& windowDesc = mpWindow->GetDescriptor();
  mSceneManager->SetView(
    Graphics::Scene::ISceneManager::eViewID0,
    windowDesc.handle,
    windowDesc.width,
    windowDesc.height,
    windowDesc.mode == Application::IWindow::eModeFullScreen);

  mMainView = mSceneManager->GetView(Graphics::Scene::ISceneManager::eViewID0);
  ThrowIf(mMainView == nullptr, Exception::eExceptionTypeEngineTest);


  // Add samples
  mSampleList.PushBack(&mTriangleSample);
  mSampleList.PushBack(&mVertexFormatSample);
  mSampleList.PushBack(&mMouseCameraSample);
  mSampleList.PushBack(&mChildMeshSample);
  mSampleList.PushBack(&mHighlightSample);
  mSampleList.PushBack(&mPhongSample);
  mSampleList.PushBack(&mMaterialSample);
  mSampleList.PushBack(&mLightSample);
  mSampleList.PushBack(&mLightPointSample);
  mSampleList.PushBack(&mLightSpotSample);
  mSampleList.PushBack(&mLightSpotShadowSample);
  mSampleList.PushBack(&mLightShadowSample);

  // Load start sample
  E_ASSERT(mCurrentSampleIndex < mSampleList.GetCount());
  mSampleList[mCurrentSampleIndex]->Load(mMainView);

  GDebugWindow::GetInstance().Initialize(width, height);

  app.Run();
}

/*----------------------------------------------------------------------------------------------------------------------
SampleApplication private methods
----------------------------------------------------------------------------------------------------------------------*/

void SampleApplication::OnEvent(const Application::FocusEvent&)
{
  // Link scene window to input manager
  mInputManager.SetActiveWindow(mpWindow);
  mSampleList[mCurrentSampleIndex]->SetActive(true);

}

void SampleApplication::OnEvent(const Application::FocusLostEvent&)
{
  mSampleList[mCurrentSampleIndex]->SetActive(false);
}

void SampleApplication::OnEvent(const Application::UpdateEvent&)
{
  // Update input
  //mInputManager->Update();

  // Update current sample
  mSampleList[mCurrentSampleIndex]->Update();

  // Graphics manager
  mSceneManager->Update();
}

void SampleApplication::OnEvent(const Application::KeyDownEvent& event)
{
  if (event.key == Application::Input::eKeyEscape)
  {
    Application::Global::GetApplication().Terminate();
  }
  else if (event.key == Application::Input::eKeySpace)
  {
    mSampleList[mCurrentSampleIndex]->Unload();
    if (++mCurrentSampleIndex == mSampleList.GetCount()) mCurrentSampleIndex = 0;
    mSampleList[mCurrentSampleIndex]->Load(mMainView);
  }

}