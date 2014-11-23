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

// Created 02-Jul-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IShaderPipeline.h
This file defines the IShaderPipeline interface. IShaderPipeline abstract the Gpu programmable pipeline.
*/

#ifndef E3_IPIPELINE_H
#define E3_IPIPELINE_H

#include <Graphics/IBlendState.h>
#include <Graphics/IBuffer.h>
#include <Graphics/IRenderTarget.h>
#include <Graphics/IDepthStencilState.h>
#include <Graphics/IRasterState.h>
#include <Graphics/ISampler.h>
#include <Graphics/IShader.h>
#include <Graphics/ITexture2D.h>
#include <Graphics/IVertexLayout.h>

namespace E 
{
namespace Graphics
{
// Forward declarations
class IBuffer;
class IVertexLayout;
class IRenderTarget;
class ISampler;
class ITexture2D;
class IBlendState;
class IDepthStencilState;
class IRasterState;

/*----------------------------------------------------------------------------------------------------------------------
IPipeline

Please note that this interface class has the following contract:

1. IPipeline requires IDevice to be initialized.
2. All bind methods expect a valid input.
----------------------------------------------------------------------------------------------------------------------*/
class IPipeline
{
public:
  virtual       ~IPipeline() {}

  virtual void  BindInput(const IBufferInstance& indexBuffer) = 0;
  virtual void  BindInput(const IBufferInstance& vertexBuffer, U32 slot) = 0;
  virtual void  BindInput(const IVertexLayoutInstance& vertexLayout) = 0;
  virtual void  BindOutput(const IRenderTargetInstance& renderTarget) = 0;
  virtual void  BindShader(const IShaderInstance& shader) = 0;
  virtual void	BindShaderConstant(const IBufferInstance& constantBuffer, IShader::Stage stage, U32 slot) = 0;
  virtual void	BindShaderInput(const IBufferInstance& resourceBuffer, IShader::Stage stage, U32 slot) = 0;
  virtual void	BindShaderInput(const IResourceInstance& resource, IShader::Stage stage, U32 slot) = 0;
  virtual void	BindShaderInput(const ITexture2DInstance& texture2D, IShader::Stage stage, U32 slot) = 0;
  virtual void	BindShaderSampler(const ISamplerInstance& sampler, IShader::Stage stage, U32 slot) = 0;
  virtual void  BindShaderOutput(const IBufferInstance& resourceBuffer, U32 slot) = 0;
  virtual void  BindShaderOutput(const ITexture2DInstance& texture2D, U32 slot) = 0;
  virtual void  BindState(const IBlendStateInstance& blendState) = 0;
  virtual void  BindState(const IDepthStencilStateInstance& depthStencilState) = 0;
  virtual void  BindState(const IRasterStateInstance& rasterState) = 0;
  virtual void  Clear() = 0;
  virtual void	UnbindShaderInput(IShader::Stage stage, U32 slot) = 0;
  virtual void  UnbindShaderOutput(U32 slot) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IPipeline types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IPipeline> IPipelineInstance;
} 
}

#endif
