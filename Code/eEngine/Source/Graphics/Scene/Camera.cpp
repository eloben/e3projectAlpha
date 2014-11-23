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

// Created 13-Aug-2014 by Elías Lozada-Benavente
// Original 01-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Camera.cpp
This file defines the Camera class.
*/

#include <EnginePch.h>
#include "Camera.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary
----------------------------------------------------------------------------------------------------------------------*/

const F32 kDefaultNearPlane = 1.0f;
const F32 kDefaultFarPlane = 1000.0f;
const U32 kDefaultFov = 90;

/*----------------------------------------------------------------------------------------------------------------------
Camera initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

// Known warning: passing this in the initializer list to mCore
#pragma warning(push)
#pragma warning (disable:4355)
Graphics::Scene::Camera::Camera()
  : mCore(this)
  , mRenderManager(Graphics::Global::GetRenderManager())
  , mInterFrameConstantBuffer(mRenderManager->GetConstantBuffer(eConstantBufferIDInterFrame))
  , mNearPlane(kDefaultNearPlane)
  , mFarPlane(kDefaultFarPlane)
  , mAspectRatio(0)
  , mFov(kDefaultFov)
  , mViewportWidth(0)
  , mViewportHeight(0)
  , mProjectionType(eProjectionTypePerspective) {}
#pragma warning(pop)

/*----------------------------------------------------------------------------------------------------------------------
Camera accessors
----------------------------------------------------------------------------------------------------------------------*/

const Vector3f& Graphics::Scene::Camera::GetAxisX() const
{
  return mRight;
}

const Vector3f& Graphics::Scene::Camera::GetAxisY() const
{
  return mUp;
}

const Vector3f& Graphics::Scene::Camera::GetAxisZ() const
{
  return mLook;
}

F32 Graphics::Scene::Camera::GetAspectRatio() const
{
  return mAspectRatio;
}

F32 Graphics::Scene::Camera::GetFar() const
{
  return mFarPlane;
}

U32 Graphics::Scene::Camera::GetFov() const
{
  return mFov;
}

const Graphics::Frustum& Graphics::Scene::Camera::GetFrustum() const
{
  return mFrustum;
}

F32 Graphics::Scene::Camera::GetNear() const
{
  return mNearPlane;
}

Graphics::Scene::ICamera::ProjectionType Graphics::Scene::Camera::GetProjectionType() const
{
  return mProjectionType;
}

const Matrix4f& Graphics::Scene::Camera::GetProjectionMatrix() const
{
  return mProjectionMatrix;
}

const Matrix4f& Graphics::Scene::Camera::GetViewMatrix() const
{
  return mViewMatrix;
}

const Matrix4f& Graphics::Scene::Camera::GetViewProjectionMatrix() const
{
  return mFrustum.GetViewProjectionMatrix();
}

void Graphics::Scene::Camera::SetAxisX(const Vector3f& v)
{
  mRight = v;
}

void Graphics::Scene::Camera::SetAxisY(const Vector3f& v)
{
  mUp = v;
}

void Graphics::Scene::Camera::SetAxisZ(const Vector3f& v)
{
  mLook = v;
}

void Graphics::Scene::Camera::SetFov(U32 v)
{
  mFov = v;
}

void Graphics::Scene::Camera::SetNearFar(F32 nearPlane, F32 farPlane)
{
  mNearPlane = nearPlane;
  mFarPlane = farPlane;
}

void Graphics::Scene::Camera::SetProjectionType(ProjectionType type)
{
  mProjectionType = type;
}

void Graphics::Scene::Camera::SetViewportDimensions(U32 width, U32 height)
{
  mAspectRatio = static_cast<F32>(width) / height;
  mViewportWidth = width;
  mViewportHeight = height;
  BuildProjectionMatrix();
  mFrustum.Update(mViewMatrix, mProjectionMatrix);
}

