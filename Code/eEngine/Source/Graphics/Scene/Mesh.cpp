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

// Created 08-Jun-2014 by Elías Lozada-Benavente
// Original 15-Nov-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Mesh.cpp
This file defines the Mesh class.
*/

#include <EnginePch.h>
#include "Mesh.h"
#include "MeshHelper.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
SceneManager assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_MESH_VERTEX_TYPE_HANDLER_MISSING    "There is no vertex handler for the supplied vertex type"

/*----------------------------------------------------------------------------------------------------------------------
Mesh auxiliary
----------------------------------------------------------------------------------------------------------------------*/

const E::Graphics::IVertexLayout::Descriptor kDefaultVertexLayoutTable[] =
{
  E::Graphics::PositionColorVertex::GetVertexLayoutDescriptor(),
  E::Graphics::CompressedPositionColorVertex::GetVertexLayoutDescriptor(),
  E::Graphics::PositionTextureVertex::GetVertexLayoutDescriptor(),
  E::Graphics::CompressedPositionTextureVertex::GetVertexLayoutDescriptor(),
  E::Graphics::PositionNormalVertex::GetVertexLayoutDescriptor(),
  E::Graphics::CompressedPositionNormalVertex::GetVertexLayoutDescriptor(),
  E::Graphics::PositionTextureNormalVertex::GetVertexLayoutDescriptor(),
  E::Graphics::CompressedPositionTextureNormalVertex::GetVertexLayoutDescriptor()
};

/*----------------------------------------------------------------------------------------------------------------------
Mesh initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

// Known warning: passing this in the initializer list to mCore
#pragma warning(push)
#pragma warning (disable:4355)
Graphics::Scene::Mesh::Mesh()
  : mCore(this)
  , mRenderManager(Graphics::Global::GetRenderManager())
  , mTransformBuffer(mRenderManager->GetResourceBuffer(eResourceBufferIDTransform))
  , mMaterialBuffer(mRenderManager->GetResourceBuffer(eResourceBufferIDMaterial))
  , mMaterial(Global::GetSceneManager()->GetDefaultMaterial())
  , mMeshID(static_cast<U32>(-1))
  , mCustomVertexType(eVertexTypeAutomatic) {}
#pragma warning(pop)

/*----------------------------------------------------------------------------------------------------------------------
Mesh accessors
----------------------------------------------------------------------------------------------------------------------*/

U32 Graphics::Scene::Mesh::GetID() const
{
  return mMeshID;
}

const Graphics::Scene::IMaterialInstance& Graphics::Scene::Mesh::GetMaterial() const
{
  return mMaterial;
}

void Graphics::Scene::Mesh::SetMaterial(IMaterialInstance material)
{
  mMaterial = material;
}

void Graphics::Scene::Mesh::SetShader(const String& shaderTechniqueName)
{
  mCustomShaderName = shaderTechniqueName;
}

void Graphics::Scene::Mesh::SetVertexType(VertexType vertexType)
{
  mCustomVertexType = vertexType;
}

