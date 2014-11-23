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

// Created 16-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ILightSpot.h
This file declares the ILightSpot class.
*/

#ifndef E3_SPOT_LIGHT_H
#define E3_SPOT_LIGHT_H

#include "ObjectCore.h"
#include "ShadowComponent.h"

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
LightSpot
----------------------------------------------------------------------------------------------------------------------*/
class LightSpot : public ILightSpot
{
public:
  struct ShaderProperties
  {
    Vector3f        direction;
    Vector3f        color;
    Vector3f        position;
    Vector4f        attenuation;
    float           cosCutOffAngle;
    Matrix4f        viewProjectionMatrix;
  };

  LightSpot();

  E_GRAPHICS_DEFINE_SCENE_OBJECT_COMMON(eObjectTypeLightSpot, mCore)

  // Accessors
  const Graphics::Color&            GetColor() const;
  void                              SetAttenuation(F32 a, F32 b, F32 c);
  void                              SetColor(const Graphics::Color& color);
  void                              SetCutOffAngle(F32 angle);
  void                              SetRange(F32 range);

  void                              Clear();
  void                              Load();
  void                              Render();
  void                              Unload();
  void                              Update(const TimeValue& deltaTime);

private:
  ObjectCore                        mCore;
  Graphics::IRenderManagerInstance  mRenderManager;
  IConstantBufferInstance           mIntraFrameConstantBuffer;
  ShaderProperties                  mShaderProperties;
  ShadowComponentInstance           mShadowComponent;
  Graphics::Color                   mColor;
  Vector3f                          mDirection;
  Vector4f                          mAttenuation;
  F32                               mCutOffAngle;
  F32                               mRange;
  U32                               mLightID;
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(LightSpot)
}; 
}
}
}

#endif
