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

// Created 08-Jun-2014 by Elías Lozada-Benavente
// Original 15-Nov-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ObjectCore.cpp
This file defines the ObjectCore class.
*/

#include <EnginePch.h>
#include "ObjectCore.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
ObjectCore assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_SCENE_OBJECT_CORE_COMPONENT_MISSING  "Parameter component is not contained"
#define E_ASSERT_MSG_SCENE_OBJECT_CORE_COMPONENT_UNIQUE   "There can only be one component of a concrete type"
#define E_ASSERT_MSG_SCENE_OBJECT_CORE_CHILD_INVALID      "Parameter object is not a child"
#define E_ASSERT_MSG_SCENE_OBJECT_CORE_MANUAL_PARENT_SET  "Attempt to manually set parent: use AddChild on parent instead."

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary
----------------------------------------------------------------------------------------------------------------------*/
const Graphics::Scene::IObjectComponentInstance kNullComponent;
const Graphics::Scene::IObjectInstance          kNullObject;

void ClampOrientationAngles(Vector3f& v);

/*----------------------------------------------------------------------------------------------------------------------
ObjectCore initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::ObjectCore::ObjectCore(IObject* pOwner)
: //mRenderCommand()
 mScale(1.0f, 1.0f, 1.0f)
, mOwner(pOwner)
{
//  mRenderCommand.pipelineState = Graphics::Global::GetRenderManager()->GetDefaultRenderState();
}

/*----------------------------------------------------------------------------------------------------------------------
ObjectCore accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Scene::IObjectInstanceList& Graphics::Scene::ObjectCore::GetChildrenList() const
{
  return mChildrenList;
}

const Graphics::Scene::IObjectComponentInstance& Graphics::Scene::ObjectCore::GetComponent(IObjectComponent::ComponentType type) const
{
  for (auto it = begin(mComponentList); it != end(mComponentList); ++it)
  {
    if ((*it)->GetComponentType() == type) return *it;
  }
  return kNullComponent;
}

const Graphics::Scene::IObjectComponentInstanceList& Graphics::Scene::ObjectCore::GetComponentList() const
{
  return mComponentList;
}

const Vector3f&	Graphics::Scene::ObjectCore::GetOrientation() const
{
  return mOrientation;
}

const Graphics::Scene::IObjectInstance& Graphics::Scene::ObjectCore::GetParent() const
{
  return mParent;
}

const Vector3f&	Graphics::Scene::ObjectCore::GetPosition() const
{
  return mPosition;
}
/*
const Graphics::RenderCommand& Graphics::Scene::ObjectCore::GetRenderCommand() const
{
  return mRenderCommand;
}

Graphics::RenderCommand& Graphics::Scene::ObjectCore::GetRenderCommand()
{
  return mRenderCommand;
}
*/
const Vector3f&	Graphics::Scene::ObjectCore::GetScale() const
{
  return mScale;
}

const String& Graphics::Scene::ObjectCore::GetTag() const
{
  return mTag;
}

const Matrix4f& Graphics::Scene::ObjectCore::GetWorldMatrix() const
{
  return mWorldMatrix;
}

void Graphics::Scene::ObjectCore::SetOrientation(const Vector3f& v)
{
  mOrientation = v;
  ClampOrientationAngles(mOrientation);
}

void Graphics::Scene::ObjectCore::SetParent(const IObjectInstance& parent)
{
  #ifdef E_DEBUG // contract enforcement: SetParent should only be used by ObjectCore through AddChild / RemoveChild / RemoveChildren
  bool validOperation = false;
  if (parent)
  {
    IObjectInstanceList childrenList = parent->GetChildrenList();
    for (auto it = begin(childrenList); it != end(childrenList); ++it)
    {
      if (mOwner == (*it).GetPtr())
      {
        validOperation = true;
        break;
      }
    }    
  }
  else
  {
    validOperation = true;
    IObjectInstanceList childrenList = mParent->GetChildrenList();
    for (auto it = begin(childrenList); it != end(childrenList); ++it)
    {
      if (mOwner == (*it).GetPtr())
      {
        validOperation = false;
        break;
      }
    }
  }
  E_ASSERT_MSG(validOperation, E_ASSERT_MSG_SCENE_OBJECT_CORE_MANUAL_PARENT_SET);
  #endif
  mParent = parent;
}

void Graphics::Scene::ObjectCore::SetPosition(const Vector3f& v)
{
  mPosition = v;
}

void Graphics::Scene::ObjectCore::SetScale(const Vector3f& v)
{
  mScale = v;
}

void Graphics::Scene::ObjectCore::SetTag(const String& tag)
{
  mTag = tag;
}

