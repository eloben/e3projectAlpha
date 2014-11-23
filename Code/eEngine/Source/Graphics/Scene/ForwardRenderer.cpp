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

/** @file ForwardRenderer.cpp
This file defines the ForwardRenderer class.
*/

#include <EnginePch.h>
#include "ForwardRenderer.h"
#include "ScenePipeline.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
ForwardRenderer auxiliary
----------------------------------------------------------------------------------------------------------------------*/

static const String kShaderMacroHasDepthTargetOnly  = "E_HAS_DEPTH_TARGET_ONLY";
static const String kShaderMacroHasDiffuseMap       = "E_HAS_DIFFUSE_MAP";
static const String kShaderMacroHasLightAmbient     = "E_HAS_LIGHT_AMBIENT";
static const String kShaderMacroHasLight = "E_HAS_LIGHT_DIRECTIONAL";
static const String kShaderMacroHasLightPoint       = "E_HAS_LIGHT_POINT";
static const String kShaderMacroHasLightSpot        = "E_HAS_LIGHT_SPOT";
static const String kShaderMacroHasShadowMap           = "E_HAS_SHADOW_MAP";
static const char*  kShaderEntryPointTable[]        = { "VS", "HS", "DS", "GS", "PS", "CS" };
static const U8     kShaderModelVersion             = 5;

static const String kShaderNameTable[] = 
{
  "ForwardDefault",
  "ForwardDefaultDiffuseMap",
  "ForwardLitAmbient",
  "ForwardLitAmbientDiffuseMap",
  "ForwardLitDirect",
  "ForwardLitDirectDiffuseMap",
  "ForwardLitDirectShadow",
  "ForwardLitDirectShadowDiffuseMap",
  "ForwardLitPoint",
  "ForwardLitPointDiffuseMap",
  "ForwardLitSpot",
  "ForwardLitSpotDiffuseMap",
  "ForwardLitSpotShadow",
  "ForwardLitSpotShadowDiffuseMap",
  "ForwardDepth"
};

static const String kDefaultShaderFileName = "ForwardPass.hlsl";
static const String kLightShaderFileName = "ForwardLightPass.hlsl";

