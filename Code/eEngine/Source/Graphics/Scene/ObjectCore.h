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

/** @file ObjectCore.h
This file declares the ObjectCore class.
*/

#ifndef E3_SCENE_OBJECT_CORE_H
#define E3_SCENE_OBJECT_CORE_H

#include "ScenePipeline.h"

/*----------------------------------------------------------------------------------------------------------------------
E_GRAPHICS_DEFINE_SCENE_OBJECT_COMMON (class definition helper)
----------------------------------------------------------------------------------------------------------------------*/
#define E_GRAPHICS_DEFINE_SCENE_OBJECT_COMMON(objectTypeID, core) \
  const IObjectInstanceList&          GetChildrenList() const                                   { return core.GetChildrenList(); } \
  const IObjectComponentInstance&     GetComponent(IObjectComponent::ComponentType type) const  { return core.GetComponent(type); } \
  const IObjectComponentInstanceList& GetComponentList() const                                  { return core.GetComponentList(); } \
  IObject::ObjectType                 GetObjectType() const                                     { return objectTypeID; } \
  const Vector3f&	                    GetOrientation() const                                    { return core.GetOrientation(); } \
  const IObjectInstance&              GetParent() const                                         { return core.GetParent(); } \
  const Vector3f&	                    GetPosition() const                                       { return core.GetPosition(); } \
  const Vector3f&                     GetScale() const                                          { return core.GetScale(); } \
  const String&			                  GetTag() const                                            { return core.GetTag(); } \
  const Matrix4f&			                GetWorldMatrix() const                                { return core.GetWorldMatrix(); } \
  void			                          SetOrientation(const Vector3f& v)                         { core.SetOrientation(v); } \
  void			                          SetParent(const IObjectInstance& parent)                  { core.SetParent(parent); } \
  void			                          SetPosition(const Vector3f& v)                            { core.SetPosition(v); } \
  void			                          SetScale(const Vector3f& v)                               { core.SetScale(v); } \
  void							                  SetTag(const String& tag)                                 { core.SetTag(tag); } \
  void							                  AddChild(const IObjectInstance& child)                          { core.AddChild(child); } \
  void							                  AddComponent(const IObjectComponentInstance& component)          { core.AddComponent(component); } \
  void							                  ClearTransform()                                          { core.ClearTransform(); } \
  void							                  RemoveChild(const IObjectInstance& child)                       { core.RemoveChild(child); } \
  void					                      RemoveChildren()                                          { core.RemoveChildren(); } \
  void							                  RemoveComponent(IObjectComponent::ComponentType type)     { core.RemoveComponent(type); } \
  void					                      RemoveComponents()                                        { core.RemoveComponents(); } \
  void			                          Rotate(const Vector3f& v)                                 { core.Rotate(v); } \
  void			                          Scale(const Vector3f& v)                                  { core.Scale(v); } \
  void			                          Translate(const Vector3f& v)                              { core.Translate(v); } \

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ObjectCore

Please note that this class has the following usage contract: 

1. This class is a private helper to implement common IObject methods.
2. ObjectCore uses SetParent to track parent-child relationships however, this method should not be used by client 
code. AddChild / RemoveChild / RemoveChildren should be used for that purpose instead (ObjectCore will assert 
otherwise).
----------------------------------------------------------------------------------------------------------------------*/
class ObjectCore
{
public:
  ObjectCore(IObject* pOwner);

  // Accessors
  const IObjectInstanceList&           GetChildrenList() const;
  const IObjectComponentInstance&      GetComponent(IObjectComponent::ComponentType type) const;
  const IObjectComponentInstanceList&  GetComponentList() const;
  const Vector3f&	                          GetOrientation() const;
  const IObjectInstance&               GetParent() const;
  const Vector3f&	                          GetPosition() const;
  const Vector3f&                           GetScale() const;
  const String&				                      GetTag() const;
  const Matrix4f&                           GetWorldMatrix() const;
  void			                                SetOrientation(const Vector3f& v);
  void                                      SetParent(const IObjectInstance& parent);
  void			                                SetPosition(const Vector3f& v);
  void			                                SetScale(const Vector3f& v);
  void							                        SetTag(const String& tag);

  // Methods
  void							                        AddChild(const IObjectInstance& child);
  void							                        AddComponent(const IObjectComponentInstance& component);
  void                                      ClearTransform();
  void			                                Load();
  void							                        RemoveChild(const IObjectInstance& child);
  void					                            RemoveChildren();
  void							                        RemoveComponent(IObjectComponent::ComponentType type);
  void					                            RemoveComponents();
  void					                            RenderChildren();
  void			                                Rotate(const Vector3f& v);
  void			                                Scale(const Vector3f& v);
  void			                                Translate(const Vector3f& v);      
  void			                                Unload();
  void                                      Update(const TimeValue& deltaTime);
              
private:
  typedef Memory::GCStaticPtr<IObject> IObjectStaticPtr;

  String						                  mTag;
  Matrix4f                            mLocalMatrix;
  Matrix4f                            mWorldMatrix;
  Vector3f		                        mPosition;
  Vector3f		                        mOrientation;
  Vector3f				                    mScale;
  IObjectInstanceList            mChildrenList;
  IObjectComponentInstanceList   mComponentList;
  IObjectStaticPtr               mOwner;
  IObjectInstance                mParent;

  void                                UpdateLocalMatrix();

  E_DISABLE_COPY_AND_ASSSIGNMENT(ObjectCore)
}; 
}
}
}

#endif
