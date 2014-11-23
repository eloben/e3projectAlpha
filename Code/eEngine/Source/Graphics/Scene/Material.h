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

// Created 06-Jun-2014 by Elías Lozada-Benavente 
// Original created 04-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Material.h
This file defines the Material struct.
*/

#ifndef E3_MESH_MATERIAL_H
#define E3_MESH_MATERIAL_H

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
Material
----------------------------------------------------------------------------------------------------------------------*/
class Material : public IMaterial
{
public:
                            Material();
  
  const Color&              GetDiffuseColor() const;
  const ISamplerInstance&   GetDiffuseSampler() const;
  const ITexture2DInstance& GetDiffuseTexture() const;
  U32                       GetID() const;
  const ShaderProperties&   GetShaderProperties() const;
  F32                       GetSmoothness() const;
  const Color&              GetSpecularColor() const;
  void                      Set(PresetID id);
  void                      SetDiffuseColor(const Graphics::Color& color);
  void                      SetDiffuseSampler(const Graphics::ISamplerInstance& diffuseSampler);
  void                      SetDiffuseTexture(const Graphics::ITexture2DInstance& diffuseTexture);
  void                      SetID(U32 value);
  void                      SetSmoothness(F32 factor);
  void                      SetSpecularColor(const Graphics::Color& color);

private:
  ShaderProperties          mShaderProperties;
  ISamplerInstance          mDiffuseSampler;
  ITexture2DInstance        mDiffuseTexture;
  U32                       mID;

  E_DISABLE_COPY_AND_ASSSIGNMENT(Material)
};

/*----------------------------------------------------------------------------------------------------------------------
Material types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<Material> MaterialInstance;
}
}
}

#endif