/*----------------------------------------------------------------------------------------------------------------------
ObjectCore methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::ObjectCore::AddChild(const IObjectInstance& child)
{
  //E_ASSERT_PTR(object->GetComponent(IObjectComponent::eComponentTypeRenderable));
  mChildrenList.PushBack(child);
  child->SetParent(mOwner);
}

void Graphics::Scene::ObjectCore::AddComponent(const IObjectComponentInstance& component)
{
  E_ASSERT_MSG(mComponentList.FindValue(component) == nullptr, E_ASSERT_MSG_SCENE_OBJECT_CORE_COMPONENT_UNIQUE);
  mComponentList.PushBack(component);
  component->SetOwner(mOwner);
}

void Graphics::Scene::ObjectCore::ClearTransform()
{
  mPosition.SetZero();
  mOrientation.SetZero();
  mScale.Set(1.0f);
}

void Graphics::Scene::ObjectCore::Load()
{
  // Trigger component unload
  for (auto it = begin(mComponentList); it != end(mComponentList); ++it) (*it)->OnLoad();
  // Load children
  for (auto it = begin(mChildrenList); it != end(mChildrenList); ++it) (*it)->Load();
}

void Graphics::Scene::ObjectCore::RemoveChild(const IObjectInstance& child)
{
#ifdef E_DEBUG
  E_ASSERT_MSG(mChildrenList.RemoveIfFast(child), E_ASSERT_MSG_SCENE_OBJECT_CORE_CHILD_INVALID);
#else
  mChildrenList.RemoveIfFast(object);
#endif
  child->SetParent(kNullObject);
}

void Graphics::Scene::ObjectCore::RemoveChildren()
{
  mChildrenList.Clear();
  for (auto it = begin(mChildrenList); it != end(mChildrenList); ++it) (*it)->SetParent(kNullObject);
}

void Graphics::Scene::ObjectCore::RemoveComponent(IObjectComponent::ComponentType type)
{
  for (auto it = begin(mComponentList); it != end(mComponentList); ++it)
  {
    if ((*it)->GetComponentType() == type)
    {
      mComponentList.RemoveFast(it);
      return;
    }
  }
  E_ASSERT_ALWAYS(E_ASSERT_MSG_SCENE_OBJECT_CORE_COMPONENT_MISSING);
}

void Graphics::Scene::ObjectCore::RemoveComponents()
{
  mComponentList.Trim(1);
}

void Graphics::Scene::ObjectCore::RenderChildren()
{
  //Graphics::Global::GetRenderManager()->AddCommand(mRenderCommand);
  // Render children
  for (auto it = begin(mChildrenList); it != end(mChildrenList); ++it) (*it)->Render();
}

void Graphics::Scene::ObjectCore::Rotate(const Vector3f& v)
{
  mOrientation += v;
  ClampOrientationAngles(mOrientation);
}

void Graphics::Scene::ObjectCore::Scale(const Vector3f& v)
{
  mScale *= v;
}

void Graphics::Scene::ObjectCore::Translate(const Vector3f& v)
{
  mPosition += v;
}

void Graphics::Scene::ObjectCore::Unload()
{
  // Unload children
  for (auto it = begin(mChildrenList); it != end(mChildrenList); ++it) (*it)->Unload();
  // Trigger component unload
  for (auto it = begin(mComponentList); it != end(mComponentList); ++it) (*it)->OnUnload();
  // Clean up render command
  //mRenderCommand.Clear();
}

void Graphics::Scene::ObjectCore::Update(const TimeValue& deltaTime)
{
  // Update local  and world matrices
  UpdateLocalMatrix();
  mWorldMatrix = (mParent) ? mLocalMatrix * mParent->GetWorldMatrix() : mLocalMatrix;
  // Trigger component update
  for (auto it = begin(mComponentList); it != end(mComponentList); ++it) (*it)->OnUpdate(deltaTime);
  // Update children
  for (auto it = begin(mChildrenList); it != end(mChildrenList); ++it) (*it)->Update(deltaTime);
}

/*----------------------------------------------------------------------------------------------------------------------
ObjectCore private methods
------------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::ObjectCore::UpdateLocalMatrix()
{
  // Transformation order is SRT which in row major / row vectors is :
  // scaleMatrix * rotationMatrix * translationMatrix
  Quatf qRotation;
  qRotation.SetRotation(Vector3f(
    Math::Rad(mOrientation.x),
    Math::Rad(mOrientation.y),
    Math::Rad(mOrientation.z)));
  qRotation.GetRotation(mLocalMatrix);
  // Update translation
  mLocalMatrix.SetTranslation(mPosition);
  // Update scaling
  if (mScale != 1.0f) mLocalMatrix.Scale(mScale);
}

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
------------------------------------------------------------------------------------------------------------------------*/

void ClampOrientationAngles(Vector3f& v)
{
  v.x = Math::Normalize180(v.x);
  v.y = Math::Normalize180(v.y);
  v.z = Math::Normalize180(v.z);
}
