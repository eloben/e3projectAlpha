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

/** @file Mesh.h
This file declares the Mesh class.
*/

#ifndef E3_MESH_H
#define E3_MESH_H

#include "Material.h"
#include "ObjectCore.h"

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
Mesh
----------------------------------------------------------------------------------------------------------------------*/
class Mesh : public IMesh
{
public:
  Mesh();

  E_GRAPHICS_DEFINE_SCENE_OBJECT_COMMON(eObjectTypeMesh, mCore)

  // Accessors
  U32                       GetID() const;
  const IMaterialInstance&  GetMaterial() const;
  void                      SetMaterial(IMaterialInstance material);
  void                      SetShader(const String& shaderTechniqueName);
  void                      SetVertexType(VertexType vertexType);

  // Methods
  void                      Clear();
  void	                    CreateCube(F32 length);
  void                      CreateCylinder(F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount);
  bool                      CreateFromFile(const FilePath& filePath);
  void                      CreateQuad(F32 length);
  void                      CreateQuad(F32 width, F32 height);
  void                      CreateSphere(F32 radius, U32 sliceCount, U32 stackCount);
  void                      CreateTriangle(F32 length);
  void                      Load();
  void                      Render();
  void                      Unload();
  void                      Update(const TimeValue& deltaTime);

private:
  ObjectCore                mCore;
  IRenderManagerInstance    mRenderManager;
  IResourceBufferInstance   mTransformBuffer;
  IResourceBufferInstance   mMaterialBuffer;
  MeshBuffer                mMeshBuffer;
  DrawState                 mDrawState;
  VertexArray               mVertexArray;
  IShaderInstance           mShader;
  IMaterialInstance         mMaterial;
  U32                       mMeshID;
  String                    mCustomShaderName;
  VertexType                mCustomVertexType;

  void                      LoadDrawState();
  void                      LoadMaterial();
  void                      LoadVertexState();
  VertexType                SelectVertexType();
  void                      UpdateCompressedPositionNormalVertexData();
  void                      UpdateCompressedPositionTextureNormalVertexData();
  void                      UpdateCompressedPositionTextureVertexData();
  void                      UpdatePositionVertexData();

  void                      ValidateMeshBuffer();
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(Mesh)
}; 
}
}
}

#endif
