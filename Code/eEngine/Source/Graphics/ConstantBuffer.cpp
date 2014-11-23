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

// Created 20-Aug-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ConstantBuffer.cpp
This file defines the ConstantBuffer class.
*/

#include <EnginePch.h>
#include "ConstantBuffer.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
ConstantBuffer assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_CONSTANT_ARRAY_REGISTER_SLOT_VALUE "Slot value (%d) must be a value between 0 and 3"
#define E_ASSERT_MSG_CONSTANT_ARRAY_REGISTER_SIZE       "Parameter value starting in slot (%d) exceeds register size"

/*----------------------------------------------------------------------------------------------------------------------
ConstantBuffer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::ConstantBuffer::ConstantBuffer() {}

void Graphics::ConstantBuffer::Initialize(IDeviceInstance deviceManager)
{
  E_ASSERT_PTR(deviceManager);
  // Create constant buffer
  Graphics::IBuffer::Descriptor constantBufferDescriptor;
  constantBufferDescriptor.type = Graphics::IBuffer::eTypeConstant;
  constantBufferDescriptor.accessFlags = Graphics::IBuffer::eAccessFlagGpuRead | Graphics::IBuffer::eAccessFlagCpuWrite;
  constantBufferDescriptor.elementSize = sizeof(F32);
  mBuffer = deviceManager->CreateBuffer(constantBufferDescriptor);
  E_ASSERT_PTR(mBuffer);
}

/*----------------------------------------------------------------------------------------------------------------------
ConstantBuffer accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IBufferInstance& Graphics::ConstantBuffer::GetBuffer() const
{ 
  return mBuffer;
}

void Graphics::ConstantBuffer::Set(U32 registerID, const Matrix4f& value)
{
  mBuffer->Set(&value[0], 16, registerID * 4);
}

void Graphics::ConstantBuffer::Set(U32 registerID, const Vector4f& value)
{
  mBuffer->Set(&value[0], 4, registerID * 4);
}

void Graphics::ConstantBuffer::Set(U32 registerID, const Vector3f& value, U32 slot)
{
  E_ASSERT_MSG(slot < 2, E_ASSERT_MSG_CONSTANT_ARRAY_REGISTER_SIZE, slot);
  U32 valueIndex = registerID * 4 + slot;
  // If the register is allocated (alignment 4-32-bit is required)
  if (valueIndex < mBuffer->GetCount())
  {
    mBuffer->Set(&value[0], 3, valueIndex);
  }
  // Set the whole register
  else
  {
    Vector4f registerValue;
    registerValue[slot] = value.x;
    registerValue[slot + 1] = value.y;
    registerValue[slot + 2] = value.z;
    Set(registerID, registerValue);
  }
}

void Graphics::ConstantBuffer::Set(U32 registerID, const Vector2f& value, U32 slot)
{
  E_ASSERT_MSG(slot < 3, E_ASSERT_MSG_CONSTANT_ARRAY_REGISTER_SIZE, slot);
  U32 valueIndex = registerID * 4 + slot;
  // If the register is allocated (alignment 4-32-bit is required)
  if (valueIndex < mBuffer->GetCount())
  {
    mBuffer->Set(&value[0], 2, valueIndex);
  }
  // Set the whole register
  else
  {
    Vector4f registerValue;
    registerValue[slot] = value.x;
    registerValue[slot + 1] = value.y;
    Set(registerID, registerValue);
  }
}

void Graphics::ConstantBuffer::Set(U32 registerID, F32 value, U32 slot)
{
  E_ASSERT_MSG(slot < 4, E_ASSERT_MSG_CONSTANT_ARRAY_REGISTER_SLOT_VALUE);
  U32 valueIndex = registerID * 4 + slot;
  // If the register is allocated (alignment 4-32-bit is required)
  if (valueIndex < mBuffer->GetCount())
  {
    mBuffer->Set(&value, 1, valueIndex);
  }
  // Set the whole register
  else
  {
    Vector4f registerValue;
    registerValue[slot] = value;
    Set(registerID, registerValue);
  }
}

void Graphics::ConstantBuffer::Set(U32 registerID, F32 slot0Value, F32 slot1Value, F32 slot2Value, F32 slot3Value)
{
  Set(registerID, Vector4f(slot0Value, slot1Value, slot2Value, slot3Value));
}