/*----------------------------------------------------------------------------------------------------------------------
ForwardRenderer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::ForwardRenderer::ForwardRenderer()
  : mRenderManager(Graphics::Global::GetRenderManager())
  , mInterFrameConstantBuffer(mRenderManager->GetConstantBuffer(eConstantBufferIDInterFrame))
  , mIntraFrameConstantBuffer(mRenderManager->GetConstantBuffer(eConstantBufferIDIntraFrame))
  , mTransformBuffer(mRenderManager->GetResourceBuffer(eResourceBufferIDTransform))
  , mMaterialBuffer(mRenderManager->GetResourceBuffer(eResourceBufferIDMaterial))
{
  LoadStates();
  LoadSamplers();
  LoadShaders();

  ITexture2D::Descriptor shadowDepthTargetDesc;
  shadowDepthTargetDesc.type = Graphics::ITexture2D::eTypeDepthTarget;
  shadowDepthTargetDesc.format = Graphics::ITexture2D::eFormatDepth32;
  shadowDepthTargetDesc.width = 800;
  shadowDepthTargetDesc.height = 600;
  shadowDepthTargetDesc.unitCount = 1;
  shadowDepthTargetDesc.accessFlags = Graphics::ITexture2D::eAccessFlagGpuRead;
  mShadowDepthTexture = mRenderManager->GetDevice()->CreateTexture2D(shadowDepthTargetDesc);
  E_ASSERT_PTR(mShadowDepthTexture);

  IRenderTarget::Descriptor shadowTargetDescriptor;
  shadowTargetDescriptor.depthTarget = mShadowDepthTexture;
  mShadowTarget = mRenderManager->GetDevice()->CreateContext(shadowTargetDescriptor);
  E_ASSERT_PTR(mShadowTarget);
}

/*----------------------------------------------------------------------------------------------------------------------
ForwardRenderer accessors
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::IRenderer::RendererType Graphics::Scene::ForwardRenderer::GetRendererType() const 
{
  return eRendererTypeForward;
}

void Graphics::Scene::ForwardRenderer::SetDepthBias(I32 depthBias)
{
  IRasterState::Descriptor rasterStateDesc = mRasterStates[eRasterStateIDDepthBias]->GetDescriptor();
  rasterStateDesc.depthBias = depthBias;
  mRasterStates[eRasterStateIDDepthBias] = mRenderManager->GetRasterState(rasterStateDesc);
}

void Graphics::Scene::ForwardRenderer::SetSlopeScaledDepthBias(F32 depthBias)
{
  IRasterState::Descriptor rasterStateDesc = mRasterStates[eRasterStateIDDepthBias]->GetDescriptor();
  rasterStateDesc.slopeScaledDepthBias += depthBias;
  mRasterStates[eRasterStateIDDepthBias] = mRenderManager->GetRasterState(rasterStateDesc);
}

/*----------------------------------------------------------------------------------------------------------------------
ForwardRenderer methods
------------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::ForwardRenderer::Render(const IViewInstance& view, const IWorldInstance& world)
{
  mView = view;
  mWorld = world;

  // Clear buffers
  mInterFrameConstantBuffer->GetBuffer()->Clear();
  mIntraFrameConstantBuffer->GetBuffer()->Clear();
  mTransformBuffer->GetBuffer()->Clear();

  if (
    mWorld->GetWorldState().objectList[IObject::eObjectTypeLight].IsEmpty() &&
    mWorld->GetWorldState().objectList[IObject::eObjectTypeLightPoint].IsEmpty() &&
    mWorld->GetWorldState().objectList[IObject::eObjectTypeLightSpot].IsEmpty())
  {
    RenderDefault();
  }
  else
  {
    RenderLit();
  }

  // Update viewport
  mView->GetViewState().viewport->Update();
}

/*----------------------------------------------------------------------------------------------------------------------
ForwardRenderer private methods
------------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::ForwardRenderer::LoadStates()
{
  IBlendState::Descriptor blendStateDesc;
  mBlendStates[eBlendStateIDDefault] = mRenderManager->GetBlendState(blendStateDesc);

  blendStateDesc.flags                    = IBlendState::eBlendFlagEnableBlend;
  blendStateDesc.sourceFactor             = IBlendState::eBlendFactorOne;
  blendStateDesc.destinationFactor        = IBlendState::eBlendFactorOne;
  blendStateDesc.alphaSourceFactor        = IBlendState::eBlendFactorZero;
  blendStateDesc.alphaDestinationFactor   = IBlendState::eBlendFactorZero;
  mBlendStates[eBlendStateIDAdditive] = mRenderManager->GetBlendState(blendStateDesc);

  IDepthStencilState::Descriptor depthStencilStateDesc;
  mDepthStencilStates[eDepthStencilStateIDDefault] = mRenderManager->GetDepthStencilState(depthStencilStateDesc);
  
  depthStencilStateDesc.depthFunction = IDepthStencilState::eComparisonFunctionLessOrEqual;
  depthStencilStateDesc.flags = IDepthStencilState::eDepthStencilFlagEnableDepth;
  mDepthStencilStates[eDepthStencilStateIDNoDepthWriting] = mRenderManager->GetDepthStencilState(depthStencilStateDesc);

  IRasterState::Descriptor rasterStateDesc;
  mRasterStates[eRasterStateIDDefault] = mRenderManager->GetRasterState(rasterStateDesc);
  
  rasterStateDesc.fillMode = IRasterState::eFillModeWireframe;
  mRasterStates[eRasterStateIDWireframe] = mRenderManager->GetRasterState(rasterStateDesc);

  rasterStateDesc.fillMode = IRasterState::eFillModeSolid;
  //rasterStateDesc.slopeScaledDepthBias = 0.001f;
  rasterStateDesc.depthBias = -0;
  //rasterStateDesc.depthBiasClamp = 0.1f;
  mRasterStates[eRasterStateIDDepthBias] = mRenderManager->GetRasterState(rasterStateDesc);


 /*
  DepthStencilState NoDepthWritesDSS
  {
    DepthEnable = true;
    DepthWriteMask = Zero;
    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;
    FrontFaceStencilFunc = Always;
    FrontFaceStencilPass = Incr;
    FrontFaceStencilFail = Keep;
    BackFaceStencilFunc = Always;
    BackFaceStencilPass = Incr;
    BackFaceStencilFail = Keep;
  };
  */

}
/*
void Graphics::Scene::ForwardRenderer::LoadViewState(const ViewState& viewState)
{
  // Store  view frame buffer
  mView->GetViewState().frameBuffer = viewState.frameBuffer;

  // Store view camera
  mView->GetViewState().camera = viewState.camera;

  // Clear object lists
  for (U32 i = 0; i < IObject::eObjectTypeCount; ++i) mWorld->GetWorldState().objectList[i].Clear();

  // Generate object lists
  for (auto it = begin(viewState.objectList); it != end(viewState.objectList); ++it)
  {
    const IObjectInstance& object = *it;
    mWorld->GetWorldState().objectList[object->GetObjectType()].PushBack(object);
  }
}
*/
void Graphics::Scene::ForwardRenderer::LoadShaders()
{
  IShader::Descriptor shaderDesc;
  shaderDesc.modelVersion = kShaderModelVersion;
  shaderDesc.stages[IShader::eStageVertex].entryPoint = kShaderEntryPointTable[IShader::eStageVertex];
  shaderDesc.stages[IShader::eStagePixel].entryPoint = kShaderEntryPointTable[IShader::eStagePixel];

  // Default shader file based techniques
  shaderDesc.stages[IShader::eStageVertex].filePath = kDefaultShaderFileName;
  shaderDesc.stages[IShader::eStagePixel].filePath = kDefaultShaderFileName;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDDefault], shaderDesc);

  shaderDesc.macroList.Resize(1);
  shaderDesc.macroList[0] = kShaderMacroHasDiffuseMap;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDDefaultDiffuseMap], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLightAmbient;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitAmbient], shaderDesc);

  shaderDesc.macroList.Resize(2);
  shaderDesc.macroList[0] = kShaderMacroHasLightAmbient;
  shaderDesc.macroList[1] = kShaderMacroHasDiffuseMap;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitAmbientDiffuseMap], shaderDesc);

  // Depth shader
  shaderDesc.macroList.Resize(1);
  shaderDesc.macroList[0] = kShaderMacroHasDepthTargetOnly;
  shaderDesc.stages[IShader::eStagePixel].filePath.Clear();
  mRenderManager->SetShader(kShaderNameTable[eShaderIDDepth], shaderDesc);
  
  // Light shader file based techniques
  shaderDesc.stages[IShader::eStageVertex].filePath = kLightShaderFileName;
  shaderDesc.stages[IShader::eStagePixel].filePath = kLightShaderFileName;
  shaderDesc.macroList[0] = kShaderMacroHasLight;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitDirect], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLightPoint;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitPoint], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLightSpot;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitSpot], shaderDesc);

  shaderDesc.macroList.Resize(2);
  shaderDesc.macroList[0] = kShaderMacroHasLight;
  shaderDesc.macroList[1] = kShaderMacroHasDiffuseMap;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitDirectDiffuseMap], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLightPoint;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitPointDiffuseMap], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLightSpot;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitSpotDiffuseMap], shaderDesc);
  
  shaderDesc.macroList[1] = kShaderMacroHasShadowMap;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitSpotShadow], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLight;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitDirectShadow], shaderDesc);

  shaderDesc.macroList.Resize(3);
  shaderDesc.macroList[0] = kShaderMacroHasLightSpot;
  shaderDesc.macroList[1] = kShaderMacroHasDiffuseMap;
  shaderDesc.macroList[2] = kShaderMacroHasShadowMap;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitSpotShadowDiffuseMap], shaderDesc);

  shaderDesc.macroList[0] = kShaderMacroHasLight;
  mRenderManager->SetShader(kShaderNameTable[eShaderIDLitDirectShadowDiffuseMap], shaderDesc);

  for (U32 i = 0; i < eShaderIDCount; ++i) mShaders[i] = mRenderManager->GetShader(kShaderNameTable[i]);
}

