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

// Created 05-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file RenderTestApplication.cpp
This file defines the RenderTestApplication class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
----------------------------------------------------------------------------------------------------------------------*/

#define E_DEBUG_MATRIX4(matrix) \
  { \
    StringBuffer sb; \
    sb.Clear(); \
    sb << E_TO_STR(matrix); \
    for (U32 i = 0; i < 4; ++i) \
    { \
      sb << "\n"; \
      for (U32 j = 0; j < 4; ++j) sb << matrix(i, j) << ", "; \
    } \
    sb << "\n"; \
    E_DEBUG_OUTPUT(sb.GetPtr()); \
  }\

Matrix4f GetTransform(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale);

/*----------------------------------------------------------------------------------------------------------------------
RenderTestApplication initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

RenderTestApplication::RenderTestApplication()
  : mRenderManager(Global::GetRenderManager()) {}

RenderTestApplication::~RenderTestApplication()
{
  if (mViewport)
  {
    mViewportContext = nullptr;
    mViewport = nullptr;
  }
  mRenderManager->Finalize();
}

void RenderTestApplication::Initialize(U32 width, U32 height)
{
  // Configure render settings
  IRenderManager::Settings& renderSettings = Global::GetRenderManager()->GetSettings();
  renderSettings.dataRootDirectory = "..\\..\\..\\Data\\";
  renderSettings.shaderFolder = "Shaders\\Fixed\\";
  renderSettings.textureFolder = "Textures\\";
  renderSettings.verticalSync = true;

  // Create main window
  Application::Application& app = Application::Global::GetApplication();
  mpWindow = app.CreateMainWindow(width, height);
  mpWindow->GetUpdateEventCallback() += this;
  mpWindow->GetKeyDownEventCallback() += this;

  ThrowIf(mRenderManager == nullptr, Exception::eExceptionTypeEngineTest);
  mRenderManager->Initialize();

  LoadTriangle();

  app.Run();
}

/*----------------------------------------------------------------------------------------------------------------------
RenderTestApplication methods
----------------------------------------------------------------------------------------------------------------------*/


void RenderTestApplication::OnEvent(const Application::UpdateEvent&)
{
  mViewportContext->Clear(Color::eDarkGrey, IRenderTarget::eClearFlagAll);

  // Next frame
  mRenderManager->Bind(mViewportContext);
  mRenderManager->Bind(mBlendState);
  mRenderManager->Bind(mDepthStencilState);
  mRenderManager->Bind(mRasterState);
  mRenderManager->Bind(mShader);
  mRenderManager->Bind(mConstantBuffer, IShader::eStageVertex, 0);
  mRenderManager->Bind(mTransformBuffer, IShader::eStageVertex, 9);
  mRenderManager->Bind(mVertexArray);
  mRenderManager->Draw(mDrawState);

  // Update frame
  mViewport->Update();
}

void RenderTestApplication::OnEvent(const Application::KeyDownEvent& event)
{
  if (event.key == Application::Input::eKeyEscape)
  {
    Application::Global::GetApplication().Terminate();
  }
}

/*----------------------------------------------------------------------------------------------------------------------
RenderTestApplication private methods
----------------------------------------------------------------------------------------------------------------------*/

