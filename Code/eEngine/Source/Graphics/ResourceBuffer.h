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

// Created 09-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ResourceBuffer.h
This file defines the ResourceBuffer class.
*/

#ifndef E3_INDEXED_RESOURCE_BUFFER_H
#define E3_INDEXED_RESOURCE_BUFFER_H

#include <EnginePch.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer
----------------------------------------------------------------------------------------------------------------------*/
class ResourceBuffer : public IResourceBuffer
{
public:
                          ResourceBuffer();
  void                    Initialize(size_t elementSize, IDeviceInstance deviceManager);

  // Accessors
  const IBufferInstance&  GetBuffer() const;
  void                    Set(U32 index, const void* pInstanceData);

  // Methods
  U32                     AcquireIndex();
  void                    ReleaseIndex(U32 index);

private:
  Containers::Queue<U32>  mFreeIndexQueue;
  IBufferInstance         mBuffer;
  U32                     mIndexCount;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ResourceBuffer)
};

/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ResourceBuffer> ResourceBufferInstance;
}
}

#endif
