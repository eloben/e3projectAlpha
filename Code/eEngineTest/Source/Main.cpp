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

// Created 05-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Main.cpp
This file defines the application main entry point.
*/

#include <EngineTestPch.h>
#include <Threads/ThreadPool.h>
#include <Application/Application.h>

using namespace E;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  const WString kApplicationName(L"E::Application");
  const WString kApplicationErrorTitle(L"Exception");
  const WString kApplicationWindowExceptionMessage(L"Window exception");

  try
  {
/*
    RenderTestApplication renderTestApp;
    renderTestApp.Initialize(512, 512);*/
    SampleApplication sampleApp;
    sampleApp.Initialize(800, 600);
  }
  catch (const Exception&)
  {
    ::MessageBox(::GetActiveWindow(), kApplicationWindowExceptionMessage.GetPtr(), kApplicationErrorTitle.GetPtr(), MB_OK);
  }

  
//   SampleApplication sampleApp;
//   //sampleApp.Initialize(800, 600);
//   sampleApp.Initialize(512, 512);
//   //sampleApp.Initialize(1280, 720);

  return 0;
}
