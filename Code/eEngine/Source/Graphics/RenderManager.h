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

/** @file RenderManager.h
This file declares the RenderManager class.
*/

#ifndef E3_RENDER_MANAGER_H
#define E3_RENDER_MANAGER_H

#include "ConstantBuffer.h"
#include "ResourceBuffer.h"

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
RenderManager

Please note that this interface has the following usage contract: 

1. The execution of render commands is performed in a separate rendering thread.
----------------------------------------------------------------------------------------------------------------------*/
class RenderManager : public IRenderManager
{
public:
  RenderManager();
  ~RenderManager();

  bool	                          Initialize();
  void	                          Finalize();
  bool                            IsReady() const;
  
  // Accessors
  const IBlendStateInstance&        GetBlendState(const IBlendState::Descriptor& desc);
  const IConstantBufferInstance&    GetConstantBuffer(U32 constantBufferID);
  const IDepthStencilStateInstance& GetDepthStencilState(const IDepthStencilState::Descriptor& desc);
  const IDeviceInstance&     GetDevice() const;
  const IRenderTargetInstance&      GetFrameBuffer(U32 viewportID) const;
  const IRasterStateInstance&       GetRasterState(const IRasterState::Descriptor& desc);
  const IResourceBufferInstance&    GetResourceBuffer(U32 resourceBufferID) const;
  const ISamplerInstance&           GetSampler(const ISampler::Descriptor& desc);
  const Settings&                   GetSettings() const;
  Settings&                         GetSettings();
  const IShaderInstance&            GetShader(const String& shaderName) const;
  const ITexture2DInstance&         GetTexture2D(const FilePath& filePath);
  const IVertexLayoutInstance&      GetVertexLayout(U32 vertexLayoutID) const;
  const IViewportInstance&          GetViewport(U32 viewportID) const;
  void                              SetResourceBuffer(U32 resourceBufferID, size_t elementSize);
  void                              SetShader(const String& shaderName, const IShader::Descriptor& desc);
  void                              SetVertexLayout(U32 vertexLayoutID, const IVertexLayout::Descriptor& desc);
  void                              SetViewport(U32 viewportID, Ptr windowHandle, U32 viewportWidth, U32 viewportHeight, bool fullScreen);

  // Methods
  void                              Bind(const IBlendStateInstance& blendState);
  void                              Bind(const IConstantBufferInstance& constantBuffer, IShader::Stage stage, U32 slot);
  void                              Bind(const IDepthStencilStateInstance& depthStencilState);
  void                              Bind(const IRasterStateInstance& rasterState);
  void                              Bind(const IRenderTargetInstance& renderTarget);
  void                              Bind(const IResourceBufferInstance& resourceBuffer, IShader::Stage stage, U32 slot);
  void                              Bind(const ISamplerInstance& sampler, IShader::Stage stage, U32 slot);
  void                              Bind(const IShaderInstance& shader);
  void                              Bind(const ITexture2DInstance& texture2D, IShader::Stage stage, U32 slot);
  void	                            Bind(const VertexArray& vertexArray);
  void                              Draw(const DrawState& drawState);
  void                              Unbind(const IResourceBufferInstance& resourceBuffer);
  void                              Unbind(const ITexture2DInstance& resourceBuffer);
  void                              Update(const IConstantBufferInstance& constantBuffer);
  void                              Update(const IResourceBufferInstance& resourceBuffer);
  void                              Update(const VertexArray& vertexArray);

private:
  typedef Memory::GCConcreteFactory<ConstantBuffer>	                      ConstantBufferFactory;
  typedef Memory::GCConcreteFactory<ResourceBuffer>	                      ResourceBufferFactory;
  typedef Containers::Map<IBlendState::Descriptor, IBlendStateInstance>               IBlendStateMap;
  typedef Containers::Map<IDepthStencilState::Descriptor, IDepthStencilStateInstance> IDepthStencilStateMap;
  typedef Containers::Map<IRasterState::Descriptor, IRasterStateInstance>             IRasterStateMap;
  typedef Containers::Map<ISampler::Descriptor, ISamplerInstance>                     ISamplerMap;
  typedef Containers::Map<String, IShaderInstance>                                    IShaderMap;
  typedef Containers::Map<String, ITexture2DInstance>                                 ITexture2DMap;

  ConstantBufferFactory           mConstantBufferFactory;
  ResourceBufferFactory           mResourceBufferFactory;
  IBlendStateMap                  mBlendStateMap;
  IDepthStencilStateMap           mDepthStencilStateMap;
  IRasterStateMap                 mRasterStateMap;
  ISamplerMap                     mSamplerMap;
  IShaderMap                      mShaderMap;
  ITexture2DMap                   mTextureMap;
  IConstantBufferInstance         mConstantBuffers[eConstantBufferIDCount];
  IResourceBufferInstance         mResourceBuffers[eResourceBufferIDCount];
  IViewportInstance               mViewports[eViewportIDCount];
  IRenderTargetInstance           mViewportContexts[eViewportIDCount];
  IVertexLayoutInstance           mVertexLayouts[eVertexLayoutIDCount];
  IDeviceInstance          mDevice;
  IPipelineInstance               mPipeline;
  PipelineState                   mPipelineState;
  Settings                        mSettings;

  void                            UnbindResource(const IResourceInstance& resource);
  void                            UpdateBuffer(const IBufferInstance& buffer);

  E_DISABLE_COPY_AND_ASSSIGNMENT(RenderManager)
}; 
}
}

#endif
