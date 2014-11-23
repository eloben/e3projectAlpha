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

// Created 05-Feb-2010 by Elías Lozada-Benavente (updated 02-Jan-2014)
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file GarbageCollection.h
This file defines a series of garbage collected factory template interfaces for object creation and destruction.
The factory classes defined are:

  - IGarbageCollector: a basic template interface for garbage collection classes.
  - GCCounter: the core counter type used by GCPtr and GCRef
  - GCPtr: smart pointer holding a unique pointer of a heap allocated garbage collected object.
  - GCStaticPtr: scoped smart pointer holding a unique pointer to a non-heap allocated garbage collected object.
  - GCRef: weak reference pointer to a GCPtr.
  - GCConcreteFactory: garbage collected version of ConcreteFactory.
  - GCGenericFactory: garbage collected version of GenericFactory.
*/

#ifndef E3_GC_FACTORY_H
#define E3_GC_FACTORY_H

#include "Factory.h"
#include <Threads/Atomic.h>
#include <Threads/Lock.h>
#include <SafeCast.h>

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID             "Attempting to derrefence a nullptr garbage collected reference"
#define E_ASSERT_GARBAGE_COLLECTION_REFERENCE_ASSIGNMENT_INVALID  "Attempting to assign a non nullptr raw pointer to garbage collected reference"

