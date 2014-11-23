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

// Created 06-Jun-2014 by Elías Lozada-Benavente
// Original created 29-Oct-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IObject.h
This file defines the IObject, IObjectComponent and IObjectGroup interfaces. IObject represent an entity participating in a concrete
scene. IObjectComponent adds functionality to an IObject, not being an entity on its own. IObjectGroup is a collection of IObject 
instances.
*/

#ifndef E3_ISCENE_OBJECT_H
#define E3_ISCENE_OBJECT_H

#include <Text/String.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
// Forward declarations
class IObject;
class IObjectComponent;
class IObjectGroup;

/*----------------------------------------------------------------------------------------------------------------------
IObject types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IObject>           IObjectInstance;
typedef Memory::GCRef<IObjectComponent>  IObjectComponentInstance;
typedef Memory::GCRef<IObjectGroup>      IObjectGroupInstance;
typedef Containers::List<IObjectInstance>            IObjectInstanceList;
typedef Containers::List<IObjectComponentInstance>   IObjectComponentInstanceList;

/*----------------------------------------------------------------------------------------------------------------------
IObjectComponent

Please note that this class has the following usage contract: 

1. All IObjectComponent callbacks are to be called by the rendering thread. In case of a multi-threaded system IObjectComponent
must guarantee that all structure accesses in OnLoad, OnUnload or OnUpdate are thread-safe or that these structures
are only used by IObjectComponent methods (preferred). The goal is using IObjectComponent as a sealed class used exclusively
by the rendering process. IObjectComponent objects should be the only ones working with Graphics::IDevice objects such as
GPU buffers, vertex layouts etc.
2. IObjectComponent can create any Graphics::IDevice instance required. IRenderManager takes care of the destruction of
these objects and guarantees that it will not happen before the rendering thread is terminated.
3. IObjectComponent can have only read access to external variables.
4. OnLoad / OnUnload must handle all the required component initialization / finalization.
----------------------------------------------------------------------------------------------------------------------*/
class IObjectComponent
{
public:
  enum ComponentType
  {
    eComponentTypeLogic,
    eComponentTypeShadow,
    eComponentTypeCount
  };

  virtual                             ~IObjectComponent() {}

  // Accessors
  virtual ComponentType               GetComponentType() const = 0;
  virtual const IObjectInstance& GetOwner() const = 0;
  virtual void                        SetOwner(const IObjectInstance& owner) = 0;
  
  // Callback methods
  virtual void                        OnLoad() = 0;
  virtual void                        OnUnload() = 0;
  virtual void                        OnUpdate(const TimeValue& deltaTime) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IRenderable
----------------------------------------------------------------------------------------------------------------------*/
class IRenderable
{
public:
  virtual       ~IRenderable() {}

  virtual void 	Render() = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ITransformable
----------------------------------------------------------------------------------------------------------------------*/
class ITransformable
{
public:
  virtual                 ~ITransformable() {}

  // Methods
  virtual const Vector3f&	GetOrientation() const = 0;
  virtual const Vector3f&	GetPosition() const = 0;
  virtual const Vector3f& GetScale() const = 0;
  virtual const Matrix4f& GetWorldMatrix() const = 0;
  virtual void			      SetOrientation(const Vector3f& v) = 0;
  virtual void			      SetPosition(const Vector3f& v) = 0;
  virtual void			      SetScale(const Vector3f& v) = 0;

  virtual void            ClearTransform() = 0;
  virtual void			      Rotate(const Vector3f& v) = 0;
  virtual void			      Scale(const Vector3f& v) = 0;
  virtual void			      Translate(const Vector3f& v) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IUpdatable
----------------------------------------------------------------------------------------------------------------------*/
class IUpdatable
{
public:
  virtual       ~IUpdatable() {}

  virtual void 	Update(const TimeValue& deltaTime) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IObject
-----------------------------------------------------------------------------------------------------------------------*/
class IObject : public ITransformable, public IUpdatable, public IRenderable
{
public:
  enum ObjectType
  {
    eObjectTypeCamera,
    eObjectTypeMesh,
    eObjectTypeLight,
    eObjectTypeLightPoint,
    eObjectTypeLightSpot,
    eObjectTypeCount
  };

  virtual                                           ~IObject() {}

  // Accessors
  virtual const IObjectInstanceList&           GetChildrenList() const = 0;
  virtual const IObjectComponentInstance&      GetComponent(IObjectComponent::ComponentType type) const = 0;
  virtual const IObjectComponentInstanceList&  GetComponentList() const = 0;
  virtual const IObjectInstance&               GetParent() const = 0;
  virtual const String&	                            GetTag() const = 0;
  virtual ObjectType                                GetObjectType() const = 0;
  virtual void                                      SetParent(const IObjectInstance& parent) = 0;
  virtual void					                            SetTag(const String& tag) = 0;

  // Methods
  virtual void					                            AddChild(const IObjectInstance& child) = 0;
  virtual void					                            AddComponent(const IObjectComponentInstance& component) = 0;
  virtual void                                      Clear() = 0;
  virtual void                                      Load() = 0;
  virtual void					                            RemoveChildren() = 0;
  virtual void					                            RemoveComponents() = 0;  
  virtual void					                            RemoveChild(const IObjectInstance& child) = 0;
  virtual void					                            RemoveComponent(IObjectComponent::ComponentType type) = 0; 
  virtual void                                      Unload() = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IObjectGroup
----------------------------------------------------------------------------------------------------------------------*/
class IObjectGroup
{
public:
  virtual       ~IObjectGroup() {}

  // Methods
  virtual void	AddObject(const IObjectInstance& object) = 0;
  virtual void	RemoveObject(const IObjectInstance& object) = 0;
};
}
}
}

#endif

