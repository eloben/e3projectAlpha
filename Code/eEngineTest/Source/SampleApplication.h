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

/** @file SampleApplication.h
This file declares the SampleApplication class.
*/

#ifndef E3_SAMPLE_APPLICATION_H
#define E3_SAMPLE_APPLICATION_H

namespace E
{
  class SampleApplication : private EventSystem::IEventHandler
  {
  public:
    SampleApplication();
    ~SampleApplication();

    void    		                      Initialize(U32 windowWidth, U32 windowHeight);

  private:
    friend Application::IWindow::KeyDownEventCallback;
    friend Application::IWindow::UpdateEventCallback;

    typedef Containers::List<ISceneSample*> SampleList;

    SampleList                        mSampleList;
    Application::IWindow*             mpWindow;
    Graphics::Scene::ISceneManagerInstance   mSceneManager;
    Input::IInputManagerInstance      mInputManager;
    Graphics::Scene::IViewInstance         mMainView;
    U32                               mCurrentSampleIndex;
    TriangleSample                    mTriangleSample, mTriangleSample2;
    VertexFormatSample                mVertexFormatSample;
    ChildMeshSample                   mChildMeshSample;
    MouseCameraSample                 mMouseCameraSample;
    HighlightSample                   mHighlightSample;
    PhongSample                       mPhongSample;
    MaterialSample                    mMaterialSample;
    LightSample            mLightSample;
    LightPointSample                  mLightPointSample;
    LightSpotSample                   mLightSpotSample;
    LightSpotShadowSample             mLightSpotShadowSample;
    LightShadowSample            mLightShadowSample;
    // Callback methods
    void                                  OnEvent(const Application::UpdateEvent& event);
    void                                  OnEvent(const Application::KeyDownEvent& event);

    E_DISABLE_COPY_AND_ASSSIGNMENT(SampleApplication);
  };
}
#endif
