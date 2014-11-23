/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

Copyright (c) 2010-2014 El�as Lozada-Benavente

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

// Created 13-Aug-2014 by El�as Lozada-Benavente
// Original 01-Dec-2011 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Camera.h
This file declares the Camera class.
*/

#ifndef E3_CAMERA_H
#define E3_CAMERA_H

#include "ObjectCore.h"

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
Camera
----------------------------------------------------------------------------------------------------------------------*/
class Camera : public ICamera
{
public:
  Camera();

  E_GRAPHICS_DEFINE_SCENE_OBJECT_COMMON(eObjectTypeCamera, mCore)

  // Accessors
  F32				                        GetAspectRatio() const;
  const Vector3f&                   GetAxisX() const;
  const Vector3f&                   GetAxisY() const;
  const Vector3f&                   GetAxisZ() const;
  F32				                        GetFar() const;
  U32				                        GetFov() const;
  const Frustum&                    GetFrustum() const;
  F32				                        GetNear() const;
  const Matrix4f&                   GetProjectionMatrix() const;
  ProjectionType                    GetProjectionType() const;
  const Matrix4f&                   GetViewMatrix() const;
  const Matrix4f&                   GetViewProjectionMatrix() const;
  void			                        SetAxisX(const Vector3f& v);
  void			                        SetAxisY(const Vector3f& v);
  void			                        SetAxisZ(const Vector3f& v);
  void			                        SetFov(U32 v);
  void			                        SetNearFar(F32 nearPlane, F32 farPlane);
  void			                        SetProjectionType(ProjectionType type);
  void                              SetViewportDimensions(U32 width, U32 height);

  // Methods
  void                              Clear();
  void                              Load();
  void                              Render();
  void                              Unload();
  void                              Update(const TimeValue& deltaTime);

private:
  ObjectCore                        mCore;
  IRenderManagerInstance            mRenderManager;
  IConstantBufferInstance           mInterFrameConstantBuffer;
  Frustum                           mFrustum;
  Matrix4f				                  mProjectionMatrix;
  Matrix4f				                  mViewMatrix;
  Vector3f		                      mRight;
  Vector3f		                      mLook;
  Vector3f		                      mUp;
  F32                               mNearPlane;
  F32                               mFarPlane;
  F32                               mAspectRatio;
  U32                               mFov;
  U32                               mViewportWidth;
  U32                               mViewportHeight;
  ProjectionType                    mProjectionType;

  void					                    BuildLookAt(const Vector3f& targetWorldPosition);
  void					                    BuildProjectionMatrix();
  void					                    BuildViewMatrix();
  void                              UpdateViewMatrix();
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(Camera)
}; 
}
}
}

#endif
