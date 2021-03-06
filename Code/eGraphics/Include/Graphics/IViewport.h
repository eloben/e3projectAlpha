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

// Created 20-Apr-2014 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IViewport.h
This file defines the IViewport interface. The IViewport represents a 2D canvas for 3D rendering.
*/

#ifndef E3_IVIEWPORT_H
#define E3_IVIEWPORT_H

#include <Memory/GarbageCollection.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IViewport
----------------------------------------------------------------------------------------------------------------------*/
class IViewport
{
public:
  enum ViewportFlag
  { 
    eViewportFlagNone, 
    eViewportFlagEnableVerticalSync = 1, 
    eViewportFlagEnableFullScreen   = 2
  }; 

  struct Descriptor 
  {   
    Ptr windowHandle;
    U32 width; 
    U32 height; 
    U8  flags;     

    Descriptor::Descriptor() 
      : windowHandle(0) 
      , width(0) 
      , height(0) 
      , flags(eViewportFlagNone) 
    {}
  };

  virtual                   ~IViewport() {}
  virtual const Descriptor& GetDescriptor() const = 0;
  virtual bool              Update() = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IViewport types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IViewport> IViewportInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::IViewport::Descriptor)

#endif