/*----------------------------------------------------------------------------------------------------------------------
Camera methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::Camera::Clear()
{
  mCore.ClearTransform();
  mNearPlane = kDefaultNearPlane;
  mFarPlane = kDefaultFarPlane;
  mAspectRatio = 0;
  mFov = kDefaultFov;
  mViewportWidth = 0;
  mViewportHeight = 0;
  mProjectionType = eProjectionTypePerspective;
}

void Graphics::Scene::Camera::Load() {}

void Graphics::Scene::Camera::Unload() {}

void Graphics::Scene::Camera::Render()
{
  // Update inter frame constant buffer with camera matrices
  mInterFrameConstantBuffer = mRenderManager->GetConstantBuffer(eConstantBufferIDInterFrame);
  mInterFrameConstantBuffer->Set(0, mFrustum.GetViewProjectionMatrix());
  mInterFrameConstantBuffer->Set(4, mProjectionMatrix);
  mInterFrameConstantBuffer->Set(8, mViewMatrix);
  mInterFrameConstantBuffer->Set(12, mCore.GetWorldMatrix().GetTranslation(), 0);
  mRenderManager->Update(mInterFrameConstantBuffer);

  // Render children
  mCore.RenderChildren();
}

void Graphics::Scene::Camera::Update(const TimeValue& deltaTime)
{
  // Build view matrix
  BuildViewMatrix();
  // Update view frustum
  mFrustum.Update(mViewMatrix, mProjectionMatrix);

  mCore.Update(deltaTime);
}

/*----------------------------------------------------------------------------------------------------------------------
Camera private methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::Camera::BuildLookAt(const Vector3f& targetWorldPosition)
{
  // Generate axis
	Vector3f mUp = Vector3f::AxisY();
	Vector3f mLook = targetWorldPosition - mCore.GetWorldMatrix().GetTranslation();
	mLook.Normalize();
	Vector3f mRight = Vector3f::Cross(mUp, mLook);
	mRight.Normalize();
	mUp = Vector3f::Cross(mLook, mRight);

  UpdateViewMatrix();
}

void Graphics::Scene::Camera::BuildProjectionMatrix()
{
  mProjectionMatrix = (mProjectionType == eProjectionTypePerspective) ? 
    Math::BuildPerspectiveLH(mFov, mAspectRatio, mNearPlane, mFarPlane) :
    Math::BuildOrthographicLH(mViewportWidth, mViewportHeight, mNearPlane, mFarPlane);
}

void Graphics::Scene::Camera::BuildViewMatrix()
{
	// Compute axis vectors
  const Matrix4f& worldMatrix = mCore.GetWorldMatrix();
  mRight = worldMatrix.GetVectorRight();
  mUp = worldMatrix.GetVectorUp();
  mLook = worldMatrix.GetVectorForward();

  /*
	// Normalize axis
	mLook.Normalize();
	mRight = Vector3f::Cross(mUp, mLook);
	mRight.Normalize();
	mUp = Vector3f::Cross(mLook, mRight);
	mUp.Normalize();
	*/
	UpdateViewMatrix();
}

void Graphics::Scene::Camera::UpdateViewMatrix()
{
  /*
  The view matrix is just the inverse of the world matrix: to simulate a camera moving and rotating in the world
  we transform the world by the inverse of the camera world matrix, moving and rotating the world in the exact 
  opposed direction and angles respectively.
  To compute easily the inverse we take into account that rotation matrices are orthogonal: rotation inverse is its transposed.
  Then we compute the inverse translation by multiplying each coordinate by the orthonormal axis vector and negate the result:

  Rx        Ux      Fx        0
  Ry        Uy      Fy        0
  Rz        Uz      Fz        0
  -Tx * R  -Ty * U  -Tz * F   1
  */
  
  mViewMatrix[ 0] = mRight.x;
  mViewMatrix[ 4] = mRight.y;
  mViewMatrix[ 8] = mRight.z;
  mViewMatrix[ 1] = mUp.x;
  mViewMatrix[ 5] = mUp.y;
  mViewMatrix[ 9] = mUp.z;
  mViewMatrix[ 2] = mLook.x;
  mViewMatrix[ 6] = mLook.y;
  mViewMatrix[10] = mLook.z;
  mViewMatrix[ 3] = 0.0f;
  mViewMatrix[ 7] = 0.0f;
  mViewMatrix[11] = 0.0f;
  const Vector3f& worldPosition = mCore.GetWorldMatrix().GetTranslation();
  mViewMatrix[12] = -Vector3f::Dot(worldPosition, mRight);
  mViewMatrix[13] = -Vector3f::Dot(worldPosition, mUp);
  mViewMatrix[14] = -Vector3f::Dot(worldPosition, mLook);
  mViewMatrix[15] = 1.0f;
  //E_DEBUG_MATRIX4(mViewMatrix);
}
