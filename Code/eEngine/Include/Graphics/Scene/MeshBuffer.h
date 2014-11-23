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

// Created 13-Aug-2014 by Elías Lozada-Benavente
// Original 25-Feb-2012 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file MeshBuffer.h
This file declares the MeshBuffer class.
*/

#ifndef E3_MESH_BUFFER_H
#define E3_MESH_BUFFER_H

#include <Graphics/Color.h>
#include <Math/Vector4.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>
#include <Containers/List.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
MeshBuffer
----------------------------------------------------------------------------------------------------------------------*/
struct MeshBuffer
{
  Containers::List<Vector3f>  positionList;
  Containers::List<Vector2f>  texCoordList;
  Containers::List<Vector3f>  normalList;
  Containers::List<Vector4f>  tangentList;
  Containers::List<Color>     colorList;
  Containers::List<U32>       indexList;
  VertexPrimitive primitive;

  void Add(const Vector3f& position, const Color& color)
  {
    positionList.PushBack(position);
    colorList.PushBack(color);
  }

  void Add(const Vector3f& position, const Vector2f& texCoord, const Vector3f& normal)
  {
    positionList.PushBack(position);
    texCoordList.PushBack(texCoord);
    normalList.PushBack(normal);
  }

  void Add(const Vector3f& position, const Vector2f& texCoord, const Vector3f& normal, const Vector4f& tangent)
  {
    positionList.PushBack(position);
    texCoordList.PushBack(texCoord);
    normalList.PushBack(normal);
    tangentList.PushBack(tangent);
  }

  void Clear()
  {
    positionList.Clear();
    texCoordList.Clear();
    normalList.Clear();
    tangentList.Clear();
    colorList.Clear();
    indexList.Clear();
  }
};
}
}
}

#endif