void Graphics::Scene::ForwardRenderer::LoadSamplers()
{
  ISampler::Descriptor samplerDesc;
  samplerDesc.filter = ISampler::eFilterMinMagMipPoint;
  mSamplers[eSamplerIDNearestClamp] = mRenderManager->GetSampler(samplerDesc);
  
  samplerDesc.filter = ISampler::eFilterMinMagLinearMipPoint;
  mSamplers[eSamplerIDBilinearClamp] = mRenderManager->GetSampler(samplerDesc);

  samplerDesc.filter = ISampler::eFilterMinMagMipLinear;
  mSamplers[eSamplerIDTrilinearClamp] = mRenderManager->GetSampler(samplerDesc);
  
  samplerDesc.filter = ISampler::eFilterAnisotropic;
  mSamplers[eSamplerIDAnisotropicClamp] = mRenderManager->GetSampler(samplerDesc);

  samplerDesc.filter = ISampler::eFilterComparisonMinMagLinearMipPoint;
  samplerDesc.comparisonFunction = ISampler::eComparisonFunctionLessOrEqual;
  mSamplers[eSamplerIDTrilinearLessOrEqualClamp] = mRenderManager->GetSampler(samplerDesc);

  samplerDesc.addressU = ISampler::eAddressModeWrap;
  samplerDesc.addressV = ISampler::eAddressModeWrap;
  samplerDesc.addressW = ISampler::eAddressModeWrap;
  
  samplerDesc.filter = ISampler::eFilterMinMagMipPoint;
  mSamplers[eSamplerIDNearestWrap] = mRenderManager->GetSampler(samplerDesc);

  samplerDesc.filter = ISampler::eFilterMinMagLinearMipPoint;
  mSamplers[eSamplerIDBilinearWrap] = mRenderManager->GetSampler(samplerDesc);

  samplerDesc.filter = ISampler::eFilterMinMagMipLinear;
  mSamplers[eSamplerIDTrilinearWrap] = mRenderManager->GetSampler(samplerDesc);

  samplerDesc.filter = ISampler::eFilterAnisotropic;
  mSamplers[eSamplerIDAnisotropicWrap] = mRenderManager->GetSampler(samplerDesc);
}

