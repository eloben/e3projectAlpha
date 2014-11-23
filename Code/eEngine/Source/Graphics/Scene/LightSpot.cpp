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

// Created 16-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LightSpot.cpp
This file defines the LightSpot class.
*/

#include <EnginePch.h>
#include "LightSpot.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary
----------------------------------------------------------------------------------------------------------------------*/

const F32 kDefaultNearPlane = 1.0f;
const F32 kDefaultFarPlane = 1000.0f;
const U32 kDefaultFov = 60;

/*----------------------------------------------------------------------------------------------------------------------
LightSpot initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

// Known warning: passing this in the initializer list to mCore
#pragma warning(push)
#pragma warning (disable:4355)
Graphics::Scene::LightSpot::LightSpot()
  : mCore(this)
  , mRenderManager(Graphics::Global::GetRenderManager())
  , mIntraFrameConstantBuffer(mRenderManager->GetConstantBuffer(eConstantBufferIDIntraFrame))
  , mColor(Graphics::Color::eWhite)
  , mDirection(0.0f, -0.5f, -0.5f)
  , mCutOffAngle(45.0f)
  , mRange(0.0f)
  , mLightID(static_cast<U32>(-1)) {}
#pragma warning(pop)

/*----------------------------------------------------------------------------------------------------------------------
LightSpot accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Color& Graphics::Scene::LightSpot::GetColor() const
{
  return mColor;
}

void Graphics::Scene::LightSpot::SetAttenuation(F32 a, F32 b, F32 c)
{
  mAttenuation.x = a;
  mAttenuation.y = b;
  mAttenuation.z = c;
}

void Graphics::Scene::LightSpot::SetColor(const Graphics::Color& color)
{
  mColor = color;
}

void Graphics::Scene::LightSpot::SetCutOffAngle(F32 angle)
{
  mCutOffAngle = Math::Clamp(Math::Normalize360(angle), 0.0f, 90.0f);
}


void Graphics::Scene::LightSpot::SetRange(F32 range)
{
  mAttenuation.w = range;
}

/*----------------------------------------------------------------------------------------------------------------------
LightSpot methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::LightSpot::Clear()
{
  mCore.ClearTransform();
}

void Graphics::Scene::LightSpot::Load()
{
  mCore.Load();

  mShadowComponent = mCore.GetComponent(IObjectComponent::eComponentTypeShadow);
}

void Graphics::Scene::LightSpot::Render()
{
  if (mShadowComponent) mIntraFrameConstantBuffer->Set(0, mShadowComponent->GetLightView()->GetViewProjectionMatrix());
  mIntraFrameConstantBuffer->Set(4, mDirection.x, mDirection.y, mDirection.z, 0.0f);
  mIntraFrameConstantBuffer->Set(5, mColor.r, mColor.g, mColor.b, mColor.a);
  mIntraFrameConstantBuffer->Set(6, mAttenuation);
  const Vector3f& position = mCore.GetWorldMatrix().GetTranslation();
  mIntraFrameConstantBuffer->Set(7, position.x, position.y, position.z, Math::Cos(Math::Rad(mCutOffAngle)));
  mRenderManager->Update(mIntraFrameConstantBuffer);

  mCore.RenderChildren();
}

void Graphics::Scene::LightSpot::Unload()
{
  mCore.Unload();
}

void Graphics::Scene::LightSpot::Update(const TimeValue& deltaTime)
{
  mCore.Update(deltaTime);

  mDirection = Matrix4f::RotateVector(mCore.GetWorldMatrix(), Vector3f::AxisZ());
}
