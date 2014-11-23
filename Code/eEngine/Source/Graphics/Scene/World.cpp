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

// Created 04-Nov-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file World.cpp
This file defines the World class.
*/

#include <EnginePch.h>
#include "World.h"
#include "ScenePipeline.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
World assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_WORLD_OBJECT_LOADED              "Parameter object is already loaded"
#define E_ASSERT_MSG_WORLD_OBJECT_NOT_LOADED          "Parameter object is not loaded"

/*----------------------------------------------------------------------------------------------------------------------
World initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::World::World() {}

/*----------------------------------------------------------------------------------------------------------------------
World accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Scene::WorldState& Graphics::Scene::World::GetWorldState() const
{
  return mWorldState;
}

/*----------------------------------------------------------------------------------------------------------------------
World methods
----------------------------------------------------------------------------------------------------------------------*/
void Graphics::Scene::World::Load(const IObjectInstance& object)
{
  if (mWorldState.objectList[object->GetObjectType()].FindValue(object) == nullptr)
  {
    object->Load();
    mWorldState.objectList[object->GetObjectType()].PushBack(object);
  }
  else
  {
#ifdef E_DEBUG
    E_ASSERT_ALWAYS(E_ASSERT_MSG_WORLD_OBJECT_LOADED);
#endif
  }
}

void Graphics::Scene::World::Unload()
{
  for (U32 i = 0; i < IObject::eObjectTypeCount; ++i)
  {
    for (auto it = begin(mWorldState.objectList[i]); it != end(mWorldState.objectList[i]); ++it) (*it)->Unload();
    mWorldState.objectList[i].Clear();
  }
}

void Graphics::Scene::World::Unload(const IObjectInstance& object)
{
#ifdef E_DEBUG
  E_ASSERT_MSG(mWorldState.objectList[object->GetObjectType()].RemoveIfFast(object), E_ASSERT_MSG_WORLD_OBJECT_NOT_LOADED);
#else
  mWorldState.objectList[object->GetObjectType()].RemoveIfFast(object);
  object->Unload();
#endif
}

void Graphics::Scene::World::Update(const TimeValue& deltaTime)
{
  // Update world state
  for (U32 i = 0; i < IObject::eObjectTypeCount; ++i)
  {
    for (auto it = begin(mWorldState.objectList[i]); it != end(mWorldState.objectList[i]); ++it) (*it)->Update(deltaTime);
  }
}
