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

/** @file EngineTestPch.h
This file defines includes all the headers required by the Engine test application for pre compilation. 
*/

#ifndef E3_ENGINE_TEST_PCH_H
#define E3_ENGINE_TEST_PCH_H

/*----------------------------------------------------------------------------------------------------------------------
[Utilities]
----------------------------------------------------------------------------------------------------------------------*/
#define E_SETTING_CUSTOM_EXCEPTION_TYPES \
  E_EXCEPTION_TYPE(EngineTest)

#include <Base.h>
#include <Application/Application.h>
#include <Assertion/Exception.h>
#include <FileSystem/File.h>
#include <Math/Matrix4.h>
#include <Math/Quaternion.h>
#include <Math/Projection.h>
#include <Singleton.h>
#include <Time/Timer.h>

/*----------------------------------------------------------------------------------------------------------------------
[Gpu]
----------------------------------------------------------------------------------------------------------------------*/
#include <Graphics/Device.h>
#include <Graphics/IPipeline.h>

/*----------------------------------------------------------------------------------------------------------------------
[Engine]
----------------------------------------------------------------------------------------------------------------------*/

#include <Graphics/Render.h>
#include <Input/Input.h>
#include <Graphics/Scene/ICamera.h>
#include <Graphics/Scene/ILight.h>
#include <Graphics/Scene/ILightPoint.h>
#include <Graphics/Scene/ILightSpot.h>
#include <Graphics/Scene/IShadowComponent.h>
#include <Graphics/Scene/IMesh.h>
#include <Graphics/Scene/Scene.h>
#include <Graphics/Scene/CameraHandler.h>

/*----------------------------------------------------------------------------------------------------------------------
[Thirdparty]
----------------------------------------------------------------------------------------------------------------------*/
#ifdef E_DEBUG
#include <vld.h>
#endif

/*----------------------------------------------------------------------------------------------------------------------
[EngineTest] (samples: order dependent)
----------------------------------------------------------------------------------------------------------------------*/
#include "ISceneSample.h"
#include "TriangleSample.h"
#include "VertexFormatSample.h"
#include "ChildMeshSample.h"
#include "MouseCameraSample.h"
#include "HighlightSample.h"
#include "PhongSample.h"
#include "MaterialSample.h"
#include "LightSample.h"
#include "LightPointSample.h"
#include "LightSpotSample.h"
#include "LightSpotShadowSample.h"
#include "LightShadowSample.h"

/*----------------------------------------------------------------------------------------------------------------------
[EngineTest]
----------------------------------------------------------------------------------------------------------------------*/
#include "DebugWindow.h"
#include "RenderTestApplication.h"
#include "SampleApplication.h"

#endif
