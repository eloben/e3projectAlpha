/*--------------------------------------------------------------------------------------------------------------------------------------
This source file is part of the E2 Engine

Copyright (c) 2010-2012 Elías Lozada-Benavente

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
---------------------------------------------------------------------------------------------------------------------------------------*/

// Created 13-Aug-2014 by Elías Lozada-Benavente
// Original 25-Feb-2012 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file MeshHelper.h
This file declares the MeshHelper class.
*/

#ifndef E3_MESH_GENERATION_H
#define E3_MESH_GENERATION_H

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
MeshHelper
----------------------------------------------------------------------------------------------------------------------*/
namespace MeshHelper
{
  void CreateCube(Graphics::Scene::MeshBuffer& meshBuffer, F32 length);
  void CreateCylinder(Graphics::Scene::MeshBuffer& meshBuffer, F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount);
  void CreateQuad(Graphics::Scene::MeshBuffer& meshBuffer, F32 width, F32 height);
  void CreateSphere(Graphics::Scene::MeshBuffer& meshBuffer, F32 radius, U32 sliceCount, U32 stackCount);
  void CreateTriangle(Graphics::Scene::MeshBuffer& meshBuffer, F32 length);
}
}
}
}

#endif
