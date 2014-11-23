/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 04-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IRenderManager.h
This file defines the IRenderManager interface. IRenderManager is the graphics engine interface.
*/

#ifndef E3_IRENDER_MANAGER_H
#define E3_IRENDER_MANAGER_H

#include <Graphics/IConstantBuffer.h>
#include <Graphics/IDevice.h>
#include <Graphics/IResourceBuffer.h>
#include <Graphics/IShader.h>
#include <Graphics/IShader.h>
#include <Graphics/Vertex.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
VertexState
----------------------------------------------------------------------------------------------------------------------*/
struct VertexArray
{
  IVertexLayoutInstance vertexLayout;
  IBufferInstance       vertexBuffer;
  IBufferInstance       indexBuffer;
  //IBufferInstance       instanceBuffer;
};

/*----------------------------------------------------------------------------------------------------------------------
DrawState
----------------------------------------------------------------------------------------------------------------------*/
struct DrawState
{
  U32             vertexCount;
  U32             indexCount;
  U32             instanceCount;
  U32             startVertex;
  U32             startIndex;
  U32             startInstance;
  U32             renderMask;
  VertexPrimitive vertexPrimitive;

  DrawState()
    : vertexCount(0)
    , indexCount(0)
    , instanceCount(0)
    , startVertex(0)
    , startIndex(0)
    , startInstance(0)
    , renderMask(0)
    , vertexPrimitive(eVertexPrimitiveUndefined) {}

