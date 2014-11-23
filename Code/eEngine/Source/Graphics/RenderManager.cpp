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

// Created 04-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file RenderManager.cpp
This file defines the RenderManager class.
*/

#include <EnginePch.h>
#include "RenderManager.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
RenderManager assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_RENDER_MANAGER_READY                                 "Render Manager is not initialized"
#define E_ASSERT_MSG_RENDER_MANAGER_DEFINED_CUSTOM_RESOURCE_BUFFER        "Resource buffer (%d) is already defined"
#define E_ASSERT_MSG_RENDER_MANAGER_DEFINED_CUSTOM_SHADER_TECHNIQUE       "Shader technique (%s) is already defined"
#define E_ASSERT_MSG_RENDER_MANAGER_DEFINED_CUSTOM_VERTEX_LAYOUT          "Vertex layout (%d) is already defined"
#define E_ASSERT_MSG_RENDER_MANAGER_DEFINED_VIEW_WINDOW                   "View (%d) is already defined"
#define E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_CUSTOM_RESOURCE_BUFFER      "Resource buffer (%d) is undefined"
#define E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_CUSTOM_SHADER_TECHNIQUE     "Shader technique (%s) is undefined"
#define E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_CUSTOM_VERTEX_LAYOUT        "Vertex layout (%d) is undefined"
#define E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_VIEWPORT                    "Viewport (%d) is not defined"

/*----------------------------------------------------------------------------------------------------------------------
RenderManager auxiliary
----------------------------------------------------------------------------------------------------------------------*/

static const Graphics::IShaderInstance kEmptyShader;

namespace E
{
  namespace Containers
  {
    /*----------------------------------------------------------------------------------------------------------------------
    MapHasher<Graphics::IBlendState::Descriptor>
    ----------------------------------------------------------------------------------------------------------------------*/
    template <>
    struct MapHasher<Graphics::IBlendState::Descriptor>
    {
      typedef Graphics::IBlendState::Descriptor Reference;
      inline static size_t Hash(const Reference& key)
      { 
        return 
          key.sourceFactor << 24 ^ 
          key.destinationFactor << 20 ^
          key.blendFunction << 17 ^
          key.alphaSourceFactor << 13 ^
          key.alphaDestinationFactor << 9 ^
          key.alphaBlendFunction << 6 ^
          key.writeMask << 2 ^
          key.flags;
      }
      inline static void Invalidate(Reference& key) { key.flags = static_cast<U8>(-1); }
      inline static bool IsEqual(const Reference& key1, const Reference& key2)
      { 
        return 
          key1.sourceFactor == key2.sourceFactor &&
          key1.destinationFactor == key2.destinationFactor &&
          key1.blendFunction == key2.blendFunction &&
          key1.alphaSourceFactor == key2.alphaSourceFactor &&
          key1.alphaDestinationFactor == key2.alphaDestinationFactor &&
          key1.alphaBlendFunction == key2.alphaBlendFunction &&
          key1.writeMask == key2.writeMask &&
          key1.flags == key2.flags;
      }
      inline static bool IsValid(const Reference& key) { return key.flags != static_cast<U8>(-1); }
    };

