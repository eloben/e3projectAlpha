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

// Created 18-Sep-2014 by Elías Lozada-Benavente
// Original created 15-Nov-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ILogicComponent.h
This file defines the ILogicComponent interface. ILogicComponent allows adding external logic to an object.
*/

#ifndef E3_ILOGIC_COMPONENT_H
#define E3_ILOGIC_COMPONENT_H

#include <Graphics/Scene/IObject.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ILogicHandler
----------------------------------------------------------------------------------------------------------------------*/
class ILogicHandler
{
public:
  virtual                 ~ILogicHandler() {}

  // Callback methods
  virtual void            OnLoad() = 0;
  virtual void            OnUnload() = 0;
  virtual void            OnUpdate(const TimeValue& deltaTime) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ILogicHandler types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ILogicHandler> ILogicHandlerInstance;

/*----------------------------------------------------------------------------------------------------------------------
ILogicComponent
----------------------------------------------------------------------------------------------------------------------*/
class ILogicComponent : public IObjectComponent
{
public:
  virtual ILogicHandlerInstance  GetHandler() const = 0;
  virtual void                   SetHandler(ILogicHandlerInstance handler) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ILogicComponent types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ILogicComponent> ILogicComponentInstance;
}
}
}

#endif
