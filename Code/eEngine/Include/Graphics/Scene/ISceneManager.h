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

// Created 15-Sep-2014 by Elías Lozada-Benavente
// Original created 04-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ISceneManager.h
This file defines the ISceneManager interface. ISceneManager is the graphics engine interface.
*/

#ifndef E3_ISCENE_MANAGER_H
#define E3_ISCENE_MANAGER_H

#include <Graphics/Scene/IObject.h>
#include <Graphics/Scene/IRenderer.h>
#include <Graphics/Scene/IView.h>
#include <Graphics/Scene/IWorld.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
ISceneManager

Please note that this interface has the following usage contract: 

1. ISceneManager methods require ISceneManager to be initialized.
2. ISceneManager allows multi-threaded access to acquisition methods. This means the rest of ISceneManager methods
should be called from the same thread.
3. ISceneManager instances: IObjectInstance, IObjectGroupInstance and IObjectComponentInstance are garbage 
collected, not requiring user code to handle their deletion. However all instances live between Initialize and
Finalize calls. This means instances will be no longer valid after ISceneManager finalization.
----------------------------------------------------------------------------------------------------------------------*/
class ISceneManager
{
public:
  enum RendererType
  {
    eRendererTypeForward,
    eRendererTypeCount
  };

  enum ViewID
  {
    eViewID0,
    eViewID1,
    eViewID2,
    eViewID3,
    eViewIDCount
  };

  virtual                               ~ISceneManager() {}
  virtual bool	                        Initialize() = 0;
  virtual void	                        Finalize() = 0;
  virtual bool                          IsReady() const = 0;

  // Accessors
  virtual const IMaterialInstance&      GetDefaultMaterial() const = 0;
  virtual const IRendererInstance&      GetRenderer() const = 0;
  virtual const ITexture2DInstance&     GetTexture2D(const FilePath& filePath) const = 0;
  virtual const IViewInstance&          GetView(U32 viewID) const = 0;
  virtual const IWorldInstance&         GetWorld() const = 0;
  virtual void                          SetRenderer(IRenderer::RendererType type) = 0;
  virtual void                          SetView(U32 viewID, Ptr windowHandle, U32 viewWidth, U32 viewHeight, bool fullScreen) = 0;

  // Methods
  virtual IObjectComponentInstance      CreateComponent(IObjectComponent::ComponentType type) = 0;
  virtual IObjectGroupInstance	        CreateGroup() = 0;
  virtual IMaterialInstance             CreateMaterial() = 0;
  virtual IObjectInstance			          CreateObject(IObject::ObjectType type) = 0;
  virtual void			                    Update() = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ISceneManager types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<ISceneManager> ISceneManagerInstance;
}
}
}

#endif