void Graphics::Scene::ForwardRenderer::RenderDefault()
{
  // Clear frame buffer
  mView->GetViewState().frameBuffer->Clear(Color::eDarkestGrey, IRenderTarget::eClearFlagAll);
  mRenderManager->Bind(mView->GetViewState().frameBuffer);

  // Bind constant buffer
  mRenderManager->Bind(mInterFrameConstantBuffer, IShader::eStageVertex, eShaderConstantRegisterInterFrame);
  // Bind resource buffers
  mRenderManager->Bind(mTransformBuffer, IShader::eStageVertex, eShaderResourceRegisterTransform);
  mRenderManager->Bind(mMaterialBuffer, IShader::eStagePixel, eShaderResourceRegisterMaterial);  

  // Apply states
  mRenderManager->Bind(mRasterStates[eRasterStateIDDefault]);
  mRenderManager->Bind(mBlendStates[eBlendStateIDDefault]);
  mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDDefault]);

  // Render view camera
  mView->GetViewState().camera->Render();

  // Render meshes
  for (auto it = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); it != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); ++it)
  {
    IMeshInstance mesh = *it;

    const ITexture2DInstance& diffuseMap = mesh->GetMaterial()->GetDiffuseTexture();
    if (diffuseMap)
    {
      mRenderManager->Bind(diffuseMap, IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
      mRenderManager->Bind(mSamplers[eSamplerIDTrilinearWrap], IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
      mRenderManager->Bind(mShaders[eShaderIDDefaultDiffuseMap]);
    }
    else
    {
      mRenderManager->Bind(mShaders[eShaderIDDefault]);
    }

    // Draw
    mesh->Render();
  }
}

void Graphics::Scene::ForwardRenderer::RenderLightAmbientPass()
{
  // Apply states
  mRenderManager->Bind(mBlendStates[eBlendStateIDDefault]);
  mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDDefault]);

  // Render view camera
  mView->GetViewState().camera->Render();

  // Render ambient pass
  for (auto it = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); it != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); ++it)
  {
    IMeshInstance mesh = *it;

    const ITexture2DInstance& diffuseMap = mesh->GetMaterial()->GetDiffuseTexture();
    if (diffuseMap)
    {
      mRenderManager->Bind(diffuseMap, IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
      mRenderManager->Bind(mSamplers[eSamplerIDTrilinearWrap], IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
      mRenderManager->Bind(mShaders[eShaderIDLitAmbientDiffuseMap]);
    }
    else
    {
      mRenderManager->Bind(mShaders[eShaderIDLitAmbient]);
    }

    // Draw
    mesh->Render();
  }
}

void Graphics::Scene::ForwardRenderer::RenderLightPass()
{
  mRenderManager->Bind(mBlendStates[eBlendStateIDAdditive]);
  mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDNoDepthWriting]);

  // Render per light
  for (auto it = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeLight]); it != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeLight]); ++it)
  {
    ILightInstance light = *it;
   
    RenderShadowPass(light->GetComponent(IObjectComponent::eComponentTypeShadow));

    light->Render();

    // Render meshes
    for (auto it2 = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); it2 != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); ++it2)
    {
      IMeshInstance mesh = *it2;

      const ITexture2DInstance& diffuseMap = mesh->GetMaterial()->GetDiffuseTexture();
      if (diffuseMap)
      {
        mRenderManager->Bind(diffuseMap, IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
        mRenderManager->Bind(mSamplers[eSamplerIDTrilinearWrap], IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
      //  mRenderManager->Bind(mShaders[eShaderIDLitDirectDiffuseMap]);
        IShadowComponentInstance shadowComponent = light->GetComponent(IObjectComponent::eComponentTypeShadow);
        mRenderManager->Bind(shadowComponent == nullptr ? mShaders[eShaderIDLitDirectDiffuseMap] : mShaders[eShaderIDLitDirectShadowDiffuseMap]);
      }
      else
      {
       // mRenderManager->Bind(mShaders[eShaderIDLitDirect]);
        IShadowComponentInstance shadowComponent = light->GetComponent(IObjectComponent::eComponentTypeShadow);
        mRenderManager->Bind(shadowComponent == nullptr ? mShaders[eShaderIDLitDirect] : mShaders[eShaderIDLitDirectShadow]);
      }

      // Draw
      mesh->Render();
    }
  }
};

