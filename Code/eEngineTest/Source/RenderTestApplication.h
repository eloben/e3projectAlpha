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

// Created 28-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file RenderTestApplication.h
This file declares the RenderTestApplication class.
*/

#ifndef E3_RENDER_TEST_APPLICATION_H
#define E3_RENDER_TEST_APPLICATION_H

namespace E
{
  class RenderTestApplication : private EventSystem::IEventHandler
  {
  public:
    RenderTestApplication();
    ~RenderTestApplication();

    void    		                          Initialize(U32 width, U32 height);


  private:
    friend Application::IWindow::KeyDownEventCallback;
    friend Application::IWindow::UpdateEventCallback;

    Application::IWindow*                 mpWindow;
    Graphics::IRenderManagerInstance      mRenderManager;
    Graphics::IBlendStateInstance         mBlendState;
    Graphics::IDepthStencilStateInstance  mDepthStencilState;
    Graphics::IRasterStateInstance        mRasterState;
    Graphics::IShaderInstance             mShader;
    Graphics::IConstantBufferInstance     mConstantBuffer;
    Graphics::IResourceBufferInstance     mTransformBuffer;
    Graphics::VertexArray                 mVertexArray;
    Graphics::DrawState                   mDrawState;
    Graphics::IViewportInstance           mViewport;
    Graphics::IRenderTargetInstance       mViewportContext;

    void                                  LoadTriangle();

    // Callback methods
    void                                  OnEvent(const Application::UpdateEvent& event);
    void                                  OnEvent(const Application::KeyDownEvent& event);

 
    E_DISABLE_COPY_AND_ASSSIGNMENT(RenderTestApplication);
  };
}
#endif

