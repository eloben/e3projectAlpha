/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

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
------------------------------------------------------------------------------------------------------------------------*/

// Created 06-Nov-2014 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Random.cpp
This file defines the library math random global accessor methods.
*/

#include <CorePch.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Math::Global methods

Please note that these methods must be defined in a source file instead of inline to guarantee a unique global allocator
 across DLLs as static variables are local to the compilation unit.
----------------------------------------------------------------------------------------------------------------------*/
RandomNumberGenerator& Global::GetRandom()
{
  return Singleton<RandomNumberGenerator>::GetInstance();
}
}
}
