/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

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

// Created 21-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Main.cpp
This file defines the application main entry point.
*/

#include <GraphicsTestPch.h>
#include <Math/Random.h>

using namespace E;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{/*
  Win32::Application winApp;
  Win32::Application::WindowDescriptor windowDesc;
  windowDesc.windowTitle = "Gpu Test - Main Window";
  windowDesc.width = 640;
  windowDesc.height = 480;
  
  SimpleVertexUpdater simpleVertexUpdater;
  windowDesc.pWindowHandler = &simpleVertexUpdater;
  winApp.Initialize(windowDesc);
  
  TextureVertexUpdater textureVertexUpdater;
  AddChildWindow(winApp, &textureVertexUpdater);
  
  IndexedVertexUpdater indexedVertexUpdater;
  AddChildWindow(winApp, &indexedVertexUpdater);
  
  InstanceVertexUpdater instanceVertexUpdater;
  AddChildWindow(winApp, &instanceVertexUpdater);

  IndexedInstanceVertexUpdater indexedInstanceVertexUpdater;
  AddChildWindow(winApp, &indexedInstanceVertexUpdater);
  
  RenderToTextureUpdater renderToTextureUpdater;
  AddChildWindow(winApp, &renderToTextureUpdater);

  RenderDepthToTextureUpdater renderDepthToTextureUpdater;
  AddChildWindow(winApp, &renderDepthToTextureUpdater);
  
  winApp.Run();

  return 0;*/

  Application::Application& app = Application::Global::GetApplication();

  SimpleVertexUpdater simpleVertexUpdater;
  simpleVertexUpdater.Initialize(app.CreateMainWindow(640, 480));

  TextureVertexUpdater textureVertexUpdater;
  textureVertexUpdater.Initialize(app.CreateChildWindow(640, 480));

  IndexedVertexUpdater indexedVertexUpdater;
  indexedVertexUpdater.Initialize(app.CreateChildWindow(640, 480));
  /*
  InstanceVertexUpdater instanceVertexUpdater;
  instanceVertexUpdater.Initialize(app.CreateChildWindow(640, 480));

  IndexedInstanceVertexUpdater indexedInstanceVertexUpdater;
  indexedInstanceVertexUpdater.Initialize(app.CreateChildWindow(640, 480));

  RenderToTextureUpdater renderToTextureUpdater;
  renderToTextureUpdater.Initialize(app.CreateChildWindow(640, 480));

  RenderDepthToTextureUpdater renderDepthToTextureUpdater;
  renderDepthToTextureUpdater.Initialize(app.CreateChildWindow(640, 480));*/

  app.Run();

  return 0;
}

