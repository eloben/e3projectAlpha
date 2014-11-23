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

// Created 04-Nov-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IWorld.h
This file defines the IWorld interface.
*/

#ifndef E3_IWORLD_H
#define E3_IWORLD_H

#include <Graphics/Scene/IObject.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
WorldState
----------------------------------------------------------------------------------------------------------------------*/
struct WorldState
{  
  IObjectInstanceList objectList[IObject::eObjectTypeCount];
};

/*----------------------------------------------------------------------------------------------------------------------
IWorld
----------------------------------------------------------------------------------------------------------------------*/
class IWorld
{
public:
  virtual                   ~IWorld() {}

  // Accessors
  virtual const WorldState& GetWorldState() const = 0;

  // Methods
  virtual void	            Load(const IObjectInstance& object) = 0;
  virtual void              Unload() = 0;
  virtual void	            Unload(const IObjectInstance& object) = 0;
  virtual void              Update(const TimeValue& deltaTime) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IWorld types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IWorld> IWorldInstance;
}
}
}

#endif
