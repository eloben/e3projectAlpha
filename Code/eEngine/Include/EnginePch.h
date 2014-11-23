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

// Created 04-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file EnginePch.h
This file defines includes all the headers required by the Engine library for pre compilation. 
*/

#ifndef E3_ENGINE_PCH_H
#define E3_ENGINE_PCH_H

/*----------------------------------------------------------------------------------------------------------------------
[Utilities]
----------------------------------------------------------------------------------------------------------------------*/
#include <Base.h>
#include <Assertion/Assert.h>
#include <Containers/Queue.h>
#include <FileSystem/File.h>
#include <Math/Algorithm.h>
#include <Math/Comparison.h>
#include <Math/Matrix4.h>
#include <Math/Projection.h>
#include <Math/Quaternion.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>
#include <Memory/Factory.h>
#include <Singleton.h>
#include <Text/String.h>
#include <Threads/ThreadPool.h>
#include <Time/Timer.h>
#include <Win32/ComUtil.h>

/*----------------------------------------------------------------------------------------------------------------------
[Gpu]
----------------------------------------------------------------------------------------------------------------------*/
#include <Graphics/Device.h>
#include <Graphics/Graphics.h>

/*----------------------------------------------------------------------------------------------------------------------
[Engine]
----------------------------------------------------------------------------------------------------------------------*/

#include <Input/Input.h>
#include <Graphics/Render.h>
#include <Graphics/Scene/ICamera.h>
#include <Graphics/Scene/ILight.h>
#include <Graphics/Scene/ILightPoint.h>
#include <Graphics/Scene/ILightSpot.h>
#include <Graphics/Scene/IShadowComponent.h>
#include <Graphics/Scene/IMesh.h>
#include <Graphics/Scene/MeshBuffer.h>
#include <Graphics/Scene/Scene.h>
#include <Graphics/Scene/CameraHandler.h>

/*----------------------------------------------------------------------------------------------------------------------
[Thirdparty]
----------------------------------------------------------------------------------------------------------------------*/
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef E_DEBUG
  #include <vld.h>

  #define E_DEBUG_MATRIX4(matrix) \
  { \
    StringBuffer sb; \
    sb << E_TO_STR(matrix); \
    for (U32 i = 0; i < 4; ++i) \
  { \
    sb << "\n"; \
    for (U32 j = 0; j < 4; ++j) sb << matrix(i, j) << ", "; \
  } \
    sb << "\n"; \
    E_DEBUG_OUTPUT(sb.GetPtr()); \
  }\

  #define E_DEBUG_VECTOR3(vector) \
  { \
    StringBuffer sb; \
    sb << E_TO_STR(vector); \
    sb << " " << vector.x << ", " << vector.y << ", " << vector.z << "\n"; \
    E_DEBUG_OUTPUT(sb.GetPtr()); \
  }\

#endif

#endif
