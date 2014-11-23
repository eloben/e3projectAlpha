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

// Created 01-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Light.h
This file declares the Light class.
*/

#ifndef E3_DIRECT_LIGHT_H
#define E3_DIRECT_LIGHT_H

#include "ObjectCore.h"
#include "ShadowComponent.h"

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
Light
----------------------------------------------------------------------------------------------------------------------*/
class Light : public ILight
{
public:
  struct ShaderProperties
  {
    Vector3f        direction;
    Vector3f        color;
  };

  Light();

  E_GRAPHICS_DEFINE_SCENE_OBJECT_COMMON(eObjectTypeLight, mCore)

  // Accessors
  const Graphics::Color&            GetColor() const;
  const Vector3f&                   GetDirection() const;
  void                              SetColor(const Graphics::Color& color);

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
  U32                               mLightID;
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(Light)
}; 
}
}
}

#endif