void Graphics::Scene::ForwardRenderer::RenderLightPointPass()
{
  mRenderManager->Bind(mBlendStates[eBlendStateIDAdditive]);
  mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDNoDepthWriting]);

  // Render per light
  for (auto it = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeLightPoint]); it != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeLightPoint]); ++it)
  {
    ILightInstance light = *it;
    light->Render();

    // Render meshes
    for (auto it2 = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); it2 != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); ++it2)
    {
      IMeshInstance mesh = *it2;

      const ITexture2DInstance& diffuseMap = mesh->GetMaterial()->GetDiffuseTexture();
      if (diffuseMap)
      {
        mRenderManager->Bind(diffuseMap, IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
        mRenderManager->Bind(mSamplers[eSamplerIDTrilinearWrap], IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
        mRenderManager->Bind(mShaders[eShaderIDLitPointDiffuseMap]);
      }
      else
      {
        mRenderManager->Bind(mShaders[eShaderIDLitPoint]);
      }

      // Draw
      mesh->Render();
    }
  }
};

void Graphics::Scene::ForwardRenderer::RenderLightSpotPass()
{
  mRenderManager->Bind(mBlendStates[eBlendStateIDAdditive]);
  mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDNoDepthWriting]);
  //mRenderManager->Bind(mRasterStates[eRasterStateIDDepthBias]);

  // Render per light
  for (auto it = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeLightSpot]); it != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeLightSpot]); ++it)
  {
    ILightInstance light = *it;
    
    RenderShadowPass(light->GetComponent(IObjectComponent::eComponentTypeShadow));
   
    light->Render();

    // Render meshes
    for (auto it2 = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); it2 != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); ++it2)
    {
      IMeshInstance mesh = *it2;

      const ITexture2DInstance& diffuseMap = mesh->GetMaterial()->GetDiffuseTexture();
      if (diffuseMap)
      {
        mRenderManager->Bind(diffuseMap, IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
        mRenderManager->Bind(mSamplers[eSamplerIDTrilinearWrap], IShader::eStagePixel, eShaderResourceRegisterDiffuseMap);
        IShadowComponentInstance shadowComponent = light->GetComponent(IObjectComponent::eComponentTypeShadow);
        mRenderManager->Bind(shadowComponent == nullptr ? mShaders[eShaderIDLitSpotDiffuseMap] : mShaders[eShaderIDLitSpotShadowDiffuseMap]);

      }
      else
      {
        IShadowComponentInstance shadowComponent = light->GetComponent(IObjectComponent::eComponentTypeShadow);
        mRenderManager->Bind(shadowComponent == nullptr ? mShaders[eShaderIDLitSpot] : mShaders[eShaderIDLitSpotShadow]);
      }

      // Draw
      mesh->Render();
    }
  }

  //mRenderManager->Bind(mRasterStates[eRasterStateIDDefault]);
};

