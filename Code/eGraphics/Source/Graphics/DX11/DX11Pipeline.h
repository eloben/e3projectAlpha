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

// Created 03-Jul-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file DX11Pipeline.h
This file declares the DX11Pipeline class.
*/

#ifndef E3_DX11_PIPELINE_H
#define E3_DX11_PIPELINE_H

#include "DX11Base.h"

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Pipeline

Please note that this set of macros has the following usage contract:

1. Clear unbinds all shader inputs and outputs.
----------------------------------------------------------------------------------------------------------------------*/
class DX11Pipeline : public IPipeline
{
public:
  DX11Pipeline();
  ~DX11Pipeline();

  void                  BindInput(const IBufferInstance& indexBuffer);
  void                  BindInput(const IBufferInstance& vertexBuffer, U32 slot);
  void                  BindInput(const IVertexLayoutInstance& vertexLayout);
  void                  BindOutput(const IRenderTargetInstance& renderTarget);
  void                  BindShader(const IShaderInstance& shader);
  void	                BindShaderConstant(const IBufferInstance& constantBuffer, IShader::Stage stage, U32 slot);
  void	                BindShaderInput(const IBufferInstance& resourceBuffer, IShader::Stage stage, U32 slot);
  void	                BindShaderInput(const IResourceInstance& resource, IShader::Stage stage, U32 slot);
  void	                BindShaderInput(const ITexture2DInstance& texture2D, IShader::Stage stage, U32 slot);
  void	                BindShaderSampler(const ISamplerInstance& sampler, IShader::Stage stage, U32 slot);
  void                  BindShaderOutput(const IBufferInstance& resourceBuffer, U32 slot);
  void                  BindShaderOutput(const ITexture2DInstance& texture2D, U32 slot);
  void                  BindState(const IBlendStateInstance& blendState);
  void                  BindState(const IDepthStencilStateInstance& depthStencilState);
  void                  BindState(const IRasterStateInstance& rasterState);
  void                  Clear();
  void	                UnbindShaderInput(IShader::Stage stage, U32 slot);
  void                  UnbindShaderOutput(U32 slot);

private:
  enum
  {
    eMaxShaderInputResourceCount = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT,  // 128
    eMaxShaderOutputResourceCount = D3D11_PS_CS_UAV_REGISTER_COUNT,               // 8
    eMaxShaderConstantCount = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT,  // 14
    eMaxShaderSamplerCount = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,               // 16
    eMaxVertexInputCount = D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT              // 32
  };

  ID3D11DeviceContext*  mpDXDeviceContext;

  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11Pipeline)
}; 
}
}

#endif
