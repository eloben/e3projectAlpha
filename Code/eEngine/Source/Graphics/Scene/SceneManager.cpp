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

// Created 14-Sep-2014 by Elías Lozada-Benavente
// Original created 04-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file SceneManager.cpp
This file defines the SceneManager class.
*/

#include <EnginePch.h>
#include "SceneManager.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
SceneManager assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_SCENE_MANAGER_DEFINED_VIEW       "Graphics view (%d) is already defined"
#define E_ASSERT_MSG_SCENE_MANAGER_UNDEFINED_RENDERER "Renderer type is not defined"
#define E_ASSERT_MSG_SCENE_MANAGER_UNDEFINED_VIEW     "Graphics view (%d) is not defined"
#define E_ASSERT_MSG_SCENE_MANAGER_READY              "Graphics manager is not initialized"

/*----------------------------------------------------------------------------------------------------------------------
SceneManager auxiliary
----------------------------------------------------------------------------------------------------------------------*/

static const E::Graphics::IShaderInstance kEmptyShader;

static size_t kResourceBufferSizeTable[] =
{
  64,   //  Transform: (float4x4)
  32,   //  Material: (float4) diffuse, (float4) specular + specular power
//   24,   //  Directional light: (float3) dir + (float3) color
//   40,   //  (float3) color + (float3) pos + (float4) attenuation + range
//   120    //  Spot light: (float3) dir + (float3) color + (float3) pos + (float4) attenuation + range + (float) cosCutOffAngle + (float4x4) lightVP
};

