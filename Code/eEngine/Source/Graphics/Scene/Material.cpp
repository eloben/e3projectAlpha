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

// Created 23-Sep-2014 by Elías Lozada-Benavente 
// Original created 04-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Material.cpp
This file defines the Material class.
*/

#include <EnginePch.h>
#include "Material.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Material auxiliary
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Color kPresetSpecularColorTable[] =
{
  Graphics::Color(0.913183f,  0.921494f,  0.924524f), // eAluminium
  Graphics::Color(0.549585f,  0.556114f,  0.554256f), // eChromium
  Graphics::Color(0.662124f,  0.654864f,  0.633732f), // eCobalt
  Graphics::Color(0.955008f,  0.637427f,  0.538163f), // eCopper
  Graphics::Color(1.0f,       0.765557f,  0.336057f), // eGold
  Graphics::Color(0.659777f,  0.608679f,  0.525649f), // eNickel
  Graphics::Color(0.672411f,  0.637331f,  0.585456f), // ePlatinum
  Graphics::Color(0.971519f,  0.959915f,  0.915324f), // eSilver
  Graphics::Color(0.541931f,  0.496791f,  0.449419f), // eTitanium
};

/*----------------------------------------------------------------------------------------------------------------------
Material initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::Scene::Material::Material()
  : mID(static_cast<U32>(-1)) {}

/*----------------------------------------------------------------------------------------------------------------------
Material accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::Color& Graphics::Scene::Material::GetDiffuseColor() const
{
  return mShaderProperties.diffuse;
}

const Graphics::ISamplerInstance& Graphics::Scene::Material::GetDiffuseSampler() const
{
  return mDiffuseSampler;
}

const Graphics::ITexture2DInstance& Graphics::Scene::Material::GetDiffuseTexture() const
{
  return mDiffuseTexture;
}

U32 Graphics::Scene::Material::GetID() const
{
  return mID;
}

const Graphics::Scene::Material::ShaderProperties& Graphics::Scene::Material::GetShaderProperties() const
{
  return mShaderProperties;
}

F32 Graphics::Scene::Material::GetSmoothness() const
{
  return mShaderProperties.specular.a;
}

const Graphics::Color& Graphics::Scene::Material::GetSpecularColor() const
{
  return mShaderProperties.specular;
}

void Graphics::Scene::Material::Set(PresetID id)
{
  mShaderProperties.specular = kPresetSpecularColorTable[id];
  mShaderProperties.specular.a = 0.25;
  mShaderProperties.diffuse = Graphics::Color::ZeroColor();
  /*
  mShaderProperties.diffuse = kPresetSpecularColorTable[id];
  mShaderProperties.diffuse.r *= 0.01f;
  mShaderProperties.diffuse.g *= 0.01f;
  mShaderProperties.diffuse.b *= 0.01f;*/
}


void Graphics::Scene::Material::SetDiffuseColor(const Graphics::Color& color)
{
  mShaderProperties.diffuse = color;
}

void Graphics::Scene::Material::SetDiffuseSampler(const Graphics::ISamplerInstance& diffuseSampler)
{
  mDiffuseSampler = diffuseSampler;
}

void Graphics::Scene::Material::SetDiffuseTexture(const Graphics::ITexture2DInstance& diffuseTexture)
{
  mDiffuseTexture = diffuseTexture;
}

void Graphics::Scene::Material::SetID(U32 value)
{
  mID = value;
}

void Graphics::Scene::Material::SetSmoothness(F32 factor)
{
  mShaderProperties.specular.a = Math::Saturate(factor);
}

void Graphics::Scene::Material::SetSpecularColor(const Graphics::Color& color)
{
  mShaderProperties.specular = color;
}