namespace E
{
namespace Memory
{
/*----------------------------------------------------------------------------------------------------------------------
IGarbageCollector

Please note that this macro has the following usage contract:

1. Collect is called when the reference count of a used object reaches zero.
2. Destroy is called to definitively destroy an object.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class IGarbageCollector
{ 
public:
  virtual ~IGarbageCollector() {}

  virtual void Collect(T* ptr) = 0;
  virtual void Destroy(T* ptr) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
GCCounter
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType>
struct GCCounter
{
  CounterType           count;
  T*                    ptr;
  IGarbageCollector<T>* pCollector;

  GCCounter() 
  : count(0)
  , ptr(nullptr)
  , pCollector(nullptr) {}
};

/*----------------------------------------------------------------------------------------------------------------------
GCPtr

GCPtr is a garbage collected unique pointer to a heap allocated object.

Please note that this macro has the following usage contract:

1. GCPtr acts like std::auto_ptr (C++11 deprecated) ensuring a unique pointer.
2. Copy constructor and assignment operator invalidate an original valid pointer (although a const reference is used
the contained GCCounter instance is mutable).
3. GCPtr allows weak pointer referencing through GCRef.
4. GCPtr raw pointer constructor requires a IGarbageCollector interface pointer which will be called by GCPtr on
pointer destruction and by GCRef on zero reference count.
5. GCPtr uses an atomic reference count by default, being suitable for multi-threading.

- Note that the size of a GCPtr is the same as the size of its raw pointer equivalent, hence there is no need to pass
by reference.
- Note that although the size is the same GCPtr cannot be declared POD.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = A32>
class GCPtr
{
public:
  GCPtr();
  GCPtr(const GCPtr& other);
  GCPtr(T* ptr, IGarbageCollector<T>* pCollector);
  ~GCPtr();

  // Operators
  GCPtr&            operator=(const GCPtr& other);
  GCPtr&            operator=(T* ptr);
  T&                operator*() const;
  T*                operator->() const;
  bool              operator==(const GCPtr& other) const;
  bool              operator==(const T* ptr) const;
  bool              operator!=(const GCPtr& other) const;
  bool              operator!=(const T* ptr) const;  
                    
  // Methods
  void              Reset();

private:
  template <class T, typename CounterType>
  friend class GCRef;

  typedef GCCounter<T, CounterType> Counter;

  mutable Counter*  mpCounter;
      
  void              Swap(const GCPtr& other);
};

/*----------------------------------------------------------------------------------------------------------------------
GCStaticPtr

GCStaticPtr is a garbage collected unique pointer to a non-heap allocated object (automatic or static storage).
GCStaticPtr implements the RAII idiom ("resource acquisition is initialization").

Please note that this macro has the following usage contract:

1. GCStaticPtr is a scoped pointer. The scope defines the validity of the pointer.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = A32>
class GCStaticPtr
{
public:
  GCStaticPtr(T* ptr);
  ~GCStaticPtr();

  bool              operator==(const T* ptr) const;
  bool              operator!=(const T* ptr) const;  

private:
  template <class T, typename CounterType>
  friend class GCRef;

  typedef GCCounter<T, CounterType> Counter;

  Counter*  mpCounter;
      
  E_DISABLE_COPY_AND_ASSSIGNMENT(GCStaticPtr)
};

/*----------------------------------------------------------------------------------------------------------------------
GCRef

GCRef is a weak reference pointer to a GCPtr / GCStaticPtr.

Please note that this macro has the following usage contract:

1. GCRef can only be assigned or constructed either from a GCPtr, GCStaticPtr or from another GCRef.
2. GCRef only allows assignment operation from a raw pointer if it is nullptr to reset the GCRef.
be nullptr).
3. GCRef behavior is similar to WeakPtr with the addition that when the reference count reached zero, the original
IGarbageCollector which created the object is notified.
4. GCRef uses an atomic reference count by default, being suitable for multi-threading.

- Note that the size of a GCRef is the same as the size of its raw pointer equivalent, hence there is no need to pass
by reference.
- Note that although the size is the same GCRef cannot be declared POD.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = A32>
class GCRef
{
public:
  GCRef();
  GCRef(const GCRef& other);
  template <class U>
  GCRef(const GCRef<U, CounterType>& other);
  template <class U>
  GCRef(const GCPtr<U, CounterType>& other);
  template <class U>
  GCRef(const GCStaticPtr<U, CounterType>& other);
  ~GCRef();

  // Operators
  GCRef&    operator=(const GCRef& other);
  template <class U>
  GCRef&    operator=(const GCRef<U, CounterType>& other);
  template <class U>
  GCRef&    operator=(const GCPtr<U, CounterType>& other);
  template <class U>
  GCRef&    operator=(const GCStaticPtr<U, CounterType>& other);
  GCRef&    operator=(T* ptr);
  T&        operator*() const;
  T*        operator->() const;
  bool      operator==(const GCRef& other) const;
  bool      operator==(const T* ptr) const;
  bool      operator!=(const GCRef& other) const;
  bool      operator!=(const T* ptr) const; 
  explicit  operator bool() const;

  // Accessors
  const T*  GetPtr() const;

  // Methods
  void      Reset();

private:
  // Allow private member access to copy constructor & assignment operator method versions which require static_cast conversion. 
  template <typename U, typename CounterType>
  friend class GCRef;

  typedef GCCounter<T, CounterType> Counter;

  Counter*  mpCounter;

  void      Swap(GCRef& other);
};

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory

This class is thread-safe.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class GCConcreteFactory : public IGarbageCollector<T>
{
public:
  typedef GCPtr<T, A32> Ptr;
  typedef GCRef<T, A32> Ref;

  GCConcreteFactory();
  ~GCConcreteFactory();

  const IAllocator*       GetAllocator() const;
  size_t                  GetLiveCount() const;
  void                    SetAllocator(IAllocator* p);

  void                    CleanUp();
  Ref                     Create();

private:
  typedef Containers::List<Ptr> PtrList;

  IAllocator*             mpAllocator;
  PtrList                 mLiveList;
  mutable Threads::Mutex  mAllocatorMutex;
  mutable Threads::Mutex  mLiveListMutex;

  void                    Collect(T* ptr);
  void                    Destroy(T* ptr);

  E_DISABLE_COPY_AND_ASSSIGNMENT(GCConcreteFactory)
};

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory

This class is thread-safe.
----------------------------------------------------------------------------------------------------------------------*/
template<class AbstractType, typename IDType = U32>
class GCGenericFactory : public IGarbageCollector<AbstractType>
{
public:
  // Types
  typedef IFactory<AbstractType> IAbstractFactory;
  typedef typename FactoryIDTypeTraits<IDType>::Parameter ConcreteTypeID;
  typedef GCPtr<AbstractType, A32> Ptr;
  typedef GCRef<AbstractType, A32> Ref;

  GCGenericFactory();

  // Accessors
  size_t                  GetLiveCount() const;
  
  // Methods
  void                    CleanUp();
  Ref                     Create(ConcreteTypeID typeID);
  void                    Register(IAbstractFactory* pAbstractFactory, ConcreteTypeID typeID);
  void                    Unregister(IAbstractFactory* pAbstractFactory);

private:
  typedef GenericFactory<AbstractType, IDType> Factory;
  typedef Containers::List<Ptr> PtrList;
  
