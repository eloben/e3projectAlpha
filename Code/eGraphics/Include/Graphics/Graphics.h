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

// Created 18-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Graphics.h
This file declares the Graphics library global accessor methods.
*/

#ifndef E3_GRAPHICS_H
#define E3_GRAPHICS_H

#include <Graphics/IDevice.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
Graphics::Global API methods

Please note that these methods have the following usage contract: 

1. This methods exists to allow retrieving / modifying the library global allocator. This is specially important 
due to the fact that the Graphics library is an independent library providing garbage collected memory managemnet.
This means that when used by third-party dynamic libraries (DLL) in combination with a MTd or MT linkage of the
C Runtime library, each DLL although sharing the same memory space will have its own heap. This characteristic may
incur in a GCRef pointer whose owner has been allocated by Graphics to try to be deallocated using the global memory
allocator of the third-party library to deallocator the GCRef counter. This behavior will trigger in debug an 
_CrtIsValidHeapPointer assertion. In order to fix this issue either:

1. Compile the solution using an MDd / MD  (Multi-Threaded DLL) linkage for the C Runtime library.
2. Set the third-party global allocator to use the Graphics global allocator:

  Memory::Global::GetAllocatorProvider().SetAllocator(Graphics::Global::GetAllocator());
----------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
  E_API Memory::IAllocator*   GetAllocator();
  E_API void                  SetAllocator(Memory::IAllocator* pAllocator);
}
}
}

#endif
