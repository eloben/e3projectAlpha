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

// Created 09-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IResourceBuffer.h
This file defines the IResourceBuffer interface. IResourceBuffer allows indexed resource specification.
*/

#ifndef E3_IRESOURCE_BUFFER_H
#define E3_IRESOURCE_BUFFER_H

#include <Graphics/IBuffer.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IResourceBuffer
----------------------------------------------------------------------------------------------------------------------*/
class IResourceBuffer
{
public:
  virtual                         ~IResourceBuffer() {}

  // Accessors
  virtual const IBufferInstance&  GetBuffer() const = 0;
  virtual void                    Set(U32 index, const void* pInstanceData) = 0;

  // Methods
  virtual U32                     AcquireIndex() = 0;
  virtual void                    ReleaseIndex(U32 index) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IRenderTarget types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IResourceBuffer> IResourceBufferInstance;
}
}

#endif
