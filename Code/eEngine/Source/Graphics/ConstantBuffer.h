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

// Created 16-Aug-2014 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ConstantBuffer.h
This file defines the ConstantBuffer class.
*/

#ifndef E3_CONSTANT_ARRAY_H
#define E3_CONSTANT_ARRAY_H

#include <EnginePch.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
ConstantBuffer
----------------------------------------------------------------------------------------------------------------------*/
class ConstantBuffer : public IConstantBuffer
{
public:
                          ConstantBuffer();
  void                    Initialize(IDeviceInstance deviceManager);

  // Accessors
  const IBufferInstance&  GetBuffer() const;
  void                    Set(U32 registerID, const Matrix4f& value);
  void                    Set(U32 registerID, const Vector4f& value);
  void                    Set(U32 registerID, const Vector3f& value, U32 slot);
  void                    Set(U32 registerID, const Vector2f& value, U32 slot);
  void                    Set(U32 registerID, F32 value, U32 slot);
  void                    Set(U32 registerID, F32 slot0Value, F32 slot1Value, F32 slot2Value, F32 slot3Value);

private:
  IBufferInstance         mBuffer;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ConstantBuffer)
};

/*----------------------------------------------------------------------------------------------------------------------
ConstantBuffer types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ConstantBuffer> ConstantBufferInstance;
}
}

#endif
