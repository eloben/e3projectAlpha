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

/** @file Frustum.cpp
This file contains the Frustum class methods definition.
*/

#include <EnginePch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Frustum auxiliary
----------------------------------------------------------------------------------------------------------------------*/

Vector3f kFrustumCornerPoints[Graphics::Frustum::ePointCount] =
{
  Vector3f(-1.0f, 1.0f, 0.0f),
  Vector3f(1.0f, 1.0f, 0.0f),
  Vector3f(-1.0f, -1.0f, 0.0f),
  Vector3f(1.0f, -1.0f, 0.0f),
  Vector3f(-1.0f, 1.0f, 1.0f),
  Vector3f(1.0f, 1.0f, 1.0f),
  Vector3f(-1.0f, -1.0f, 1.0f),
  Vector3f(1.0f, -1.0f, 1.0f)
};
/*
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
*/
/*----------------------------------------------------------------------------------------------------------------------
Frustum initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Frustum::Frustum() {}

/*----------------------------------------------------------------------------------------------------------------------
Frustum accessors
----------------------------------------------------------------------------------------------------------------------*/

const Spheref& Graphics::Frustum::GetBoundingSphere() const
{
  return mBoundingSphere;
}

const Matrix4f& Graphics::Frustum::GetViewProjectionMatrix() const
{
  return mViewProjectionMatrix;
}

const Matrix4f& Graphics::Frustum::GetInverseProjectionMatrix() const
{
  return mInverseProjectionMatrix;
}

bool Graphics::Frustum::IsInside(const Vector3f& point, F32 radius /* = 0.0f */) const
{
	// Check if the point is inside all 6 frustum planes
	for(U32 i = 0; i < 6; ++i) 
	{
		if (mPlanes[i].GetDistanceToPoint(point) < -radius)
			return false;
	}

	return true;
}

/*----------------------------------------------------------------------------------------------------------------------
Frustum methods

Implementation based on: Fast Extraction of Viewing Frustum Planes from the World-View-Projection Matrix (2001)
by Gil Gribb and Klaus Hartmann 
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Frustum::Update(const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix)
{
	// Compute view projection matrix
	mViewProjectionMatrix = viewMatrix * projectionMatrix;

	// Build planes
	mPlanes[ePlaneLeft] = Planef(
    mViewProjectionMatrix[ 3] + mViewProjectionMatrix[ 0],
    mViewProjectionMatrix[ 7] + mViewProjectionMatrix[ 4],
    mViewProjectionMatrix[11] + mViewProjectionMatrix[ 8],
    mViewProjectionMatrix[15] + mViewProjectionMatrix[12]);
	mPlanes[ePlaneLeft].Normalize();

	mPlanes[ePlaneRight] = Planef(
    mViewProjectionMatrix[ 3] - mViewProjectionMatrix[ 0],
		mViewProjectionMatrix[ 7] - mViewProjectionMatrix[ 4],
		mViewProjectionMatrix[11] - mViewProjectionMatrix[ 8],
		mViewProjectionMatrix[15] - mViewProjectionMatrix[12]);
	mPlanes[ePlaneRight].Normalize();

	mPlanes[ePlaneTop] = Planef(
    mViewProjectionMatrix[ 3] - mViewProjectionMatrix[ 1],
		mViewProjectionMatrix[ 7] - mViewProjectionMatrix[ 5],
		mViewProjectionMatrix[11] - mViewProjectionMatrix[ 9],
		mViewProjectionMatrix[15] - mViewProjectionMatrix[13]);
	mPlanes[ePlaneTop].Normalize();

	mPlanes[ePlaneBottom] = Planef(
    mViewProjectionMatrix[ 3] + mViewProjectionMatrix[ 1],
		mViewProjectionMatrix[ 7] + mViewProjectionMatrix[ 5],
		mViewProjectionMatrix[11] + mViewProjectionMatrix[ 9],
		mViewProjectionMatrix[15] + mViewProjectionMatrix[13]);
	mPlanes[ePlaneBottom].Normalize();

	mPlanes[ePlaneNear] = Planef(
    mViewProjectionMatrix[ 2],
    mViewProjectionMatrix[ 6],
    mViewProjectionMatrix[10],
    mViewProjectionMatrix[14]);
	mPlanes[ePlaneNear].Normalize();

	mPlanes[ePlaneFar] = Planef(
    mViewProjectionMatrix[ 3] - mViewProjectionMatrix[ 2],
    mViewProjectionMatrix[ 7] - mViewProjectionMatrix[ 6],
    mViewProjectionMatrix[11] - mViewProjectionMatrix[10],
    mViewProjectionMatrix[15] - mViewProjectionMatrix[14]);
	mPlanes[ePlaneFar].Normalize();

  // Compute inverse
  mInverseProjectionMatrix = Matrix4f::Invert(mViewProjectionMatrix);
  // Compute frustum points
  for (U32 i = 0; i < ePointCount; ++i) mPoints[i] =  Matrix4f::TransformPoint(mInverseProjectionMatrix, kFrustumCornerPoints[i]);
  // Compute frustum bounding sphere
  mBoundingSphere.SetPoints(mPoints, ePointCount);
}