void Graphics::Scene::ForwardRenderer::RenderShadowPass(const IShadowComponentInstance& shadowComponent)
{
  // Render view camera
  if (shadowComponent)
  {
    // Unbind shadow map
    mRenderManager->Unbind(mShadowDepthTexture);
    
    // Apply states
    //mRenderManager->Bind(mRasterStates[eRasterStateIDDefault]);
    mRenderManager->Bind(mBlendStates[eBlendStateIDDefault]);
    mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDDefault]);

    // Clear and apply shadow target 
    mShadowTarget->Clear(Color::eBlack, IRenderTarget::eClearFlagDepth);
    mRenderManager->Bind(mShadowTarget);

    // Set current view dimensions to the shadow map view
    const IViewport::Descriptor viewportDesc = mView->GetViewState().viewport->GetDescriptor();
    ICameraInstance shadowView =  shadowComponent->GetLightView();
    
    shadowView->SetViewportDimensions(viewportDesc.width, viewportDesc.height);
    /*
    if (shadowComponent->GetOwner()->GetObjectType() == IObject::eObjectTypeLight)
    {
      const Frustum& frustum = mView->GetViewState().camera->GetFrustum();
      Vector3f lightDir = Matrix4f::RotateVector(shadowView->GetWorldMatrix(), Vector3f::AxisZ());
      Spheref boundingSphere = frustum.GetBoundingSphere();
      shadowView->SetPosition(boundingSphere.GetOrigin() - lightDir * boundingSphere.GetRadius());
      shadowView->Update(0);
    }*/
    shadowComponent->GetLightView()->Render();

    // Render meshes
    for (auto it = begin(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); it != end(mWorld->GetWorldState().objectList[IObject::eObjectTypeMesh]); ++it)
    {
      IMeshInstance mesh = *it;
      // Apply default shader state
      mRenderManager->Bind(mShaders[eShaderIDDepth]);
      // Draw
      mesh->Render();
    }

    // Restore frame buffer
    mRenderManager->Bind(mView->GetViewState().frameBuffer);

    // Apply states
    //mRenderManager->Bind(mRasterStates[eRasterStateIDDefault]);
    mRenderManager->Bind(mBlendStates[eBlendStateIDAdditive]);
    mRenderManager->Bind(mDepthStencilStates[eDepthStencilStateIDNoDepthWriting]);

    // Submit shader texture
    mRenderManager->Bind(mShadowDepthTexture, IShader::eStagePixel, 4);
    mRenderManager->Bind(mSamplers[eSamplerIDTrilinearLessOrEqualClamp], IShader::eStagePixel, 4);

    // Restore camera
    mView->GetViewState().camera->Render();
  }
}

void Graphics::Scene::ForwardRenderer::RenderLit()
{
  // Clear frame buffer
  mView->GetViewState().frameBuffer->Clear(Color::eBlack, IRenderTarget::eClearFlagAll);
  mRenderManager->Bind(mView->GetViewState().frameBuffer);

  // Bind constant buffers
  mRenderManager->Bind(mInterFrameConstantBuffer, IShader::eStageVertex, eShaderConstantRegisterInterFrame);
  mRenderManager->Bind(mIntraFrameConstantBuffer, IShader::eStageVertex, eShaderConstantRegisterIntraFrame);
  mRenderManager->Bind(mInterFrameConstantBuffer, IShader::eStagePixel, eShaderConstantRegisterInterFrame);
  mRenderManager->Bind(mIntraFrameConstantBuffer, IShader::eStagePixel, eShaderConstantRegisterIntraFrame);

  // Bind resource buffers
  mRenderManager->Bind(mTransformBuffer, IShader::eStageVertex, eShaderResourceRegisterTransform);
  mRenderManager->Bind(mMaterialBuffer, IShader::eStagePixel, eShaderResourceRegisterMaterial); 

  // Apply states
  mRenderManager->Bind(mRasterStates[eRasterStateIDDefault]);
  mRenderManager->Bind(mBlendStates[eBlendStateIDDefault]);
  mRenderManager->Bind(mBlendStates[eDepthStencilStateIDDefault]);

  RenderLightAmbientPass();
  RenderLightPass();
  RenderLightPointPass();
  RenderLightSpotPass();
}
