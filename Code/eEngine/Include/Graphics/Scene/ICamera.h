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
// Original created 02-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ICamera.h
This file defines the ICamera interface.
*/

#ifndef E3_ICAMERA_H
#define E3_ICAMERA_H

#include <Graphics/Frustum.h>
#include <Graphics/Scene/IObject.h>
#include <Math/Matrix4.h>
#include <Math/Vector3.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ICamera
----------------------------------------------------------------------------------------------------------------------*/
class ICamera : public IObject
{
public:
  enum ProjectionType
  {
    eProjectionTypeOrthographic,
    eProjectionTypePerspective
  };

  virtual                 ~ICamera() {}

  // Accessors
  virtual F32				      GetAspectRatio() const = 0;
  virtual const Vector3f& GetAxisX() const = 0;
  virtual const Vector3f& GetAxisY() const = 0;
  virtual const Vector3f& GetAxisZ() const = 0;
  virtual F32				      GetFar() const = 0;
  virtual U32				      GetFov() const = 0;
  virtual const Frustum&  GetFrustum() const = 0;
  virtual F32				      GetNear() const = 0;
  virtual ProjectionType  GetProjectionType() const = 0;
  virtual const Matrix4f& GetProjectionMatrix() const = 0;
  virtual const Matrix4f& GetViewMatrix() const = 0;
  virtual const Matrix4f& GetViewProjectionMatrix() const = 0;
  virtual void			      SetFov(U32 v) = 0;
  virtual void			      SetNearFar(F32 nearPlane, F32 farPlane) = 0;
  virtual void			      SetProjectionType(ProjectionType type) = 0;
  virtual void			      SetViewportDimensions(U32 width, U32 height) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ICamera types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ICamera> ICameraInstance;
}
}
}

#endif
