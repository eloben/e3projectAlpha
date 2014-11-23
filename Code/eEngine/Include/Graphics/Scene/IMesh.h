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

// Created 06-Jun-2014 by Elías Lozada-Benavente
// Original created 30-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IMesh.h
This file defines the IMesh interface.
*/

#ifndef E3_IMESH_H
#define E3_IMESH_H

#include <Graphics/Scene/IMaterial.h>
#include <Graphics/Scene/IObject.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
IMesh
----------------------------------------------------------------------------------------------------------------------*/
class IMesh : public IObject
{
public:
  enum VertexType
  {
    eVertexTypePosition,
    eVertexTypePositionTexture,
    eVertexTypePositionNormal,
    eVertexTypePositionTextureNormal,
    eVertexTypeAutomatic
  };

  // Accessors
  virtual U32                       GetID() const = 0;
  virtual const IMaterialInstance&  GetMaterial() const = 0;
  virtual void                      SetMaterial(IMaterialInstance material) = 0;
  virtual void                      SetShader(const String& shaderTechniqueName) = 0;
  virtual void                      SetVertexType(VertexType vertexType) = 0;

  // Methods
  virtual void                      CreateCube(F32 length) = 0;
  virtual void                      CreateCylinder(F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount) = 0;
  virtual bool                      CreateFromFile(const FilePath& filePath) = 0;
  virtual void                      CreateQuad(F32 length) = 0;
  virtual void                      CreateQuad(F32 width, F32 height) = 0;
  virtual void                      CreateSphere(F32 radius, U32 sliceCount, U32 stackCount) = 0;
  virtual void                      CreateTriangle(F32 length) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IMesh types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IMesh> IMeshInstance;
}
}
}

#endif