    /*----------------------------------------------------------------------------------------------------------------------
    MapHasher<Graphics::IDepthStencilState::Descriptor>

    Please note that this interface has the following usage contract:

    1. Hash computation does not include stencilReadMask or stencilWriteMask
    ----------------------------------------------------------------------------------------------------------------------*/
    template <>
    struct MapHasher<Graphics::IDepthStencilState::Descriptor>
    {
      typedef Graphics::IDepthStencilState::Descriptor Reference;
      inline static size_t Hash(const Reference& key)
      { 
        // Note that stencilReadMask & stencilWriteMask are omitted in the hash computation
        return 
          key.depthFunction << 27 ^ 
          key.backFaceFailStencilFunction << 24 ^
          key.backFaceDepthFailStencilFunction << 21 ^
          key.backFacePassStencilFunction << 18 ^
          key.backFaceStencilComparisonFunction << 15 ^
          key.frontFaceFailStencilFunction << 12 ^
          key.frontFaceDepthFailStencilFunction << 9 ^
          key.frontFacePassStencilFunction << 6 ^
          key.frontFaceStencilComparisonFunction << 3 ^
          key.flags;
      }
      inline static void Invalidate(Reference& key) { key.flags = static_cast<U8>(-1); }
      inline static bool IsEqual(const Reference& key1, const Reference& key2)
      { 
        return 
          key1.depthFunction == key2.depthFunction &&
          key1.backFaceFailStencilFunction == key2.backFaceFailStencilFunction &&
          key1.backFaceDepthFailStencilFunction == key2.backFaceDepthFailStencilFunction &&
          key1.backFacePassStencilFunction == key2.backFacePassStencilFunction &&
          key1.backFaceStencilComparisonFunction == key2.backFaceStencilComparisonFunction &&
          key1.frontFaceFailStencilFunction == key2.frontFaceFailStencilFunction &&
          key1.frontFaceDepthFailStencilFunction == key2.frontFaceDepthFailStencilFunction &&
          key1.frontFacePassStencilFunction == key2.frontFacePassStencilFunction &&
          key1.frontFaceStencilComparisonFunction == key2.frontFaceStencilComparisonFunction &&
          key1.stencilReadMask == key2.stencilReadMask &&
          key1.stencilWriteMask == key2.stencilWriteMask &&
          key1.flags == key2.flags;
      }
      inline static bool IsValid(const Reference& key) { return key.flags != static_cast<U8>(-1); }
    };

    /*----------------------------------------------------------------------------------------------------------------------
    MapHasher<Graphics::IRasterState::Descriptor>

    Please note that this interface has the following usage contract:

    1. Hash computation does not include depthBias, depthBiasClamp or slopeScaledDepthBias
    ----------------------------------------------------------------------------------------------------------------------*/
    template <>
    struct MapHasher<Graphics::IRasterState::Descriptor>
    {
      typedef Graphics::IRasterState::Descriptor Reference;
      inline static size_t Hash(const Reference& key)
      { 
        return 
          key.cullMode << 6 ^ 
          key.fillMode << 5 ^
          key.flags;
      }
      inline static void Invalidate(Reference& key) { key.flags = static_cast<U8>(-1); }
      inline static bool IsEqual(const Reference& key1, const Reference& key2)
      { 
        return 
          key1.cullMode == key2.cullMode &&
          key1.fillMode == key2.fillMode &&
          key1.depthBias == key2.depthBias &&
          key1.depthBiasClamp == key2.depthBiasClamp &&
          key1.slopeScaledDepthBias == key2.slopeScaledDepthBias &&
          key1.flags == key2.flags;
      }
      inline static bool IsValid(const Reference& key) { return key.flags != static_cast<U8>(-1); }
    };

    /*----------------------------------------------------------------------------------------------------------------------
    MapHasher<Graphics::ISampler::Descriptor>

    Please note that this interface has the following usage contract:

    1. Hash computation does not include borderColor, maxLod, minLod, mipLodBias or maxAnisotropy
    ----------------------------------------------------------------------------------------------------------------------*/
    template <>
    struct MapHasher<Graphics::ISampler::Descriptor>
    {
      typedef Graphics::ISampler::Descriptor Reference;
      inline static size_t Hash(const Reference& key)
      { 
        return 
          key.filter ^ 
          key.addressU << 5 ^
          key.addressV << 8 ^
          key.addressW << 11 ^
          key.comparisonFunction << 14;
      }
      inline static void Invalidate(Reference& key) { key.maxAnisotropy = 0; }
      inline static bool IsEqual(const Reference& key1, const Reference& key2)
      { 
        return 
          key1.filter == key2.filter &&
          key1.addressU == key2.addressU &&
          key1.addressV == key2.addressV &&
          key1.addressW == key2.addressW &&
          key1.comparisonFunction == key2.comparisonFunction &&
          key1.borderColor == key2.borderColor &&
          key1.maxLod == key2.maxLod &&
          key1.minLod == key2.minLod &&
          key1.mipLodBias == key2.mipLodBias &&
          key1.maxAnisotropy == key2.maxAnisotropy;
      }
      inline static bool IsValid(const Reference& key) { return key.maxAnisotropy != 0; }
    };
  }
}

