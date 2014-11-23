/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

Copyright (c) 2010-2014 El�as Lozada-Benavente

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

// Created 18-Sep-2014 by El�as Lozada-Benavente
// Original 15-Nov-2011 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file LogicComponent.cpp
This file defines the LogicComponent class.
*/

#include <EnginePch.h>
#include "LogicComponent.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
LogicComponent initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::LogicComponent::LogicComponent() {}

/*----------------------------------------------------------------------------------------------------------------------
LogicComponent accessors
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::IObjectComponent::ComponentType Graphics::Scene::LogicComponent::GetComponentType() const
{ 
  return IObjectComponent::eComponentTypeLogic;
}

Graphics::Scene::ILogicHandlerInstance Graphics::Scene::LogicComponent::GetHandler() const
{
  return mHandler;
}

const Graphics::Scene::IObjectInstance& Graphics::Scene::LogicComponent::GetOwner() const
{
  return mOwner;
}

void Graphics::Scene::LogicComponent::SetHandler(ILogicHandlerInstance handler)
{
  mHandler = handler;
}

void Graphics::Scene::LogicComponent::SetOwner(const IObjectInstance& owner)
{
  mOwner = owner;
}

/*----------------------------------------------------------------------------------------------------------------------
LogicComponent methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::LogicComponent::OnLoad()
{
  E_ASSERT_PTR(mHandler);
  mHandler->OnLoad();
}

void Graphics::Scene::LogicComponent::OnUnload()
{
  E_ASSERT_PTR(mHandler);
  mHandler->OnUnload();
}

void Graphics::Scene::LogicComponent::OnUpdate(const TimeValue& deltaTime)
{
  E_ASSERT_PTR(mHandler);
  mHandler->OnUpdate(deltaTime);
}
