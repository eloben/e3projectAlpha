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

// Created 22-Sep-2014 by Elías Lozada-Benavente
// Original created 30-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IMaterial.h
This file defines the IMaterial interface.
*/

#ifndef E3_IMATERIAL_H
#define E3_IMATERIAL_H

#include <Graphics/ITexture2D.h>

namespace E 
{
namespace Graphics
{
namespace Scene
{
/*----------------------------------------------------------------------------------------------------------------------
IMaterial
----------------------------------------------------------------------------------------------------------------------*/
class IMaterial
{
public:
  enum PresetID
  {
    eAluminium,
    eChromium,
    eCobalt,
    eCopper,
    eGold,
    eNickel,
    ePlatinum,
    eSilver,
    eTitanium,    
  };

  struct ShaderProperties
  {
    Graphics::Color diffuse;
    Graphics::Color specular;

    ShaderProperties()
      : diffuse(Graphics::Color::eWhite)
      , specular(Graphics::Color::eWhite) {}
  };

  virtual ~IMaterial() {}

  // Accessors
  virtual const Graphics::Color&                GetDiffuseColor() const = 0;
  virtual const Graphics::ITexture2DInstance&   GetDiffuseTexture() const = 0;
  virtual const ShaderProperties&               GetShaderProperties() const = 0;
  virtual F32                                   GetSmoothness() const = 0;
  virtual const Graphics::Color&                GetSpecularColor() const = 0;
  virtual void                                  Set(PresetID id) = 0;
  virtual void                                  SetDiffuseColor(const Graphics::Color& color) = 0;
  virtual void                                  SetDiffuseTexture(const Graphics::ITexture2DInstance& diffuseTexture) = 0;
  virtual void                                  SetSmoothness(F32 factor) = 0;
  virtual void                                  SetSpecularColor(const Graphics::Color& color) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IMaterial types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCRef<IMaterial> IMaterialInstance;
}
}
}

#endif
