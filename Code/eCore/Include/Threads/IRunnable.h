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

// Created 05-Feb-2010 by El�as Lozada-Benavente
// Based on original by �ngel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Thread.h
This file defines the Thread interface class. IRunnable interface is intended to be used as a base 
class to define an actual operation that needs to be carried out in a separate thread. IRunnable 
objects are passed to Thread instances on initialization, and will be executed on a new thread.
*/

#ifndef E3_IRUNNABLE_H
#define E3_IRUNNABLE_H

#include <Base.h>

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
IRunnable
----------------------------------------------------------------------------------------------------------------------*/	
class IRunnable
{
public:
  virtual     ~IRunnable() {}
  virtual I32 Run() = 0;
};
}
}

#endif