/*----------------------------------------------------------------------------------------------------------------------
RenderManager initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::RenderManager::RenderManager() 
: mDevice(Graphics::Global::GetDevice()) {}

Graphics::RenderManager::~RenderManager()
{
	Finalize();
}

bool Graphics::RenderManager::Initialize()
{
  if (mDevice->Initialize())
  {
    // Get the pipeline instance
    mPipeline = mDevice->GetPipeline();
    return true;
  }
  return false;
}

void Graphics::RenderManager::Finalize()
{
  
  // Clear default pipeline state
  //mDefaultRenderState.Clear(); // not needed

  // Clean up constant arrays
  for (U32 stage = 0; stage < eConstantBufferIDCount; ++stage) mConstantBuffers[stage] = nullptr;
  // Clean up resource arrays
  for (U32 stage = 0; stage < eResourceBufferIDCount; ++stage) mResourceBuffers[stage] = nullptr;
  // Clean up vertex layouts
  for (U32 stage = 0; stage < eVertexLayoutIDCount; ++stage) mVertexLayouts[stage] = nullptr;
  // Clean up viewport contexts
  for (U32 stage = 0; stage < eViewportIDCount; ++stage) mViewportContexts[stage] = nullptr;
  // Clean up viewports
  for (U32 stage = 0; stage < eViewportIDCount; ++stage) mViewports[stage] = nullptr;

  // Clear containers
  mBlendStateMap.Clear();
  mDepthStencilStateMap.Clear();
  mRasterStateMap.Clear();
  mShaderMap.Clear();

  // Clean up factories
  mConstantBufferFactory.CleanUp();
  mResourceBufferFactory.CleanUp();

  // Clean up device
  mDevice->Finalize();
}

bool Graphics::RenderManager::IsReady() const
{
  return mDevice != nullptr && mDevice->IsReady();
}

/*----------------------------------------------------------------------------------------------------------------------
RenderManager accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IBlendStateInstance& Graphics::RenderManager::GetBlendState(const IBlendState::Descriptor& desc)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  // Search map for state
  auto pPair = mBlendStateMap.FindPair(desc);
  if (pPair) return (*pPair).second;
  // Create state for given desc and update the state map
  pPair = mBlendStateMap.Insert(desc, mDevice->CreateBlendState(desc));
  E_ASSERT_PTR(pPair);
  return (*pPair).second;
}

const Graphics::IConstantBufferInstance& Graphics::RenderManager::GetConstantBuffer(U32 constantBufferID)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mConstantBuffers[constantBufferID] == nullptr)
  {
    E_DEBUG_MSG("[RenderManager] ConstantBufferInstance: [%d]", mConstantBufferFactory.GetLiveCount() + 1);
    // Create constant buffer
    ConstantBufferInstance constantBuffer = mConstantBufferFactory.Create();
    E_ASSERT_PTR(constantBuffer);
    constantBuffer->Initialize(mDevice);
    // Store constant buffer
    mConstantBuffers[constantBufferID] = constantBuffer;
  }
  return mConstantBuffers[constantBufferID];
}

const Graphics::IDepthStencilStateInstance& Graphics::RenderManager::GetDepthStencilState(const IDepthStencilState::Descriptor& desc)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  // Search map for state
  auto pPair = mDepthStencilStateMap.FindPair(desc);
  if (pPair) return (*pPair).second;
  // Create state for given desc and update the state map
  pPair = mDepthStencilStateMap.Insert(desc, mDevice->CreateDepthStencilState(desc));
  E_ASSERT_PTR(pPair);
  return (*pPair).second;
}

const Graphics::IDeviceInstance& Graphics::RenderManager::GetDevice() const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  return mDevice;
}

const Graphics::IRasterStateInstance& Graphics::RenderManager::GetRasterState(const IRasterState::Descriptor& desc)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  // Search map for state
  auto pPair = mRasterStateMap.FindPair(desc);
  if (pPair) return (*pPair).second;
  // Create state for given desc and update the state map
  pPair = mRasterStateMap.Insert(desc, mDevice->CreateRasterState(desc));
  E_ASSERT_PTR(pPair);
  return (*pPair).second;
}

const Graphics::IResourceBufferInstance& Graphics::RenderManager::GetResourceBuffer(U32 resourceBufferID) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mResourceBuffers[resourceBufferID] != nullptr, E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_CUSTOM_RESOURCE_BUFFER, resourceBufferID);
  return mResourceBuffers[resourceBufferID];
}

const Graphics::ISamplerInstance& Graphics::RenderManager::GetSampler(const ISampler::Descriptor& desc)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  // Search map for state
  auto pPair = mSamplerMap.FindPair(desc);
  if (pPair) return (*pPair).second;
  // Create state for given desc and update the state map
  pPair = mSamplerMap.Insert(desc, mDevice->CreateSampler(desc));
  E_ASSERT_PTR(pPair);
  return (*pPair).second;
}

const Graphics::IRenderManager::Settings& Graphics::RenderManager::GetSettings() const
{
  return mSettings;
}

Graphics::IRenderManager::Settings& Graphics::RenderManager::GetSettings()
{
  return mSettings;
}

const Graphics::IShaderInstance& Graphics::RenderManager::GetShader(const String& shaderName) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  // Search map for state
  auto pPair = mShaderMap.FindPair(shaderName);
  E_ASSERT_MSG(pPair != nullptr, E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_CUSTOM_SHADER_TECHNIQUE, shaderName.GetPtr());
  return (pPair) ? (*pPair).second : kEmptyShader;
}
 
const Graphics::ITexture2DInstance& Graphics::RenderManager::GetTexture2D(const FilePath& fileName)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_PTR(mDevice);
  // Search map for state
  auto pPair = mTextureMap.FindPair(fileName);
  if (pPair) return (*pPair).second;
  // Create shader technique
  const ITexture2DInstance& texture2D =  mSettings.dataRootDirectory.GetLength() ? 
    mDevice->CreateTexture2D(mSettings.dataRootDirectory + mSettings.textureFolder + fileName) :
    mDevice->CreateTexture2D(fileName);
  E_ASSERT_PTR(texture2D);
  // Update map
  pPair = mTextureMap.Insert(fileName, texture2D);
  E_ASSERT_PTR(pPair);
  return (*pPair).second;
}

const Graphics::IVertexLayoutInstance& Graphics::RenderManager::GetVertexLayout(U32 vertexLayoutID) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mVertexLayouts[vertexLayoutID] != nullptr, E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_CUSTOM_VERTEX_LAYOUT, vertexLayoutID); 
  return mVertexLayouts[vertexLayoutID];
}

const Graphics::IViewportInstance& Graphics::RenderManager::GetViewport(U32 viewportID) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mViewports[viewportID] != nullptr, E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_VIEWPORT, viewportID);
  return mViewports[viewportID];
}

const Graphics::IRenderTargetInstance& Graphics::RenderManager::GetFrameBuffer(U32 viewportID) const
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mViewports[viewportID] != nullptr, E_ASSERT_MSG_RENDER_MANAGER_UNDEFINED_VIEWPORT, viewportID);
  return mViewportContexts[viewportID];
}

void Graphics::RenderManager::SetResourceBuffer(U32 resourceBufferID, size_t elementSize)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mResourceBuffers[resourceBufferID] == nullptr, E_ASSERT_MSG_RENDER_MANAGER_DEFINED_CUSTOM_RESOURCE_BUFFER, resourceBufferID);
  E_DEBUG_MSG("[RenderManager] ResourceBufferInstance: [%d]", mResourceBufferFactory.GetLiveCount() + 1);
  // Create resource buffer
  ResourceBufferInstance resourceBuffer = mResourceBufferFactory.Create();
  E_ASSERT_PTR(resourceBuffer);
  resourceBuffer->Initialize(elementSize, mDevice);
  // Store resource buffer
  mResourceBuffers[resourceBufferID] = resourceBuffer;
}

void Graphics::RenderManager::SetShader(const String& shaderName, const IShader::Descriptor& desc)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_PTR(mDevice);
  // Check for existing shader
  E_ASSERT_MSG(mShaderMap.FindPair(shaderName) == nullptr, E_ASSERT_MSG_RENDER_MANAGER_DEFINED_CUSTOM_SHADER_TECHNIQUE, shaderName.GetPtr());
  // Create shader
  IShaderInstance shader;
  if (mSettings.dataRootDirectory.GetLength())
  {
    IShader::Descriptor systemPathDesc = desc;
    FilePath shaderFolder = mSettings.dataRootDirectory + mSettings.shaderFolder;
    for (U32 stage = 0; stage < IShader::eStageCount; ++stage)
    {
      if (systemPathDesc.stages[stage].filePath.GetLength()) systemPathDesc.stages[stage].filePath = shaderFolder + systemPathDesc.stages[stage].filePath;
    }
    shader = mDevice->CreateShader(systemPathDesc);
  }
  else
  {
    shader = mDevice->CreateShader(desc);
  }

  if (shader) mShaderMap.Insert(shaderName, shader);
}

void Graphics::RenderManager::SetVertexLayout(U32 vertexLayoutID, const IVertexLayout::Descriptor& desc)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mVertexLayouts[vertexLayoutID] == nullptr, E_ASSERT_MSG_RENDER_MANAGER_DEFINED_CUSTOM_VERTEX_LAYOUT, vertexLayoutID);
  mVertexLayouts[vertexLayoutID] = mDevice->CreateVertexLayout(desc);
  E_ASSERT_PTR(mVertexLayouts[vertexLayoutID]);
}

void Graphics::RenderManager::SetViewport(U32 viewportID, Ptr windowHandle, U32 viewportWidth, U32 viewportHeight, bool fullScreen)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  E_ASSERT_MSG(mViewports[viewportID] == nullptr, E_ASSERT_MSG_RENDER_MANAGER_DEFINED_VIEW_WINDOW, viewportID);

  // Create viewport
  IViewport::Descriptor viewportDescriptor;
  viewportDescriptor.windowHandle = windowHandle;
  viewportDescriptor.width = viewportWidth;
  viewportDescriptor.height = viewportHeight;
  if (fullScreen) viewportDescriptor.flags |= IViewport::eViewportFlagEnableFullScreen;
  if (mSettings.verticalSync) viewportDescriptor.flags |= IViewport::eViewportFlagEnableVerticalSync;
  mViewports[viewportID] = mDevice->CreateViewport(viewportDescriptor);
  E_ASSERT_PTR(mViewports[viewportID]);

  // Create viewport color target
  ITexture2DInstance viewportColorTarget = mDevice->CreateTexture2D(mViewports[viewportID]);
  E_ASSERT_PTR(viewportColorTarget);

  // Create viewport depth target
  ITexture2D::Descriptor depthTargetDescriptor;
  depthTargetDescriptor.type = Graphics::ITexture2D::eTypeDepthTarget;
  depthTargetDescriptor.format = Graphics::ITexture2D::eFormatDepth24S8;
  depthTargetDescriptor.width = viewportWidth;
  depthTargetDescriptor.height = viewportHeight;
  depthTargetDescriptor.unitCount = 1;
  depthTargetDescriptor.accessFlags = Graphics::ITexture2D::eAccessFlagNone;
  ITexture2DInstance viewportDepthTarget = mDevice->CreateTexture2D(depthTargetDescriptor);
  E_ASSERT_PTR(viewportDepthTarget);

  // Create viewport context
  IRenderTarget::Descriptor viewportContextDescriptor;
  viewportContextDescriptor.colorTargets.Reserve(1);
  viewportContextDescriptor.colorTargets[0] = viewportColorTarget;
  viewportContextDescriptor.depthTarget = viewportDepthTarget;
  mViewportContexts[viewportID] = mDevice->CreateContext(viewportContextDescriptor);
  E_ASSERT_PTR(mViewportContexts[viewportID]);
}

/*----------------------------------------------------------------------------------------------------------------------
RenderManager methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::RenderManager::Bind(const IBlendStateInstance& blendState)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.blendState != blendState)
  {
    mPipeline->BindState(blendState);
    mPipelineState.blendState = blendState;
  }
}

void Graphics::RenderManager::Bind(const IConstantBufferInstance& constantBuffer, IShader::Stage stage, U32 slot)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  const IBufferInstance& buffer = constantBuffer->GetBuffer();
  if (mPipelineState.shaderStages[stage].constantBuffers[slot] != buffer)
  {
    buffer->Update();
    mPipeline->BindShaderConstant(buffer, static_cast<IShader::Stage>(stage), slot);
    mPipelineState.shaderStages[stage].constantBuffers[slot] = buffer;
  }
}

void Graphics::RenderManager::Bind(const IDepthStencilStateInstance& depthStencilState)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.depthStencilState != depthStencilState)
  {
    mPipeline->BindState(depthStencilState);
    mPipelineState.depthStencilState = depthStencilState;
  }
}

void Graphics::RenderManager::Bind(const IRasterStateInstance& rasterState)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.rasterState != rasterState)
  {
    mPipeline->BindState(rasterState);
    mPipelineState.rasterState = rasterState;
  }
}

void Graphics::RenderManager::Bind(const IRenderTargetInstance& renderTarget)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.renderTarget != renderTarget)
  {
    mPipeline->BindOutput(renderTarget);
    mPipelineState.renderTarget = renderTarget;
  }
}

void Graphics::RenderManager::Bind(const IResourceBufferInstance& resourceBuffer, IShader::Stage stage, U32 slot)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  const IBufferInstance& buffer = resourceBuffer->GetBuffer();
  if (mPipelineState.shaderStages[stage].resources[slot] != buffer)
  {
    buffer->Update();
    mPipeline->BindShaderInput(buffer, static_cast<IShader::Stage>(stage), slot);
    mPipelineState.shaderStages[stage].resources[slot] = resourceBuffer->GetBuffer();
  }
}

void Graphics::RenderManager::Bind(const ISamplerInstance& sampler, IShader::Stage stage, U32 slot)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.shaderStages[stage].samplers[slot] != sampler)
  {
    mPipeline->BindShaderSampler(sampler, stage, slot);
    mPipelineState.shaderStages[stage].samplers[slot] = sampler;
  }
}

void Graphics::RenderManager::Bind(const IShaderInstance& shader)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.shader != shader)
  {
    mPipeline->BindShader(shader);
    mPipelineState.shader = shader;
  }
}

void Graphics::RenderManager::Bind(const ITexture2DInstance& texture2D, IShader::Stage stage, U32 slot)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.shaderStages[stage].resources[slot] != texture2D)
  {
    mPipeline->BindShaderInput(texture2D, stage, slot);
    mPipelineState.shaderStages[stage].resources[slot] = texture2D;
  }
}

void Graphics::RenderManager::Bind(const VertexArray& vertexArray)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (mPipelineState.vertexLayout != vertexArray.vertexLayout)
  {
    mPipeline->BindInput(vertexArray.vertexLayout);
    mPipelineState.vertexLayout = vertexArray.vertexLayout;
  }
  if (mPipelineState.vertexBuffer != vertexArray.vertexBuffer)
  {
    vertexArray.vertexBuffer->Update();
    mPipeline->BindInput(vertexArray.vertexBuffer, 0);
    mPipelineState.vertexBuffer = vertexArray.vertexBuffer;
  }
  if (mPipelineState.indexBuffer != vertexArray.indexBuffer)
  {
    vertexArray.indexBuffer->Update();
    mPipeline->BindInput(vertexArray.indexBuffer);
    mPipelineState.indexBuffer = vertexArray.indexBuffer;
  }
}

void Graphics::RenderManager::Draw(const DrawState& drawState)
{
  E_ASSERT_MSG(IsReady(), E_ASSERT_MSG_RENDER_MANAGER_READY);
  if (drawState.vertexCount)
  {
    mPipelineState.renderTarget->Draw(
      drawState.vertexPrimitive,
      drawState.vertexCount,
      drawState.indexCount,
      drawState.instanceCount,
      drawState.startVertex,
      drawState.startIndex,
      drawState.startInstance);
  }
}

void Graphics::RenderManager::Unbind(const IResourceBufferInstance& resourceBuffer)
{
  UnbindResource(resourceBuffer->GetBuffer());
}

void Graphics::RenderManager::Unbind(const ITexture2DInstance& texture2D)
{
  UnbindResource(texture2D);
}

void Graphics::RenderManager::Update(const IConstantBufferInstance& constantBuffer)
{
  const IBufferInstance& buffer = constantBuffer->GetBuffer();
  if (buffer->Update())
  {
    bool bufferFound = false;
    for (U32 slot = 0; slot < ShaderStageState::eResourceCount; ++slot)
    {
      for (U32 stage = 0; stage< IShader::eStageCount; ++stage)
      {
        if (mPipelineState.shaderStages[stage].constantBuffers[slot] == buffer)
        {
          mPipeline->BindShaderConstant(buffer, static_cast<IShader::Stage>(stage), slot);
          bufferFound = true;
        }
      }
      if (bufferFound) return;
    }
  }
}

void Graphics::RenderManager::Update(const IResourceBufferInstance& resourceBuffer)
{
  const IBufferInstance& buffer = resourceBuffer->GetBuffer();
  if (buffer->Update())
  {
    bool bufferFound = false;
    for (U32 slot = 0; slot < ShaderStageState::eResourceCount; ++slot)
    {
      for (U32 stage = 0; stage< IShader::eStageCount; ++stage)
      {
        if (mPipelineState.shaderStages[stage].resources[slot] == buffer)
        {
          mPipeline->BindShaderInput(buffer, static_cast<IShader::Stage>(stage), slot);
          bufferFound = true;
        }
      }
      if (bufferFound) return;
    }
  }
}

void Graphics::RenderManager::Update(const VertexArray& vertexArray)
{
  if (vertexArray.vertexBuffer->Update() && mPipelineState.vertexBuffer == vertexArray.vertexBuffer)
  {
    mPipeline->BindInput(vertexArray.vertexBuffer, 0);
  }
    
  if (vertexArray.indexBuffer->Update() && mPipelineState.indexBuffer == vertexArray.indexBuffer)
  {
    mPipeline->BindInput(vertexArray.indexBuffer);
  }
}

/*----------------------------------------------------------------------------------------------------------------------
RenderManager methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::RenderManager::UnbindResource(const IResourceInstance& resource)
{
  for (U32 slot = 0; slot < ShaderStageState::eResourceCount; ++slot)
  {
    for (U32 stage = 0; stage< IShader::eStageCount; ++stage)
    {
      if (mPipelineState.shaderStages[stage].resources[slot] == resource)
      {
        while (stage < IShader::eStageCount)
        {
          mPipeline->UnbindShaderInput(static_cast<IShader::Stage>(stage), slot);
          mPipelineState.shaderStages[stage++].resources[slot] = nullptr;
        }
      }
    }
  }
}

void Graphics::RenderManager::UpdateBuffer(const IBufferInstance& buffer)
{
  if (buffer->Update())
  {
    bool bufferFound = false;
    for (U32 slot = 0; slot < ShaderStageState::eResourceCount; ++slot)
    {
      for (U32 stage = 0; stage< IShader::eStageCount; ++stage)
      {
        if (mPipelineState.shaderStages[stage].resources[slot] == buffer)
        {
          mPipeline->BindShaderInput(buffer, static_cast<IShader::Stage>(stage), slot);
          bufferFound = true;
        }
      }
      if (bufferFound) return;
    }
  }
}
