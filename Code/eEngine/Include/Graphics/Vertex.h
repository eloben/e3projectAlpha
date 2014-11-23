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

// Created 15-Aug-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Vertex.h
This file defines the Vertex class.
*/

#ifndef E3_VERTEX_H
#define E3_VERTEX_H

#include <Graphics/Color.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
ShaderMacros
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Vector2<U16>  Vector2h;
typedef Math::Vector4<U16>  Vector4h;

namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
ShaderMacros
----------------------------------------------------------------------------------------------------------------------*/
namespace ShaderMacros
{
  static const String kUseVertexCompression = "E_USE_VERTEX_COMPRESSION";
}

/*----------------------------------------------------------------------------------------------------------------------
PositionVertex
----------------------------------------------------------------------------------------------------------------------*/
struct PositionVertex
{
  Vector3f  position;
  U32       meshID;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(2);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
PositionColorVertex
----------------------------------------------------------------------------------------------------------------------*/
struct PositionColorVertex
{
  Vector3f  position;
  U32       meshID;
  Color     color;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(3);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatFloat4;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeColor;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
CompressedPositionColorVertex
----------------------------------------------------------------------------------------------------------------------*/
struct CompressedPositionColorVertex
{
  Vector3f  position;
  U32       meshID;
  U32       color;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(3);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeColor;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
PositionTextureVertex
----------------------------------------------------------------------------------------------------------------------*/
struct PositionTextureVertex
{
  Vector3f position;
  U32      meshID;
  Vector2f texCoord;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(3);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatFloat2;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeTexCoord;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
CompressedPositionTextureVertex
----------------------------------------------------------------------------------------------------------------------*/
struct CompressedPositionTextureVertex
{
  Vector3f  position;
  U32       meshID;
  Vector2h  texCoord;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(3);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatHalf2;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeTexCoord;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
PositionNormalVertex
----------------------------------------------------------------------------------------------------------------------*/
struct PositionNormalVertex
{
  Vector3f  position;
  U32       meshID;
  Vector3f  normal;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(3);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeNormal;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
CompressedPositionNormalVertex
----------------------------------------------------------------------------------------------------------------------*/
struct CompressedPositionNormalVertex
{
  Vector3f  position;
  U32       meshID;
  Vector4h  normal;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(3);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatHalf4;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeNormal;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
PositionTextureNormalVertex
----------------------------------------------------------------------------------------------------------------------*/
struct PositionTextureNormalVertex
{
  Vector3f position;
  U32      meshID;
  Vector2f texCoord;
  Vector3f normal;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(4);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatFloat2;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeTexCoord;
      sDescriptor.elements[3].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[3].type   = IVertexLayout::Element::eTypeNormal;
    }
    return sDescriptor;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
CompressedPositionTextureNormalVertex
----------------------------------------------------------------------------------------------------------------------*/
struct CompressedPositionTextureNormalVertex
{
  Vector3f  position;
  U32       meshID;
  Vector2h  texCoord;
  Vector4h  normal;

  static IVertexLayout::Descriptor& GetVertexLayoutDescriptor()
  {
    static IVertexLayout::Descriptor sDescriptor;
    if (sDescriptor.elements.GetSize() == 0)
    {
      sDescriptor.elements.Reserve(4);
      sDescriptor.elements[0].format = IVertexLayout::Element::eFormatFloat3;
      sDescriptor.elements[0].type   = IVertexLayout::Element::eTypePosition;
      sDescriptor.elements[1].format = IVertexLayout::Element::eFormatUnsigned;
      sDescriptor.elements[1].type   = IVertexLayout::Element::eTypeID;
      sDescriptor.elements[2].format = IVertexLayout::Element::eFormatHalf2;
      sDescriptor.elements[2].type   = IVertexLayout::Element::eTypeTexCoord;
      sDescriptor.elements[3].format = IVertexLayout::Element::eFormatHalf4;
      sDescriptor.elements[3].type   = IVertexLayout::Element::eTypeNormal;
    }
    return sDescriptor;
  }
};
}
}

#endif