/*----------------------------------------------------------------------------------------------------------------------
Mesh methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::Mesh::Clear()
{
  //Memory::Zero(&mMaterial);
  mCustomShaderName.Clear();
  mCustomVertexType = eVertexTypeAutomatic; 
  mCore.ClearTransform();
}

void Graphics::Scene::Mesh::CreateCube(F32 length)
{
  MeshHelper::CreateCube(mMeshBuffer, length);
}

void Graphics::Scene::Mesh::CreateCylinder(F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount)
{
  MeshHelper::CreateCylinder(mMeshBuffer, topRadius, bottomRadius, height, sliceCount, stackCount);
}

bool Graphics::Scene::Mesh::CreateFromFile(const FilePath& filePath)
{
  Assimp::Importer assimpImporter;
  const aiScene* pAssimpScene = assimpImporter.ReadFile(
    filePath.GetPtr(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType);

  if (pAssimpScene == nullptr) return false;

  E_ASSERT_TODO;

  return true;
}

void Graphics::Scene::Mesh::CreateQuad(F32 length)
{
  MeshHelper::CreateQuad(mMeshBuffer, length, length);
}

void Graphics::Scene::Mesh::CreateQuad(F32 width, F32 height)
{
  MeshHelper::CreateQuad(mMeshBuffer, width, height);
}

void Graphics::Scene::Mesh::CreateSphere(F32 radius, U32 sliceCount, U32 stackCount)
{
  MeshHelper::CreateSphere(mMeshBuffer, radius, sliceCount, stackCount);
}

void Graphics::Scene::Mesh::CreateTriangle(F32 length)
{
  MeshHelper::CreateTriangle(mMeshBuffer, length);
}

void Graphics::Scene::Mesh::Load()
{
  // Get a mesh ID
  mMeshID = mTransformBuffer->AcquireIndex();
  // Load mesh
  LoadVertexState();
  LoadMaterial();
  LoadDrawState();
  // Load core object
  mCore.Load();
}

void Graphics::Scene::Mesh::Unload()
{
  // Release mesh ID
  mTransformBuffer->ReleaseIndex(mMeshID);
  // Unload core object
  mCore.Unload();
}

void Graphics::Scene::Mesh::Render()
{
  // Update transform buffer with world matrix
  E_ASSERT(mMeshID != -1);
  Matrix4f transposedWorldMatrix = Matrix4f::Transpose(mCore.GetWorldMatrix());
  mTransformBuffer->Set(mMeshID, &transposedWorldMatrix[0]);
  mRenderManager->Update(mTransformBuffer);
  // Bind vertex array
  mRenderManager->Bind(mVertexArray);
  // Draw
  mRenderManager->Draw(mDrawState);
  // Render children
  mCore.RenderChildren();
}

void Graphics::Scene::Mesh::Update(const TimeValue& deltaTime)
{
  // Update only active camera
  mCore.Update(deltaTime);
}

/*----------------------------------------------------------------------------------------------------------------------
Mesh private methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::Mesh::LoadDrawState()
{
  // Configure draw state
  mDrawState.vertexPrimitive = eVertexPrimitiveTriangleList;
  mDrawState.vertexCount = static_cast<U32>(mMeshBuffer.positionList.GetCount());
  mDrawState.startVertex = mVertexArray.vertexBuffer->GetCount() - mDrawState.vertexCount;
  mDrawState.indexCount = static_cast<U32>(mMeshBuffer.indexList.GetCount());
  mDrawState.startIndex = mVertexArray.indexBuffer->GetCount() - mDrawState.indexCount;
}

void Graphics::Scene::Mesh::LoadMaterial()
{
  // Set material properties
  mMaterialBuffer->Set(mMeshID, &mMaterial->GetShaderProperties());
  mRenderManager->Update(mMaterialBuffer);
}

void Graphics::Scene::Mesh::LoadVertexState()
{
  // Select vertex type
  VertexType vertexType = SelectVertexType();
  // Get vertex layout
  mVertexArray.vertexLayout = mRenderManager->GetVertexLayout(vertexType);
  E_ASSERT_PTR(mVertexArray.vertexLayout);
  // Create vertex buffer
  IBuffer::Descriptor vertexBufferDescriptor;
  vertexBufferDescriptor.type = IBuffer::eTypeVertex;
  vertexBufferDescriptor.accessFlags = IBuffer::eAccessFlagGpuRead;
  vertexBufferDescriptor.elementSize = mVertexArray.vertexLayout->GetDescriptor().GetVertexSize();
  // Destroy previous vertex buffer
  mVertexArray.vertexBuffer = mRenderManager->GetDevice()->CreateBuffer(vertexBufferDescriptor);
  E_ASSERT_PTR(mVertexArray.vertexBuffer);

  // Create index buffer
  IBuffer::Descriptor indexBufferDescriptor;
  indexBufferDescriptor.type = IBuffer::eTypeIndex;
  indexBufferDescriptor.accessFlags = IBuffer::eAccessFlagGpuRead;
  indexBufferDescriptor.elementSize = sizeof(U32);
  mVertexArray.indexBuffer = mRenderManager->GetDevice()->CreateBuffer(indexBufferDescriptor);
  E_ASSERT_PTR(mVertexArray.indexBuffer);

  // Add vertex data
  switch (vertexType)
  {
  case eVertexTypePosition: UpdatePositionVertexData();
    break;
  case eVertexTypePositionTexture: UpdateCompressedPositionTextureVertexData();
    break;
  case eVertexTypePositionNormal: UpdateCompressedPositionNormalVertexData();
    break;
  case eVertexTypePositionTextureNormal: UpdateCompressedPositionTextureNormalVertexData();
    break;
  default:
    E_ASSERT_ALWAYS(E_ASSERT_MSG_MESH_VERTEX_TYPE_HANDLER_MISSING);
  }
  // Add index data
  if (mMeshBuffer.indexList.GetCount()) mVertexArray.indexBuffer->Add(mMeshBuffer.indexList.GetPtr(), static_cast<U32>(mMeshBuffer.indexList.GetCount()));
}

Graphics::Scene::IMesh::VertexType Graphics::Scene::Mesh::SelectVertexType()
{
  if (mCustomVertexType != eVertexTypeAutomatic) return mCustomVertexType;
  E_ASSERT_PTR(mMaterial);

  if (mMeshBuffer.normalList.GetCount())
  {
    return (mMeshBuffer.texCoordList.GetCount() && mMaterial->GetDiffuseTexture()) ? 
      eVertexTypePositionTextureNormal : eVertexTypePositionNormal;
  }
  else
  {
    return (mMeshBuffer.texCoordList.GetCount() && mMaterial->GetDiffuseTexture()) ? 
      eVertexTypePositionTexture : eVertexTypePosition;
  }

}

void Graphics::Scene::Mesh::UpdateCompressedPositionNormalVertexData()
{
  CompressedPositionNormalVertex vertex;
  const IBufferInstance& vertexBuffer = mVertexArray.vertexBuffer;
  for (U32 i = 0; i < mMeshBuffer.positionList.GetCount(); ++i)
  {
    vertex.position = mMeshBuffer.positionList[i];
    vertex.meshID = mMeshID;
    vertex.normal.x = Math::Half(mMeshBuffer.normalList[i].x);
    vertex.normal.y = Math::Half(mMeshBuffer.normalList[i].y);
    vertex.normal.z = Math::Half(mMeshBuffer.normalList[i].z);
    vertexBuffer->Add(&vertex, 1);
  }
}

void Graphics::Scene::Mesh::UpdateCompressedPositionTextureNormalVertexData()
{
  CompressedPositionTextureNormalVertex vertex;
  const IBufferInstance& vertexBuffer = mVertexArray.vertexBuffer;
  for (U32 i = 0; i < mMeshBuffer.positionList.GetCount(); ++i)
  {
    vertex.position = mMeshBuffer.positionList[i];
    vertex.meshID = mMeshID;
    vertex.texCoord.x = Math::Half(mMeshBuffer.texCoordList[i].x);
    vertex.texCoord.y = Math::Half(mMeshBuffer.texCoordList[i].y);
    vertex.normal.x = Math::Half(mMeshBuffer.normalList[i].x);
    vertex.normal.y = Math::Half(mMeshBuffer.normalList[i].y);
    vertex.normal.z = Math::Half(mMeshBuffer.normalList[i].z);
    vertexBuffer->Add(&vertex, 1);
  }
}

void Graphics::Scene::Mesh::UpdateCompressedPositionTextureVertexData()
{
  CompressedPositionTextureVertex vertex;
  const IBufferInstance& vertexBuffer = mVertexArray.vertexBuffer;
  for (U32 i = 0; i < mMeshBuffer.positionList.GetCount(); ++i)
  {
    vertex.position = mMeshBuffer.positionList[i];
    vertex.meshID = mMeshID;
    vertex.texCoord.x = Math::Half(mMeshBuffer.texCoordList[i].x);
    vertex.texCoord.y = Math::Half(mMeshBuffer.texCoordList[i].y);
    vertexBuffer->Add(&vertex, 1);
  }
}

void Graphics::Scene::Mesh::UpdatePositionVertexData()
{
  PositionColorVertex vertex;
  const IBufferInstance& vertexBuffer = mVertexArray.vertexBuffer;

  for (U32 i = 0; i < mMeshBuffer.positionList.GetCount(); ++i)
  {
    vertex.position = mMeshBuffer.positionList[i];
    vertex.meshID = mMeshID;
    vertexBuffer->Add(&vertex, 1);
  }
}
