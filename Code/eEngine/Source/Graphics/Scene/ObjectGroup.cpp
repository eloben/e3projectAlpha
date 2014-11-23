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

// Created 14-Sep-2014 by Elías Lozada-Benavente
// Original 15-Nov-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ObjectGroup.cpp
This file defines the ObjectGroup class.
*/

#include <EnginePch.h>
#include "ObjectGroup.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
ObjectGroup assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_SCENE_OBJECT_GROUP_OBJECT_MISSING               "Argument object does not belong to this scene"

/*----------------------------------------------------------------------------------------------------------------------
ObjectGroup initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
ObjectGroup accessors
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
ObjectGroup methods
------------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::ObjectGroup::AddObject(const IObjectInstance& object)
{
  mObjectList.PushBack(object);
}

void Graphics::Scene::ObjectGroup::RemoveObject(const IObjectInstance& object)
{
#ifdef E_DEBUG
  E_ASSERT_MSG(mObjectList.RemoveIfFast(object), E_ASSERT_MSG_SCENE_OBJECT_GROUP_OBJECT_MISSING);
#else
  mObjectList.RemoveIfFast(object);
#endif
}
