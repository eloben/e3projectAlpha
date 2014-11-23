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

// Created 17-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LightPoint.cpp
This file defines the LightPoint class.
*/

#include <EnginePch.h>
#include "LightPoint.h"

using namespace E;


/*----------------------------------------------------------------------------------------------------------------------
LightPoint initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

// Known warning: passing this in the initializer list to mCore
#pragma warning(push)
#pragma warning (disable:4355)
Graphics::Scene::LightPoint::LightPoint()
  : mCore(this)
  , mRenderManager(Graphics::Global::GetRenderManager())
  , mIntraFrameConstantBuffer(mRenderManager->GetConstantBuffer(eConstantBufferIDIntraFrame))
  , mColor(Graphics::Color::eWhite)
  , mAttenuation(0.0f, 0.2f, 1.0f, 10.0f)
  , mLightID(static_cast<U32>(-1)) {}
#pragma warning(pop)

/*----------------------------------------------------------------------------------------------------------------------
LightPoint accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Color& Graphics::Scene::LightPoint::GetColor() const
{
  return mColor;
}

void Graphics::Scene::LightPoint::SetAttenuation(F32 a, F32 b, F32 c)
{
  mAttenuation.x = a;
  mAttenuation.y = b;
  mAttenuation.z = c;
}

void Graphics::Scene::LightPoint::SetColor(const Graphics::Color& color)
{
  mColor = color;
}

void Graphics::Scene::LightPoint::SetRange(F32 range)
{
  mAttenuation.w = range;
}

/*----------------------------------------------------------------------------------------------------------------------
LightPoint methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::LightPoint::Clear()
{
  mCore.ClearTransform();
}

void Graphics::Scene::LightPoint::Load()
{
  mCore.Load();
}

void Graphics::Scene::LightPoint::Render()
{
  mIntraFrameConstantBuffer->Set(5, mColor.r, mColor.g, mColor.b, mColor.a);
  mIntraFrameConstantBuffer->Set(6, mAttenuation);
  mIntraFrameConstantBuffer->Set(7, mCore.GetWorldMatrix().GetTranslation(), 0);
  mRenderManager->Update(mIntraFrameConstantBuffer);

  mCore.RenderChildren();
}

void Graphics::Scene::LightPoint::Unload()
{
  // Release light ID
 // mRenderManager->GetResourceBuffer(eResourceBufferIDLight)->ReleaseIndex(mLightID);
}

void Graphics::Scene::LightPoint::Update(const TimeValue& deltaTime)
{
  mCore.Update(deltaTime);
}
