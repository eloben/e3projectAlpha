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

// Created 04-Nov-2014 by Elías Lozada-Benavente
// Original 28-Sep-2012 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Frustum.h
This file contains the declaration of the Frustum class. Frustum class represents the camera viewable space defined by
6 planes. This space is called frustum and its used to discard non viewable geometry each frame, lowering the GPU 
geometry load.
*/

#ifndef E3_FRUSTUM_H
#define E3_FRUSTUM_H

#include <Math/Plane.h>
#include <Math/Matrix4.h>
#include <Math/Sphere.h>

namespace E 
{
namespace Graphics
{
class Frustum
{
public:
  enum Plane
  {
    ePlaneRight,
    ePlaneLeft,
    ePlaneTop,
    ePlaneBottom,
    ePlaneNear,
    ePlaneFar,
    ePlaneCount
  };

  enum Point
  {
    ePointNearTopLeft,
    ePointNearTopRight,
    ePointNearBottomLeft,
    ePointNearBottomRight,
    ePointFarTopLeft,
    ePointFarTopRight,
    ePointFarBottomLeft,
    ePointFarBottomRight,
    ePointCount
  };

  Frustum();

  // Accessors
  const Spheref&  GetBoundingSphere() const;
  const Matrix4f& GetInverseProjectionMatrix() const;
  const Matrix4f& GetViewProjectionMatrix() const;
  bool		        IsInside(const Vector3f& point, F32 radius = 0.0f) const;

  // Methods
  void		        Update(const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix);

private:
  Matrix4f	      mViewProjectionMatrix;
  Matrix4f	      mInverseProjectionMatrix;
  Planef		      mPlanes[ePlaneCount];
  Vector3f        mPoints[ePointCount];
  Spheref         mBoundingSphere;

  E_DISABLE_COPY_AND_ASSSIGNMENT(Frustum)
};
}
}

#endif

