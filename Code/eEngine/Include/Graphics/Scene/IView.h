/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 El�as Lozada-Benavente

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

// Created 15-Sep-2014 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IView.h
This file defines the IView interface.
*/

#ifndef E3_IVIEW_H
#define E3_IVIEW_H

#include <Graphics/Scene/ICamera.h>
#include <Graphics/IRenderTarget.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ViewState
----------------------------------------------------------------------------------------------------------------------*/
struct ViewState
{  
  ICameraInstance       camera;
  IRenderTargetInstance frameBuffer;
  IViewportInstance     viewport;

  void Clear()
  {
    camera = nullptr;
    frameBuffer = nullptr;
    viewport = nullptr;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
IView
----------------------------------------------------------------------------------------------------------------------*/
class IView
{
public:
  virtual                   ~IView() {}

  // Accessors
  virtual const ViewState&  GetViewState() const = 0;
  virtual void              SetCamera(const ICameraInstance& camera) = 0;

  // Methods
  virtual void              Update(const TimeValue& deltaTime) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IView types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IView> IViewInstance;
}
}
}

#endif
