/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 RenderManager

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

// Created 14-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file View.cpp
This file defines the View class.
*/

#include <EnginePch.h>
#include "View.h"
#include "ScenePipeline.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
View assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_OBJECT_LOADED      "Parameter object is already loaded"
#define E_ASSERT_MSG_OBJECT_NOT_LOADED  "Parameter object is not loaded"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------------
View initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::View::View() {}

void Graphics::Scene::View::Initialize(const Graphics::IViewportInstance& viewport, const Graphics::IRenderTargetInstance& frameBuffer)
{
  mViewState.viewport = viewport;
  mViewState.frameBuffer = frameBuffer;
}

void Graphics::Scene::View::Finalize()
{
  if (mViewState.viewport)
  {
    mViewState.Clear();
  }
}

/*----------------------------------------------------------------------------------------------------------------------
View accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Scene::ViewState& Graphics::Scene::View::GetViewState() const
{
  return mViewState;
}

void Graphics::Scene::View::SetCamera(const ICameraInstance& camera)
{
  if (camera != nullptr && camera != mViewState.camera)
  {
    mViewState.camera = camera;
    const IViewport::Descriptor& viewportDesc = mViewState.viewport->GetDescriptor();
    mViewState.camera->SetViewportDimensions(viewportDesc.width, viewportDesc.height);
  }
}

/*----------------------------------------------------------------------------------------------------------------------
View methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::View::Update(const TimeValue& deltaTime)
{
  if (mViewState.camera) mViewState.camera->Update(deltaTime);   
}
