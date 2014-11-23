/*--------------------------------------------------------------------------------------------------------------------------------------
This source file is part of the E2 Engine

Copyright (c) 2010-2012 Elías Lozada-Benavente

Based on original by Frank D. Luna Code (2008)

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
---------------------------------------------------------------------------------------------------------------------------------------*/

// Created 13-Aug-2014 by Elías Lozada-Benavente
// Original 25-Feb-2012 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file MeshHelper.cpp
This file defines the MeshHelper class. 
Implementation portions: Copyright (C) Frank D. Luna, 2008
*/

#include <EnginePch.h>
#include "MeshHelper.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary
----------------------------------------------------------------------------------------------------------------------*/

void BuildCylinderBottomCap(Graphics::Scene::MeshBuffer& meshBuffer, F32 radius, F32 height, U32 sliceCount);
void BuildCylinderTopCap(Graphics::Scene::MeshBuffer& meshBuffer, F32 radius, F32 height, U32 sliceCount);
void BuildCylinderStacks(Graphics::Scene::MeshBuffer& meshBuffer, F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount);

/*----------------------------------------------------------------------------------------------------------------------
MeshHelper methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::Scene::MeshHelper::CreateCube(Graphics::Scene::MeshBuffer& meshBuffer, F32 length)
{
  // Clear meshBuffer
  meshBuffer.Clear();

	// Fill vertices
  F32 r = length * 0.5f;
	meshBuffer.Add(Vector3f(-r, -r, -r), Vector2f(0.0f, 1.0), Vector3f(0.0f, 0.0f, -1.0));
	meshBuffer.Add(Vector3f(-r,  r, -r), Vector2f(0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0));
	meshBuffer.Add(Vector3f( r,  r, -r), Vector2f(1.0, 0.0f), Vector3f(0.0f, 0.0f, -1.0));
	meshBuffer.Add(Vector3f( r, -r, -r), Vector2f(1.0, 1.0), Vector3f(0.0f, 0.0f, -1.0));

	meshBuffer.Add(Vector3f(-r, -r,  r), Vector2f(1.0, 1.0), Vector3f(0.0f, 0.0f,  1.0));
	meshBuffer.Add(Vector3f( r, -r,  r), Vector2f(0.0f, 1.0), Vector3f(0.0f, 0.0f,  1.0));
	meshBuffer.Add(Vector3f( r,  r,  r), Vector2f(0.0f, 0.0f), Vector3f(0.0f, 0.0f,  1.0));
	meshBuffer.Add(Vector3f(-r,  r,  r), Vector2f(1.0, 0.0f), Vector3f(0.0f, 0.0f,  1.0));
	
	meshBuffer.Add(Vector3f(-r,  r, -r), Vector2f(0.0f, 1.0), Vector3f(0.0f,  1.0, 0.0f));
	meshBuffer.Add(Vector3f(-r,  r,  r), Vector2f(0.0f, 0.0f), Vector3f(0.0f,  1.0, 0.0f));
	meshBuffer.Add(Vector3f( r,  r,  r), Vector2f(1.0, 0.0f), Vector3f(0.0f,  1.0, 0.0f));
	meshBuffer.Add(Vector3f( r,  r, -r), Vector2f(1.0, 1.0), Vector3f(0.0f,  1.0, 0.0f));

	meshBuffer.Add(Vector3f(-r, -r, -r), Vector2f(1.0, 1.0), Vector3f(0.0f, -1.0, 0.0f));
	meshBuffer.Add(Vector3f( r, -r, -r), Vector2f(0.0f, 1.0), Vector3f(0.0f, -1.0, 0.0f));
	meshBuffer.Add(Vector3f( r, -r,  r), Vector2f(0.0f, 0.0f), Vector3f(0.0f, -1.0, 0.0f));
	meshBuffer.Add(Vector3f(-r, -r,  r), Vector2f(1.0, 0.0f), Vector3f(0.0f, -1.0, 0.0f));

	meshBuffer.Add(Vector3f(-r, -r,  r), Vector2f(0.0f, 1.0), Vector3f(-1.0, 0.0f, 0.0f));
	meshBuffer.Add(Vector3f(-r,  r,  r), Vector2f(0.0f, 0.0f), Vector3f(-1.0, 0.0f, 0.0f));
	meshBuffer.Add(Vector3f(-r,  r, -r), Vector2f(1.0, 0.0f), Vector3f(-1.0, 0.0f, 0.0f));
	meshBuffer.Add(Vector3f(-r, -r, -r), Vector2f(1.0, 1.0), Vector3f(-1.0, 0.0f, 0.0f));

	meshBuffer.Add(Vector3f( r, -r, -r), Vector2f(0.0f, 1.0), Vector3f( 1.0, 0.0f, 0.0f));
	meshBuffer.Add(Vector3f( r,  r, -r), Vector2f(0.0f, 0.0f), Vector3f( 1.0, 0.0f, 0.0f));
	meshBuffer.Add(Vector3f( r,  r,  r), Vector2f(1.0, 0.0f), Vector3f( 1.0, 0.0f, 0.0f));
	meshBuffer.Add(Vector3f( r, -r,  r), Vector2f(1.0, 1.0), Vector3f( 1.0, 0.0f, 0.0f));

	// Fill indices
	for (U32 i = 0; i < 6; ++i)
	{
		meshBuffer.indexList.PushBack(i * 4);
		meshBuffer.indexList.PushBack(i * 4 + 1);
		meshBuffer.indexList.PushBack(i * 4 + 2);
		meshBuffer.indexList.PushBack(i * 4);
		meshBuffer.indexList.PushBack(i * 4 + 2);
		meshBuffer.indexList.PushBack(i * 4 + 3);
	}
}

void Graphics::Scene::MeshHelper::CreateCylinder(Graphics::Scene::MeshBuffer& meshBuffer, F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount)
{
  // Clear meshBuffer
  meshBuffer.Clear();

  BuildCylinderStacks(meshBuffer, topRadius, bottomRadius, height, sliceCount, stackCount);
  BuildCylinderTopCap(meshBuffer, topRadius, height, sliceCount);
  BuildCylinderBottomCap(meshBuffer, bottomRadius, height, sliceCount);
}

void Graphics::Scene::MeshHelper::CreateQuad(Graphics::Scene::MeshBuffer& meshBuffer, F32 width, F32 height /* = width */)
{
  // Clear meshBuffer
  meshBuffer.Clear();

  // Fill vertices
  F32 x = width * 0.5f;
  F32 y = height * 0.5f;
  meshBuffer.Add(Vector3f(-x, -y, 0.0f), Vector2f(0.0f, 1.0f), Vector3f(0.0f, 0.0f, -1.0));
  meshBuffer.Add(Vector3f(-x,  y, 0.0f), Vector2f(0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0));
  meshBuffer.Add(Vector3f( x,  y, 0.0f), Vector2f(1.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0));
  meshBuffer.Add(Vector3f( x, -y, 0.0f), Vector2f(1.0f, 1.0f), Vector3f(0.0f, 0.0f, -1.0));

  // Fill indices
  meshBuffer.indexList.PushBack(0);
  meshBuffer.indexList.PushBack(1);
  meshBuffer.indexList.PushBack(2);
  meshBuffer.indexList.PushBack(0);
  meshBuffer.indexList.PushBack(2);
  meshBuffer.indexList.PushBack(3);
}

