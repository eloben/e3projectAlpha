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

// Created 18-Oct-2014 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ShadowComponent.h
This file declares the ShadowComponent class.
*/

#ifndef E3_SHADOW_COMPONENT_H
#define E3_SHADOW_COMPONENT_H

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ShadowComponent
----------------------------------------------------------------------------------------------------------------------*/
class ShadowComponent : public IShadowComponent
{
public:
  ShadowComponent();

  // Accessors
  ComponentType           GetComponentType() const;
  const IObjectInstance&  GetOwner() const;
  const Matrix4f&         GetViewProjectionMatrix() const;
  const ICameraInstance&  GetLightView() const;
  void                    SetOwner(const IObjectInstance& owner);
  void                    SetView(const IViewInstance& view);

  const Graphics::ITexture2DInstance& GetDepthTexture() const;
  // Callback methods
  void                    OnLoad();
  void                    OnUnload();
  void                    OnUpdate(const TimeValue& deltaTime);

private:
  IObjectInstance         mOwner;
  IViewInstance         mView;
  ICameraInstance         mLightCamera;
  //Graphics::IRenderTargetInstance      mLightContext;
  //Graphics::ITexture2DInstance    mDepthTexture;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ShadowComponent)
};

/*----------------------------------------------------------------------------------------------------------------------
ShadowComponent types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ShadowComponent> ShadowComponentInstance;
}
}
}

#endif
