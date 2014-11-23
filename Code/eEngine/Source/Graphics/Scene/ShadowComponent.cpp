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

// Created 18-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ShadowComponent.cpp
This file defines the ShadowComponent class.
*/

#include <EnginePch.h>
#include "ShadowComponent.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
ShadowComponent auxiliary
----------------------------------------------------------------------------------------------------------------------*/

static const U32 kShadowMapSize = 1024;

/*----------------------------------------------------------------------------------------------------------------------
ShadowComponent initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::ShadowComponent::ShadowComponent() {}

/*----------------------------------------------------------------------------------------------------------------------
ShadowComponent accessors
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::IObjectComponent::ComponentType Graphics::Scene::ShadowComponent::GetComponentType() const
{ 
  return IObjectComponent::eComponentTypeShadow;
}

// const Graphics::ITexture2DInstance& Graphics::Scene::ShadowComponent::GetDepthTexture() const
// {
//   return mLightContext->GetDescriptor().colorTargets[0];
//  // return mLightContext->GetDescriptor().depthTarget;
// }

const Graphics::Scene::ICameraInstance& Graphics::Scene::ShadowComponent::GetLightView() const
{
  return mLightCamera;
  // return mLightContext->GetDescriptor().depthTarget;
}

const Graphics::Scene::IObjectInstance& Graphics::Scene::ShadowComponent::GetOwner() const
{
  return mOwner;
}

const Matrix4f& Graphics::Scene::ShadowComponent::GetViewProjectionMatrix() const
{
  return mLightCamera->GetViewProjectionMatrix();
}

void Graphics::Scene::ShadowComponent::SetOwner(const IObjectInstance& owner)
{
  mOwner = owner;
}

/*----------------------------------------------------------------------------------------------------------------------
ShadowComponent methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::ShadowComponent::OnLoad()
{
  mLightCamera = Graphics::Scene::Global::GetSceneManager()->CreateObject(IObject::eObjectTypeCamera);
  if (mOwner->GetObjectType() == IObject::eObjectTypeLight)
  {
    mLightCamera->SetProjectionType(ICamera::eProjectionTypeOrthographic);
    mLightCamera->SetNearFar(1.0f, 100.0f);
   // mOwner->SetPosition(Vector3f(0.0f, 0.0f, -10.0f));
    //mLightCamera->SetProjectionType(ICamera::eProjectionTypePerspective);
  }
  else
  {
    mLightCamera->SetProjectionType(ICamera::eProjectionTypePerspective);
  }
  mLightCamera->SetViewportDimensions(kShadowMapSize, kShadowMapSize);
  //mLightCamera->SetNearFar(1.0f, 100.0f);
}

void Graphics::Scene::ShadowComponent::OnUnload()
{

}

void Graphics::Scene::ShadowComponent::OnUpdate(const TimeValue& deltaTime)
{
  mLightCamera->SetPosition(mOwner->GetPosition());
  mLightCamera->SetOrientation(mOwner->GetOrientation());
  mLightCamera->Update(deltaTime);
}