/*----------------------------------------------------------------------------------------------------------------------
SceneManager initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::SceneManager::SceneManager()
  : mInputManager(Application::Global::GetInputManager())
  , mRenderManager(Graphics::Global::GetRenderManager())
{
  // Register types
  mRendererFactory.Register(&mForwardRendererFactory, IRenderer::eRendererTypeForward);
  mObjectFactory.Register(&mCameraFactory, IObject::eObjectTypeCamera);
  mObjectFactory.Register(&mMeshFactory, IObject::eObjectTypeMesh);
  mObjectFactory.Register(&mLightFactory, IObject::eObjectTypeLight);
  mObjectFactory.Register(&mLightPointFactory, IObject::eObjectTypeLightPoint);
  mObjectFactory.Register(&mLightSpotFactory, IObject::eObjectTypeLightSpot);
  mComponentFactory.Register(&mLogicComponentFactory, IObjectComponent::eComponentTypeLogic);
  mComponentFactory.Register(&mShadowComponentFactory, IObjectComponent::eComponentTypeShadow);
}

Graphics::Scene::SceneManager::~SceneManager()
{
  // Unregister types
  mComponentFactory.Unregister(&mShadowComponentFactory);
  mComponentFactory.Unregister(&mLogicComponentFactory);
  mObjectFactory.Unregister(&mLightSpotFactory);
  mObjectFactory.Unregister(&mLightPointFactory);
  mObjectFactory.Unregister(&mLightFactory);
  mObjectFactory.Unregister(&mMeshFactory);
  mObjectFactory.Unregister(&mCameraFactory);
  mRendererFactory.Unregister(&mForwardRendererFactory);
}

bool Graphics::Scene::SceneManager::Initialize()
{
  if (!mRenderManager->Initialize()) return false;

  mRenderManager->SetVertexLayout(IMesh::eVertexTypePosition, PositionVertex::GetVertexLayoutDescriptor());
  mRenderManager->SetVertexLayout(IMesh::eVertexTypePositionTexture, CompressedPositionTextureVertex::GetVertexLayoutDescriptor());
  mRenderManager->SetVertexLayout(IMesh::eVertexTypePositionNormal, CompressedPositionNormalVertex::GetVertexLayoutDescriptor());
  mRenderManager->SetVertexLayout(IMesh::eVertexTypePositionTextureNormal, CompressedPositionTextureNormalVertex::GetVertexLayoutDescriptor());

  mRenderManager->SetResourceBuffer(eResourceBufferIDTransform, kResourceBufferSizeTable[eResourceBufferIDTransform]);
  mRenderManager->SetResourceBuffer(eResourceBufferIDMaterial, kResourceBufferSizeTable[eResourceBufferIDMaterial]);

  mRenderer = mRendererFactory.Create(IRenderer::eRendererTypeForward);

  mWorld = mWorldFactory.Create();

  mDefaultMaterial = mMaterialFactory.Create();

  return true;
}

void Graphics::Scene::SceneManager::Finalize()
{
  // Finalize render manager first (terminate rendering thread)
  mRenderManager->Finalize();
  // Finalize windows
  for (U32 i = 0; i < eViewIDCount; ++i) mViews[i] = nullptr;
  // Clean up factories
  mObjectFactory.CleanUp();
  mComponentFactory.CleanUp();
  mMaterialFactory.CleanUp();
  mViewFactory.CleanUp();
  mWorldFactory.CleanUp();
  mRendererFactory.CleanUp();
}

bool Graphics::Scene::SceneManager::IsReady() const
{
  return mRenderManager->IsReady();
}

/*----------------------------------------------------------------------------------------------------------------------
SceneManager accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Scene::IMaterialInstance& Graphics::Scene::SceneManager::GetDefaultMaterial() const
{
  return mDefaultMaterial;
}

const Graphics::Scene::IRendererInstance& Graphics::Scene::SceneManager::GetRenderer() const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mRenderer;
}

const Graphics::ITexture2DInstance& Graphics::Scene::SceneManager::GetTexture2D(const FilePath& filePath) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mRenderManager->GetTexture2D(filePath);
}
         
const Graphics::Scene::IViewInstance& Graphics::Scene::SceneManager::GetView(U32 viewID) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  E_ASSERT_MSG(mViews[viewID] != nullptr, E_ASSERT_MSG_SCENE_MANAGER_UNDEFINED_VIEW, viewID);
  return mViews[viewID];
}

const Graphics::Scene::IWorldInstance& Graphics::Scene::SceneManager::GetWorld() const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mWorld;
}

void Graphics::Scene::SceneManager::SetRenderer(IRenderer::RendererType type)
{
  if (mRenderer->GetRendererType() != type) mRenderer = mRendererFactory.Create(type);
}

void Graphics::Scene::SceneManager::SetView(U32 viewID, Ptr windowHandle, U32 viewWidth, U32 viewHeight, bool fullScreen)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  E_ASSERT_MSG(mViews[viewID] == nullptr, E_ASSERT_MSG_SCENE_MANAGER_DEFINED_VIEW, viewID);
    
  // Create window viewport
  mRenderManager->SetViewport(viewID, windowHandle, viewWidth, viewHeight, fullScreen);
  // Create window
  ViewInstance window = mViewFactory.Create();
  E_ASSERT_PTR(window);
  window->Initialize(mRenderManager->GetViewport(viewID), mRenderManager->GetFrameBuffer(viewID));
  // Store window
  mViews[viewID] = window;
}

/*----------------------------------------------------------------------------------------------------------------------
SceneManager methods
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::IObjectComponentInstance Graphics::Scene::SceneManager::CreateComponent(IObjectComponent::ComponentType type)
{
  E_DEBUG_MSG("[SceneManager] IObjectComponentInstance: [%d]", mComponentFactory.GetLiveCount() + 1);
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mComponentFactory.Create(type);
}

Graphics::Scene::IMaterialInstance Graphics::Scene::SceneManager::CreateMaterial()
{
  E_DEBUG_MSG("[SceneManager] IMaterialInstance: [%d]", mMaterialFactory.GetLiveCount() + 1);
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mMaterialFactory.Create();
}

Graphics::Scene::IObjectInstance Graphics::Scene::SceneManager::CreateObject(IObject::ObjectType type)
{
  E_DEBUG_MSG("[SceneManager] IObjectInstance: [%d]", mObjectFactory.GetLiveCount() + 1);
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mObjectFactory.Create(type);
}

Graphics::Scene::IObjectGroupInstance Graphics::Scene::SceneManager::CreateGroup()
{
  E_DEBUG_MSG("[SceneManager] IObjectGroupInstance: [%d]", mGroupFactory.GetLiveCount() + 1);
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_SCENE_MANAGER_READY);
  return mGroupFactory.Create();
}

void Graphics::Scene::SceneManager::Update()
{
  mInputManager.Update();

  // Get elapsed time
  TimeValue currentTime = mTimer.GetElapsed();
  TimeValue deltaTime = currentTime - mPreviousTime;    
  // Update world
  mWorld->Update(deltaTime);
  // Update views
  for (U32 i = 0; i < eViewIDCount; ++i)
  {
    if (mViews[i])
    {
     // mInputManager.SetCurrentViewport(mViews[i]->GetViewState().viewport);
      mViews[i]->Update(deltaTime);  
    }
  }
  // Render views
  for (U32 i = 0; i < eViewIDCount; ++i) if (mViews[i]) mRenderer->Render(mViews[i], mWorld);
  // Update last time
  mPreviousTime = currentTime;
}