void RenderTestApplication::LoadTriangle()
{
  // Create viewport
  const Application::IWindow::Descriptor& windowDesc = mpWindow->GetDescriptor();
  mRenderManager->SetViewport(
    IRenderManager::eViewportID0,
    windowDesc.handle,
    windowDesc.width,
    windowDesc.height,
    windowDesc.mode == Application::IWindow::eModeFullScreen);

  mViewport = mRenderManager->GetViewport(IRenderManager::eViewportID0);
  ThrowIf(mViewport == nullptr, Exception::eExceptionTypeEngineTest);
  mViewportContext = mRenderManager->GetFrameBuffer(IRenderManager::eViewportID0);
  ThrowIf(mViewportContext == nullptr, Exception::eExceptionTypeEngineTest);

  // Set the default states
  mBlendState = mRenderManager->GetBlendState(IBlendState::Descriptor::Default());
  mDepthStencilState = mRenderManager->GetDepthStencilState(IDepthStencilState::Descriptor::Default());
  mRasterState = mRenderManager->GetRasterState(IRasterState::Descriptor::Default());

  // Get Vertex array
  mRenderManager->SetVertexLayout(IRenderManager::eVertexLayoutID0, CompressedPositionColorVertex::GetVertexLayoutDescriptor());
  mVertexArray.vertexLayout = mRenderManager->GetVertexLayout(IRenderManager::eVertexLayoutID0);

  // Create vertex buffer
  IBuffer::Descriptor vertexBufferDescriptor;
  vertexBufferDescriptor.type = IBuffer::eTypeVertex;
  vertexBufferDescriptor.accessFlags = IBuffer::eAccessFlagGpuRead;
  vertexBufferDescriptor.elementSize = mVertexArray.vertexLayout->GetDescriptor().GetVertexSize();
  mVertexArray.vertexBuffer = mRenderManager->GetDevice()->CreateBuffer(vertexBufferDescriptor);

  // Define vertices
  F32 r = 100.0f;
  CompressedPositionColorVertex vertices[3];
  vertices[0].position = Vector3f(-r, -r, 0.0f);
  vertices[1].position = Vector3f( 0.0f, r,  0.0f);
  vertices[2].position = Vector3f(r, -r,  0.0f);
  vertices[0].color = Color(Color::eRed).GetRGBA();
  vertices[1].color = Color(Color::eGreen).GetRGBA();
  vertices[2].color = Color(Color::eBlue).GetRGBA();
  vertices[0].meshID = 0;
  vertices[1].meshID = 0;
  vertices[2].meshID = 0;
  
  // Add vertices
  mVertexArray.vertexBuffer->Add(&vertices[0], 3);

  // Create the draw state
  mDrawState.vertexPrimitive = eVertexPrimitiveTriangleList;
  mDrawState.vertexCount = 3;
  
  // Create shader
  IShader::Descriptor shaderDescriptor;
  shaderDescriptor.modelVersion = 5;
  shaderDescriptor.stages[IShader::eStageVertex].filePath  = "PositionColor.hlsl";
  shaderDescriptor.stages[IShader::eStageVertex].entryPoint = "VS";
  shaderDescriptor.stages[IShader::eStagePixel].filePath  = "PositionColor.hlsl";
  shaderDescriptor.stages[IShader::eStagePixel].entryPoint = "PS";
  mRenderManager->SetShader("Color", shaderDescriptor);

  // Get shader
  mShader = mRenderManager->GetShader("Color");
  ThrowIf(!mShader, Exception::eExceptionTypeEngineTest);

  // Fill camera data to a constant buffer
  mConstantBuffer = mRenderManager->GetConstantBuffer(IRenderManager::eConstantBufferID0);
  Matrix4f viewMatrix;
  viewMatrix[12] = 0.0f;
  viewMatrix[13] = 0.0f;
  viewMatrix[14] = 100.0f;

  Matrix4f projectionMatrix = Math::BuildOrthographicLH(
    mViewport->GetDescriptor().width,
    mViewport->GetDescriptor().height,
    1.0f, 1000.0f);

  Matrix4f viewProjectionMatrix = viewMatrix * projectionMatrix;
  //viewProjectionMatrix.Transpose(); // No need to transpose matrices before submit to GPU due to #pragma pack_matrix (row_major)
  mConstantBuffer->Set(0, viewProjectionMatrix);
  
  // Create transform buffer
  mRenderManager->SetResourceBuffer(IRenderManager::eResourceBufferID0, 64);
  
  // Set transform data
  mTransformBuffer = mRenderManager->GetResourceBuffer(IRenderManager::eResourceBufferID0);
  Matrix4f worldMatrix;
  worldMatrix = GetTransform(
    Vector3f(0.0f, 50.0f, 0.0f),
    Vector3f(0.0f, 0.0f, 45.0f),
    Vector3f(1.0f, 1.2f, 1.0f));
  mTransformBuffer->Set(0, &worldMatrix[0]);

  /*
  //transformBuffer->Set(0, &Matrix4f::Identity()[0]);
  E_DEBUG_MATRIX4(viewMatrix);
  E_DEBUG_MATRIX4(projectionMatrix);
  E_DEBUG_MATRIX4(viewProjectionMatrix);
  */
}

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
----------------------------------------------------------------------------------------------------------------------*/

Matrix4f GetTransform(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
{
  Matrix4f worldMatrix;
  // Update rotation
  Quatf qRotation;
  qRotation.SetRotation(Vector3f(
    Math::Rad(rotation.x),
    Math::Rad(rotation.y),
    Math::Rad(rotation.z)));
  qRotation.GetRotation(worldMatrix);
  // Update translation
  worldMatrix.SetTranslation(position);
  // Update scaling
  if (scale != 1.0f) worldMatrix.Scale(scale);
  
  // Transpose transformation as resource buffer does not take into account data format. Hence #pragma pack_matrix (row_major) has no effect
  worldMatrix.Transpose();

  return worldMatrix;

  /*
  // Matrix composition equivalent
  Matrix4f translationMatrix;
  translationMatrix.SetTranslation(position);
  Matrix4f scaleMatrix;
  scaleMatrix.SetScale(scale);
  Matrix4f rotationMatrix;
  qRotation.GetRotation(rotationMatrix);

  return translationMatrix * scaleMatrix * rotationMatrix;
  */
}