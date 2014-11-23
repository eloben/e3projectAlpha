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

// Created 23-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ForwardRenderer.h
This file declares the ForwardRenderer class.
*/

#ifndef E3_FORWARD_RENDERER_H
#define E3_FORWARD_RENDERER_H

#include <Graphics/Scene/IRenderer.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ForwardRenderer
----------------------------------------------------------------------------------------------------------------------*/
class ForwardRenderer : public IRenderer
{
public:
  enum ShaderID
  {
    eShaderIDDefault,
    eShaderIDDefaultDiffuseMap,
    eShaderIDLitAmbient,
    eShaderIDLitAmbientDiffuseMap,
    eShaderIDLitDirect,
    eShaderIDLitDirectDiffuseMap,
    eShaderIDLitDirectShadow,
    eShaderIDLitDirectShadowDiffuseMap,
    eShaderIDLitPoint,
    eShaderIDLitPointDiffuseMap,
    eShaderIDLitSpot,
    eShaderIDLitSpotDiffuseMap,
    eShaderIDLitSpotShadow,
    eShaderIDLitSpotShadowDiffuseMap,
    eShaderIDDepth,
    eShaderIDCount
  };

  enum BlendStateID
  {
    eBlendStateIDDefault,
    eBlendStateIDAdditive,
    eBlendStateIDCount
  };

  enum DepthStencilStateID
  {
    eDepthStencilStateIDDefault,
    eDepthStencilStateIDNoDepthWriting,
    eDepthStencilStateIDCount
  };

  enum RasterStateID
  {
    eRasterStateIDDefault,
    eRasterStateIDWireframe,
    eRasterStateIDDepthBias,
    eRasterStateIDCount
  };

  enum SamplerID
  {
    eSamplerIDNearestClamp,   // Min_Mag_Mip_Point
    eSamplerIDNearestWrap,    // Min_Mag_Mip_Point
    eSamplerIDBilinearClamp,  // Min_Mag_Linear_Mip_Point
    eSamplerIDBilinearWrap,   // Min_Mag_Linear_Mip_Point
    eSamplerIDTrilinearClamp, // Min_Mag_Mip_Linear
    eSamplerIDTrilinearWrap,  // Min_Mag_Mip_Linear
    eSamplerIDTrilinearLessOrEqualClamp,
    eSamplerIDAnisotropicClamp, // Min_Mag_Mip_Linear
    eSamplerIDAnisotropicWrap,  // Min_Mag_Mip_Linear
    eSamplerIDCount
  };

  ForwardRenderer();

  // Accessors
  RendererType                GetRendererType() const;
  void                        SetDepthBias(I32 depthBias);
  void                        SetSlopeScaledDepthBias(F32 depthBias);

  // Methods
  void                        Render(const IViewInstance& view, const IWorldInstance& world);

private:
  IRenderManagerInstance      mRenderManager;
  IViewInstance               mView;
  IWorldInstance              mWorld;
  IShaderInstance             mShaders[eShaderIDCount];
  IRasterStateInstance        mRasterStates[eRasterStateIDCount];
  IBlendStateInstance         mBlendStates[eBlendStateIDCount];
  IDepthStencilStateInstance  mDepthStencilStates[eDepthStencilStateIDCount];
  ISamplerInstance            mSamplers[eSamplerIDCount];
  IConstantBufferInstance     mInterFrameConstantBuffer;
  IConstantBufferInstance     mIntraFrameConstantBuffer;
  IResourceBufferInstance     mTransformBuffer;
  IResourceBufferInstance     mMaterialBuffer;
  ICameraInstance             mShadowCamera;
  IRenderTargetInstance       mShadowTarget;
  ITexture2DInstance          mShadowDepthTexture;

  void                        LoadSamplers();
  void                        LoadShaders();
  void                        LoadStates();
  void                        LoadViewState(const ViewState& renderState);
  
  void                        RenderDefault();
  void                        RenderLightAmbientPass();
  void                        RenderLightPass();
  void                        RenderLightPointPass();
  void                        RenderLightSpotPass();
  void                        RenderShadowPass(const IShadowComponentInstance& shadowComponent);
  void                        RenderLit();
        
  E_DISABLE_COPY_AND_ASSSIGNMENT(ForwardRenderer)
}; 
}
}
}

#endif
