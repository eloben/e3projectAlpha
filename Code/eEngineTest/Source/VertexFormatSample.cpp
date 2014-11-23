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

// Created 08-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file VertexFormatSample.cpp
This file defines the VertexFormatSample class.
*/

#include <EngineTestPch.h>

using namespace E;
using namespace E::Graphics::Scene;

/*----------------------------------------------------------------------------------------------------------------------
VertexFormatSample methods
----------------------------------------------------------------------------------------------------------------------*/

void VertexFormatSample::Load(IViewInstance window)
{
  mView = window;

  // Create elements
  ICameraInstance camera = mSceneManager->CreateObject(IObject::eObjectTypeCamera);
  camera->SetProjectionType(ICamera::eProjectionTypeOrthographic);
  camera->Translate(Vector3f(0.0f, 0.0f, -0.5f * camera->GetFar()));
  mView->SetCamera(camera);

  // Create a triangle mesh
  IMeshInstance colorMesh = mSceneManager->CreateObject(IObject::eObjectTypeMesh);
  colorMesh->CreateTriangle(100.0f);
  colorMesh->Translate(Vector3f(-100.0f, 0.0f, 0.0f));
  mSceneManager->GetWorld()->Load(colorMesh);
  /*
  IMeshInstance uncompressedColorMesh = mSceneManager->CreateObject(IObject::eObjectTypeMesh);
  uncompressedColorMesh->CreateTriangle(100.0f);
  uncompressedColorMesh->Translate(Vector3f(100.0f, 100.0f, 0.0f));
  uncompressedColorMesh->SetVertexType(IMesh::eVertexTypeColor);
  mSceneManager->GetWorld()->Load(uncompressedColorMesh);
  */
  IMeshInstance texturedMesh = mSceneManager->CreateObject(IObject::eObjectTypeMesh);
  texturedMesh->CreateQuad(100.0f);
  texturedMesh->Translate(Vector3f(100.0f, 0.0f, 0.0f));
  IMaterialInstance material = mSceneManager->CreateMaterial();
  material->SetDiffuseTexture(mSceneManager->GetTexture2D("test.png"));
  texturedMesh->SetMaterial(material);
  //texturedMesh->SetVertexType(IMesh::eVertexTypeCompressedTexture);
  mSceneManager->GetWorld()->Load(texturedMesh);
  /*
  IMeshInstance uncompressedTexturedMesh = mSceneManager->CreateObject(IObject::eObjectTypeMesh);
  uncompressedTexturedMesh->CreateQuad(100.0f);
  uncompressedTexturedMesh->Translate(Vector3f(100.0f, -100.0f, 0.0f));
  uncompressedTexturedMesh->SetMaterial(material);
  uncompressedTexturedMesh->SetVertexType(IMesh::eVertexTypeTexture);
  mSceneManager->GetWorld()->Load(uncompressedTexturedMesh);*/
}