  void Clear()
  {
    vertexCount = 0;
    indexCount = 0;
    instanceCount = 0;
    startVertex = 0;
    startIndex = 0;
    startInstance = 0;
    vertexPrimitive = eVertexPrimitiveUndefined;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
ShaderStageState
----------------------------------------------------------------------------------------------------------------------*/
struct ShaderStageState
{
  enum 
  {
    eConstantBufferCount = 10,
    eResourceCount = 10,
    eSamplerCount = 10
  };

  IBufferInstance   constantBuffers[eConstantBufferCount];
  IResourceInstance resources[eResourceCount];
  ISamplerInstance  samplers[eSamplerCount];

  void Clear()
  {
    for (U32 i = 0; i < eConstantBufferCount; ++i) constantBuffers[i] = nullptr;
    for (U32 i = 0; i < eResourceCount; ++i) resources[i] = nullptr;
    for (U32 i = 0; i < eSamplerCount; ++i) samplers[i] = nullptr;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
PipelineState
----------------------------------------------------------------------------------------------------------------------*/
struct PipelineState
{
  ShaderStageState            shaderStages[IShader::eStageCount];
  IShaderInstance             shader;
  IVertexLayoutInstance       vertexLayout;
  IBufferInstance             vertexBuffer;
  IBufferInstance             indexBuffer;
  IBufferInstance             instanceBuffer;
  IBlendStateInstance         blendState;
  IDepthStencilStateInstance  depthStencilState;
  IRasterStateInstance        rasterState;
  IRenderTargetInstance       renderTarget;
  
  void Clear()
  {
    for (U32 i = 0; i < IShader::eStageCount; ++i) shaderStages[i].Clear();
    shader = nullptr;
    vertexLayout = nullptr;
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    instanceBuffer = nullptr;
    blendState = nullptr;
    depthStencilState = nullptr;
    rasterState = nullptr;
    renderTarget = nullptr;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
IRenderManager

Please note that this interface has the following usage contract: 

1. IRenderManager methods require IRenderManager to be initialized.
2. IRenderManager tracks the GPU pipeline state.
3. All bind methods perform a GPU bind only if the parameter object is not already bound (not tracked in the
pipeline state).
4. Bind methods with buffer parameters update the buffers before binding only if the buffer is not already bound.
To ensure proper update of bound resources (especially for resources bound to multiple stages) use Update instead.
5. Update assumes that a concrete shader resource is bound to the same slot in all shader stages.
6. Update only performs a GPU bind if the updated resource is previously bound by a Bind method call (tracked in
the pipeline state).
----------------------------------------------------------------------------------------------------------------------*/
class IRenderManager
{
public:
  /*----------------------------------------------------------------------------------------------------------------------
  Settings
  ----------------------------------------------------------------------------------------------------------------------*/
  struct Settings
  {
    FilePath  dataRootDirectory;
    FilePath  shaderFolder;
    FilePath  textureFolder;
    bool      verticalSync;
  };

  /*----------------------------------------------------------------------------------------------------------------------
  ViewportID
  ----------------------------------------------------------------------------------------------------------------------*/
  enum ViewportID
  {
    eViewportID0,
    eViewportID1,
    eViewportID2,
    eViewportID3,
    eViewportIDCount
  };

  /*----------------------------------------------------------------------------------------------------------------------
  VertexType
  ----------------------------------------------------------------------------------------------------------------------*/
  enum VertexLayoutID
  {
    eVertexLayoutID0,
    eVertexLayoutID1,
    eVertexLayoutID2,
    eVertexLayoutID3,
    eVertexLayoutID4,
    eVertexLayoutID5,
    eVertexLayoutID6,
    eVertexLayoutID7,
    eVertexLayoutID8,
    eVertexLayoutID9,
    eVertexLayoutIDCount
  };

  /*----------------------------------------------------------------------------------------------------------------------
  ConstantBufferID
  ----------------------------------------------------------------------------------------------------------------------*/
  enum ConstantBufferID
  {
    eConstantBufferID0,
    eConstantBufferID1,
    eConstantBufferID2,
    eConstantBufferID3,
    eConstantBufferID4,
    eConstantBufferIDCount
  };

  /*----------------------------------------------------------------------------------------------------------------------
  ResourceBufferID
  ----------------------------------------------------------------------------------------------------------------------*/
  enum ResourceBufferID
  {
    eResourceBufferID0,
    eResourceBufferID1,
    eResourceBufferID2,
    eResourceBufferID3,
    eResourceBufferID4,
    eResourceBufferID5,
    eResourceBufferID6,
    eResourceBufferID7,
    eResourceBufferID8,
    eResourceBufferID9,
    eResourceBufferIDCount
  };

  virtual                                 ~IRenderManager() {}
  virtual bool	                          Initialize() = 0;
  virtual void	                          Finalize() = 0;
  virtual bool                            IsReady() const = 0;

  // Accessors
  virtual const IBlendStateInstance&        GetBlendState(const IBlendState::Descriptor& desc) = 0;
  virtual const IConstantBufferInstance&    GetConstantBuffer(U32 constantBufferID) = 0;
  virtual const IDepthStencilStateInstance& GetDepthStencilState(const IDepthStencilState::Descriptor& desc) = 0;
  virtual const IDeviceInstance&     GetDevice() const = 0;
  virtual const IRenderTargetInstance&      GetFrameBuffer(U32 viewportID) const = 0;
  virtual const IRasterStateInstance&       GetRasterState(const IRasterState::Descriptor& desc) = 0;
  virtual const IResourceBufferInstance&    GetResourceBuffer(U32 resourceBufferID) const = 0;
  virtual const ISamplerInstance&           GetSampler(const ISampler::Descriptor& desc) = 0;
  virtual const Settings&                   GetSettings() const = 0;
  virtual Settings&                         GetSettings() = 0;
  virtual const IShaderInstance&            GetShader(const String& shaderName) const = 0;
  virtual const ITexture2DInstance&         GetTexture2D(const FilePath& filePath) = 0;
  virtual const IVertexLayoutInstance&      GetVertexLayout(U32 vertexLayoutID) const = 0;
  virtual const IViewportInstance&          GetViewport(U32 viewportID) const = 0;
  virtual void                              SetResourceBuffer(U32 resourceBufferID, size_t elementSize) = 0;
  virtual void                              SetShader(const String& shaderName, const IShader::Descriptor& desc) = 0;
  virtual void                              SetVertexLayout(U32 vertexLayoutID, const IVertexLayout::Descriptor& desc) = 0;
  virtual void                              SetViewport(U32 viewportID, Ptr windowHandle, U32 viewportWidth, U32 viewportHeight, bool fullScreen) = 0;

  // Methods
  virtual void                              Bind(const IBlendStateInstance& blendState) = 0;
  virtual void                              Bind(const IConstantBufferInstance& constantBuffer, IShader::Stage stage, U32 slot) = 0;
  virtual void                              Bind(const IDepthStencilStateInstance& depthStencilState) = 0;
  virtual void                              Bind(const IRasterStateInstance& rasterState) = 0;
  virtual void                              Bind(const IRenderTargetInstance& renderTarget) = 0;
  virtual void                              Bind(const IResourceBufferInstance& resourceBuffer, IShader::Stage stage, U32 slot) = 0;
  virtual void                              Bind(const ISamplerInstance& sampler, IShader::Stage stage, U32 slot) = 0;
  virtual void                              Bind(const IShaderInstance& shader) = 0;
  virtual void                              Bind(const ITexture2DInstance& texture2D, IShader::Stage stage, U32 slot) = 0;
  virtual void	                            Bind(const VertexArray& vertexArray) = 0;
  virtual void                              Draw(const DrawState& drawState) = 0;
  virtual void                              Unbind(const IResourceBufferInstance& resourceBuffer) = 0;
  virtual void                              Unbind(const ITexture2DInstance& resourceBuffer) = 0;
  virtual void                              Update(const IConstantBufferInstance& constantBuffer) = 0;
  virtual void                              Update(const IResourceBufferInstance& resourceBuffer) = 0;
  virtual void                              Update(const VertexArray& vertexArray) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IRenderManager types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IRenderManager> IRenderManagerInstance;
}
}

#endif