  Factory                 mFactory;
  PtrList                 mLiveList;
  mutable Threads::Mutex  mFactoryMutex;
  mutable Threads::Mutex  mLiveListMutex;

  void                    Collect(AbstractType* ptr);
  void                    Destroy(AbstractType* ptr);

  E_DISABLE_COPY_AND_ASSSIGNMENT(GCGenericFactory)
};

/*----------------------------------------------------------------------------------------------------------------------
GCPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCPtr<T, CounterType>::GCPtr()
  : mpCounter(nullptr) {}

template <class T, typename CounterType>
inline GCPtr<T, CounterType>::GCPtr(const GCPtr& other)
  : mpCounter(nullptr)
{
  Swap(other);
}

template <class T, typename CounterType>
inline GCPtr<T, CounterType>::GCPtr(T* ptr, IGarbageCollector<T>* pCollector)
  : mpCounter(nullptr)
{
  E_ASSERT_PTR(ptr);
  E_ASSERT_PTR(pCollector);
  mpCounter = E_NEW(Counter);
  mpCounter->ptr = ptr;
  mpCounter->pCollector = pCollector;
}

template <class T, typename CounterType>
inline GCPtr<T, CounterType>::~GCPtr()
{
  Reset();
}

/*----------------------------------------------------------------------------------------------------------------------
GCPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCPtr<T, CounterType>& GCPtr<T, CounterType>::operator=(const GCPtr& other)
{
  Swap(other);
  return *this;
}

template <class T, typename CounterType>
inline GCPtr<T, CounterType>& GCPtr<T, CounterType>::operator=(T* ptr)
{
  GCPtr(ptr).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
inline T& GCPtr<T, CounterType>::operator*() const
{
  E_ASSERT_PTR(mpCounter);
  return *(mpCounter->ptr);
}

template <class T, typename CounterType>
inline T* GCPtr<T, CounterType>::operator->() const
{
  E_ASSERT_PTR(mpCounter);
  return mpCounter->ptr;
}

template <class T, typename CounterType>
inline bool GCPtr<T, CounterType>::operator==(const GCPtr& other) const
{
  return mpCounter == other.mpCounter ? mpCounter->ptr == other.mpCounter->ptr : false;
}

template <class T, typename CounterType>
inline bool GCPtr<T, CounterType>::operator==(const T* ptr) const
{
  return mpCounter == nullptr ? ptr == nullptr : mpCounter->ptr == ptr;
}

template <class T, typename CounterType>
inline bool GCPtr<T, CounterType>::operator!=(const GCPtr& other) const
{
  return mpCounter == other.mpCounter ? mpCounter->ptr != other.mpCounter->ptr : true;
}

template <class T, typename CounterType>
inline bool GCPtr<T, CounterType>::operator!=(const T* ptr) const
{
  return mpCounter == nullptr ? ptr != nullptr : mpCounter->ptr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCPtr methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCPtr<T, CounterType>::Reset()
{
  if (mpCounter) 
  {      
    mpCounter->pCollector->Destroy(mpCounter->ptr);
    (mpCounter->count == 0) ? E_DELETE(mpCounter) : mpCounter->ptr = nullptr;
  }
  mpCounter = nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCPtr private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCPtr<T, CounterType>::Swap(const GCPtr& other)
{
  Counter* tmpManagedPtr = mpCounter;
  mpCounter = other.mpCounter;
  other.mpCounter = tmpManagedPtr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCStaticPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCStaticPtr<T, CounterType>::GCStaticPtr(T* ptr)
  : mpCounter(E_NEW(Counter))
{
  mpCounter->ptr = ptr;
}

template <class T, typename CounterType>
inline GCStaticPtr<T, CounterType>::~GCStaticPtr()
{
  (mpCounter->count == 0) ? E_DELETE(mpCounter) : mpCounter->ptr = nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCStaticPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline bool GCStaticPtr<T, CounterType>::operator==(const T* ptr) const
{
  return mpCounter->ptr == ptr;
}

template <class T, typename CounterType>
inline bool GCStaticPtr<T, CounterType>::operator!=(const T* ptr) const
{
  return mpCounter->ptr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCRef initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCRef<T, CounterType>::GCRef()
  : mpCounter(nullptr) {}

template <class T, typename CounterType>
inline GCRef<T, CounterType>::GCRef(const GCRef& other)
  : mpCounter(other.mpCounter)
{
  if (mpCounter) ++(mpCounter->count);
}

template <class T, typename CounterType>
template <class U>
inline GCRef<T, CounterType>::GCRef(const GCRef<U, CounterType>& other)
  : mpCounter(reinterpret_cast<Counter*>(other.mpCounter))
{
  if (mpCounter)
  {
    E_ASSERT(SafeCast<T>(other.mpCounter->ptr));
    ++(mpCounter->count);
  }
}

template <class T, typename CounterType>
template <class U>
inline GCRef<T, CounterType>::GCRef(const GCPtr<U, CounterType>& other)
  : mpCounter(reinterpret_cast<Counter*>(other.mpCounter))
{
  if (mpCounter)
  {
    E_ASSERT(SafeCast<T>(other.mpCounter->ptr));
    ++(mpCounter->count);
  }
}

template <class T, typename CounterType>
template <class U>
inline GCRef<T, CounterType>::GCRef(const GCStaticPtr<U, CounterType>& other)
  : mpCounter(reinterpret_cast<Counter*>(other.mpCounter))
{
  if (mpCounter)
  {
    E_ASSERT(SafeCast<T>(other.mpCounter->ptr));
    ++(mpCounter->count);
  }
}

template <class T, typename CounterType>
inline GCRef<T, CounterType>::~GCRef()
{
  Reset();
}

/*----------------------------------------------------------------------------------------------------------------------
GCRef operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCRef<T, CounterType>& GCRef<T, CounterType>::operator=(const GCRef& other)
{
  GCRef(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline GCRef<T, CounterType>& GCRef<T, CounterType>::operator=(const GCRef<U, CounterType>& other)
{
  GCRef(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline GCRef<T, CounterType>& GCRef<T, CounterType>::operator=(const GCPtr<U, CounterType>& other)
{
  GCRef(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline GCRef<T, CounterType>& GCRef<T, CounterType>::operator=(const GCStaticPtr<U, CounterType>& other)
{
  GCRef(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
inline GCRef<T, CounterType>& GCRef<T, CounterType>::operator=(T* ptr)
{
  E_ASSERT_MSG(ptr == nullptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_ASSIGNMENT_INVALID);
  if (ptr == nullptr) Reset();
  return *this;
}

template <class T, typename CounterType>
inline T& GCRef<T, CounterType>::operator*() const
{
  E_ASSERT_MSG(mpCounter && mpCounter->ptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID);
  return *(mpCounter->ptr);
}

template <class T, typename CounterType>
inline T* GCRef<T, CounterType>::operator->() const
{
  E_ASSERT_MSG(mpCounter && mpCounter->ptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID);
  return mpCounter->ptr;
}

template <class T, typename CounterType>
inline bool GCRef<T, CounterType>::operator==(const GCRef& other) const
{
  // Same counter value
  if (mpCounter == other.mpCounter)  return true;
  // Null counter against valid counter
  if (mpCounter == nullptr) return other.mpCounter->ptr == nullptr;
  // Valid counter against nullptr or different counter
  return (other.mpCounter == nullptr) ?  mpCounter->ptr == nullptr : false;
}

template <class T, typename CounterType>
inline bool GCRef<T, CounterType>::operator==(const T* ptr) const
{
  return (mpCounter == nullptr) ? ptr == nullptr : mpCounter->ptr == ptr;
}

template <class T, typename CounterType>
inline bool GCRef<T, CounterType>::operator!=(const GCRef& other) const
{
  return !((*this) == other);
}

template <class T, typename CounterType>
inline bool GCRef<T, CounterType>::operator!=(const T* ptr) const
{
  return !((*this) == ptr);
}

template <class T, typename CounterType>
inline  GCRef<T, CounterType>::operator bool() const
{
  return operator!=(nullptr);
}

/*----------------------------------------------------------------------------------------------------------------------
GCRef accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline const T* GCRef<T, CounterType>::GetPtr() const
{
  return (mpCounter == nullptr) ? nullptr : mpCounter->ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCRef methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCRef<T, CounterType>::Reset()
{
  if (mpCounter) 
  {      
    --(mpCounter->count);
    if (mpCounter->count == 0)
    { 
      if (mpCounter->ptr == nullptr)
      {
        E_DELETE(mpCounter);
      }
      else if (mpCounter->pCollector)
      {
        mpCounter->pCollector->Collect(mpCounter->ptr);
      }
    }
    mpCounter = nullptr;
  }
}

/*----------------------------------------------------------------------------------------------------------------------
GCRef private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCRef<T, CounterType>::Swap(GCRef& other)
{
  Counter* tmpManagedPtr = mpCounter;
  mpCounter = other.mpCounter;
  other.mpCounter = tmpManagedPtr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline GCConcreteFactory<T>::GCConcreteFactory(): mpAllocator(Memory::Global::GetAllocator()) {}

template <class T>
inline GCConcreteFactory<T>::~GCConcreteFactory()
{ 
  E_ASSERT_MSG(mLiveList.IsEmpty(), E_ASSERT_MSG_MEMORY_FACTORY_NOT_EMPTY_CONCRETE_TYPE_FACTORY);
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline const IAllocator* GCConcreteFactory<T>::GetAllocator() const
{
  // [Critical section]
  Threads::Lock l(mAllocatorMutex);
  return mpAllocator;
}

template <class T>
inline size_t GCConcreteFactory<T>::GetLiveCount() const
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  return mLiveList.GetCount();
}

template <class T>
inline void GCConcreteFactory<T>::SetAllocator(IAllocator* p)
{
  // [Critical section]
  Threads::Lock l(mAllocatorMutex);
  mpAllocator = p;
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void GCConcreteFactory<T>::CleanUp()
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  mLiveList.Clear();
}

template <class T>
inline typename GCConcreteFactory<T>::Ref GCConcreteFactory<T>::Create()
{
  T* ptr;
  // [Critical section]
  {
    Threads::Lock l(mAllocatorMutex);
    ptr = E_NEW(T, 1, mpAllocator, IAllocator::eTagFactoryNew);
    E_ASSERT_PTR(ptr);
  }
  // [Critical section]
  {
    Threads::Lock l(mLiveListMutex);
    mLiveList.PushBack(Ptr(ptr, this));
    return *mLiveList.GetBack();
  }
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void GCConcreteFactory<T>::Collect(T* ptr)
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  for (auto it = begin(mLiveList); it != end(mLiveList); ++it)
  {
    if (*it == ptr)
    {
      mLiveList.RemoveFast(it);
      return;
    }
  }
  E_ASSERT_ALWAYS(E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT);
}

template <class T>
inline void GCConcreteFactory<T>::Destroy(T* ptr)
{
  // [Critical section]
  Threads::Lock l(mAllocatorMutex);
  E_ASSERT(ptr);
  E_DELETE(ptr, 1, mpAllocator, IAllocator::eTagFactoryDelete);
}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline GCGenericFactory<AbstractType, IDType>::GCGenericFactory() {}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

// Gets a list of current live objects
template<class AbstractType, typename IDType>
inline size_t GCGenericFactory<AbstractType, IDType>::GetLiveCount() const
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  return mFactory.GetLiveCount();
}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory methods
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::CleanUp()
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  mLiveList.Clear();
  #ifdef E_DEBUG
  // [Critical section]
  {
    Threads::Lock l(mFactoryMutex);
    E_ASSERT(mFactory.GetLiveCount() == 0);
  }
  #endif
}

template<class AbstractType, typename IDType>
inline typename GCGenericFactory<AbstractType, IDType>::Ref GCGenericFactory<AbstractType, IDType>::Create(ConcreteTypeID typeID)
{
  Ptr ptr;
  // [Critical section]
  {
    Threads::Lock l(mFactoryMutex);
    ptr = Ptr(mFactory.Create(typeID), this);
  }
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  mLiveList.PushBack(ptr);
  return *mLiveList.GetBack();
}

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Register(IAbstractFactory* pAbstractFactory, ConcreteTypeID typeID)
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  mFactory.Register(pAbstractFactory, typeID);
}

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Unregister(IAbstractFactory* pAbstractFactory)
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  mFactory.Unregister(pAbstractFactory);
}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory private methods
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Collect(AbstractType* ptr)
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  for (auto it = begin(mLiveList); it != end(mLiveList); ++it)
  {
    if (*it == ptr)
    {
      mLiveList.RemoveFast(it);
      return;
    }
  }
  E_ASSERT_ALWAYS(E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT);
}

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Destroy(AbstractType* ptr)
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  mFactory.Destroy(ptr);
}
}
}

#endif
