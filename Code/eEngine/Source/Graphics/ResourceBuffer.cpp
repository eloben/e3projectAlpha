/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 RenderManager

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

/** @file ResourceBuffer.cpp
This file defines the ResourceBuffer class.
*/

#include <EnginePch.h>
#include "ResourceBuffer.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_INDEXED_RESOURCE_BUFFER_MAX_INDEX_COUNT "Indexed resource buffer index count cannot exceed %d"

/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer auxiliary
----------------------------------------------------------------------------------------------------------------------*/
static const U32 kMaxIndexCount = 32 * 1024 * 1024; // 128 MB / 4 bytes required by F32 (minimum size resource data)

/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::ResourceBuffer::ResourceBuffer()
  : mIndexCount(0) {}

void Graphics::ResourceBuffer::Initialize(size_t elementSize, IDeviceInstance deviceManager)
{
  E_ASSERT_PTR(deviceManager);
  // Create constant buffer
  Graphics::IBuffer::Descriptor resourceBufferDescriptor;
  resourceBufferDescriptor.type = Graphics::IBuffer::eTypeResource;
  resourceBufferDescriptor.accessFlags = Graphics::IBuffer::eAccessFlagGpuRead | Graphics::IBuffer::eAccessFlagCpuWrite;
  resourceBufferDescriptor.elementSize = elementSize;
  mBuffer = deviceManager->CreateBuffer(resourceBufferDescriptor);
  E_ASSERT_PTR(mBuffer);
}

/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer accessors
----------------------------------------------------------------------------------------------------------------------*/


const Graphics::IBufferInstance& Graphics::ResourceBuffer::GetBuffer() const
{ 
  return mBuffer;
}

void Graphics::ResourceBuffer::Set(U32 index, const void* pInstanceData)
{
  mBuffer->Set(pInstanceData, 1, index);
}

/*----------------------------------------------------------------------------------------------------------------------
ResourceBuffer methods
----------------------------------------------------------------------------------------------------------------------*/

U32 Graphics::ResourceBuffer::AcquireIndex()
{
  E_ASSERT_MSG(mIndexCount < kMaxIndexCount, E_ASSERT_MSG_INDEXED_RESOURCE_BUFFER_MAX_INDEX_COUNT, kMaxIndexCount);
  if (mFreeIndexQueue.IsEmpty()) return mIndexCount++;
  U32 index = mFreeIndexQueue.GetFront();
  mFreeIndexQueue.Pop();

  return index;
}

void Graphics::ResourceBuffer::ReleaseIndex(U32 index)
{
  mFreeIndexQueue.Push(index);
}