void Graphics::Scene::MeshHelper::CreateSphere(Graphics::Scene::MeshBuffer& meshBuffer, F32 radius, U32 sliceCount, U32 stackCount)
{
  // Clear meshBuffer
  meshBuffer.Clear();

  F32 phiStep = Math::kPif / stackCount;
  F32 thetaStep = Math::k2Pif / sliceCount;

  // Do not count the poles as rings
  U32 ringCount = stackCount - 1;
  for(U32 i = 1; i <= ringCount; ++i)
  {
    F32 phi = i * phiStep;
    for(U32 j = 0; j <= sliceCount; ++j)
    {
      F32 theta = j * thetaStep;
      Vector3f position(
        radius * Math::Sin(phi) * Math::Cos(theta),
        radius * Math::Cos(phi),
        radius * Math::Sin(phi) * Math::Sin(theta));
      Vector3f normal = position;
      normal.Normalize();
      Vector2f texCoord(theta / Math::k2Pif, phi / Math::kPif);

      meshBuffer.Add(position, texCoord, normal);
    }
  }

  // Add the poles
  meshBuffer.Add(Vector3f(0.0f, -radius, 0.0f), Vector2f(0.0f, 1.0f), Vector3f(0.0f, -1.0f, 0.0f));
  meshBuffer.Add(Vector3f(0.0f,  radius, 0.0f), Vector2f(0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
  
  U32 northPoleIndex = static_cast<U32>(meshBuffer.positionList.GetCount() - 1);
  U32 southPoleIndex = northPoleIndex - 1;
  U32 ringVertexCount = sliceCount + 1;
  // Non poles
  for(U32 i = 0; i < stackCount - 2; ++i)
  {
    for(U32 j = 0; j < sliceCount; ++j)
    {
      meshBuffer.indexList.PushBack( i    * ringVertexCount + j);
      meshBuffer.indexList.PushBack( i    * ringVertexCount + j + 1);
      meshBuffer.indexList.PushBack((i+1) * ringVertexCount + j);

      meshBuffer.indexList.PushBack((i+1) * ringVertexCount + j);
      meshBuffer.indexList.PushBack( i    * ringVertexCount + j + 1);
      meshBuffer.indexList.PushBack((i+1) * ringVertexCount + j + 1);
    }
  }
  // Top stack indices
  for(U32 i = 0; i < sliceCount; ++i)
  {
    meshBuffer.indexList.PushBack(northPoleIndex);
    meshBuffer.indexList.PushBack(i + 1);
    meshBuffer.indexList.PushBack(i);
  }
  // Bottom stack was indices
  U32 baseIndex = (ringCount - 1) * ringVertexCount;
  for(U32 i = 0; i < sliceCount; ++i)
  {
    meshBuffer.indexList.PushBack(southPoleIndex);
    meshBuffer.indexList.PushBack(baseIndex + i);
    meshBuffer.indexList.PushBack(baseIndex + i + 1);
  }
}

void Graphics::Scene::MeshHelper::CreateTriangle(Graphics::Scene::MeshBuffer& meshBuffer, F32 length)
{
  // Clear meshBuffer
  meshBuffer.Clear();

  // Fill in the front face vertex data.
  F32 r = length * 0.5f;
  meshBuffer.Add(Vector3f(-r,   -r, 0.0f), Graphics::Color::eRed);
  meshBuffer.Add(Vector3f(0.0f,  r, 0.0f), Graphics::Color::eGreen);
  meshBuffer.Add(Vector3f( r,   -r, 0.0f), Graphics::Color::eBlue);
}


/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
------------------------------------------------------------------------------------------------------------------------*/

void BuildCylinderBottomCap(Graphics::Scene::MeshBuffer& meshBuffer, F32 radius, F32 height, U32 sliceCount)
{
	U32 baseIndex = static_cast<U32> (meshBuffer.positionList.GetCount());

	// Duplicate cap vertices because the texture coordinates and normals differ.
	F32 y = -0.5f * height;
	Vector3f normal(0.0f, -1.0f, 0.0f);

	// Ring vertices
	F32 dTheta = Math::k2Pif / sliceCount;

	for(U32 i = 0; i <= sliceCount; ++i)
	{
		Vector3f position(radius * cosf(i * dTheta), y, radius * sinf(i * dTheta));
		Vector2f texCoord(0.5f * position.x / radius + 0.5f, -0.5f * position.z / radius + 0.5f);
		meshBuffer.Add(position, texCoord,	normal);
	}

	// Cap center vertex
	meshBuffer.Add(Vector3f(0.0f, y, 0.0f), Vector2f(0.5f, 0.5f), normal);

	// Center vertex index
	U32 centerIndex = static_cast<U32> (meshBuffer.positionList.GetCount()) - 1;

	// Compute indices
	for(U32 i = 0; i < sliceCount; ++i)
	{
		meshBuffer.indexList.PushBack(centerIndex);
		meshBuffer.indexList.PushBack(baseIndex + i);
		meshBuffer.indexList.PushBack(baseIndex + i + 1);
	}
}

void BuildCylinderStacks(Graphics::Scene::MeshBuffer& meshBuffer, F32 topRadius, F32 bottomRadius, F32 height, U32 sliceCount, U32 stackCount)
{
	F32 stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from distance to position
	F32 radiusStep = (topRadius - bottomRadius) / stackCount;

	U32 ringCount = stackCount + 1;

	// Compute vertices for each stack ring
	for(U32 i = 0; i < ringCount; ++i)
	{
		F32 y = -0.5f * height + i * stackHeight;
		F32 r = bottomRadius + i * radiusStep;

		// Height and radius of next ring up.
		F32 nextY = -0.5f * height + (i + 1) * stackHeight;
		F32 nextR = bottomRadius   + (i + 1) * radiusStep;

		// Vertices of ring
		F32 dTheta = Math::k2Pif / sliceCount;
		for(U32 j = 0; j <= sliceCount; ++j)
		{
			F32 c = cosf(j * dTheta);
			F32 s = sinf(j * dTheta);
			Vector2f texCoord(	static_cast<F32> (j) / sliceCount, 
								1.0f -	static_cast<F32> (i) / stackCount);

			// Partial derivative in theta direction to get tangent vector (this is a unit vector).
			Vector3f tangent(-s, 0.0f, c);

			// Compute tangent vector down the slope of the cone (if the position/distance 
			// radii differ then we get a cone and not a true cylinder)
			Vector3f position(r * c, y, r * s);
			Vector3f nextPosition(nextR * c, nextY, nextR * s);
			Vector3f distance = position - nextPosition;
			distance.Normalize();

			Vector3f normal = Vector3f::Cross(tangent, distance);
			normal.Normalize();

      meshBuffer.Add(position, texCoord, normal);
		}
	}

	// Compute indices for each stack
	U32 ringVertexCount = sliceCount + 1;

	for (U32 i = 0; i < stackCount; ++i)
	{
		for (U32 j = 0; j < sliceCount; ++j)
		{
			meshBuffer.indexList.PushBack( i		* ringVertexCount + j);
			meshBuffer.indexList.PushBack((i + 1) * ringVertexCount + j);
			meshBuffer.indexList.PushBack((i + 1) * ringVertexCount + j + 1);

			meshBuffer.indexList.PushBack( i		* ringVertexCount + j);
			meshBuffer.indexList.PushBack((i + 1) * ringVertexCount + j + 1);
			meshBuffer.indexList.PushBack( i		* ringVertexCount + j + 1);
		}
	}
}


void BuildCylinderTopCap(Graphics::Scene::MeshBuffer& meshBuffer, F32 radius, F32 height, U32 sliceCount)
{
 	U32 baseIndex = static_cast<U32> (meshBuffer.positionList.GetCount());

	// Duplicate cap vertices because the texture coordinates and normals differ.
	F32 y = 0.5f * height;
	Vector3f normal(0.0f, 1.0f, 0.0f);

	// Ring vertices
	F32 dTheta = Math::k2Pif / sliceCount;

	for(U32 i = 0; i <= sliceCount; ++i)
	{
		Vector3f position(radius * cosf(i * dTheta), y, radius * sinf(i * dTheta));
		Vector2f texCoord(0.5f * position.x / radius + 0.5f, -0.5f * position.z / radius + 0.5f);
		meshBuffer.Add(position, texCoord,	normal);
	}

	// Cap center vertex
	meshBuffer.Add(Vector3f(0.0f, y, 0.0f), Vector2f(0.5f, 0.5f), normal);

	// Center vertex index
	U32 centerIndex = static_cast<U32> (meshBuffer.positionList.GetCount()) - 1;

	// Compute indices
	for(U32 i = 0; i < sliceCount; ++i)
	{
		meshBuffer.indexList.PushBack(centerIndex);
		meshBuffer.indexList.PushBack(baseIndex + i + 1);
		meshBuffer.indexList.PushBack(baseIndex + i);
	}
}
