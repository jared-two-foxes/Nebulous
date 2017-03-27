// -*- C++ -*-
/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */

/** \file Texture.h \brief Contains the Texture class, which encapsulates an
    OpenGL texture object; the SubTexture class, which represents a portion of
    an OpenGL texture object; and the TextureManager class, which provides
    GUI-wide management of Texture objects. */

#ifndef __NEBULAE_ALPHA_SUBTEXTURE_H__
#define __NEBULAE_ALPHA_SUBTEXTURE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Texture;

/** \brief This class is a convenient way to store the info needed to use a
    portion of a texture. */
class SubTexture
{
private:
  const Texture* m_texture;       ///< shared_ptr to texture object with entire image
  int            m_width;
  int            m_height;
  Real           m_tex_coords[4]; ///< position of element within containing texture 

  public:
    /** \name Structors */ ///@{
    SubTexture(); ///< default ctor

    /** Creates a SubTexture from a GG::Texture and coordinates into it.*/
    SubTexture( const Texture* texture, Real x1 = 0, Real y1 = 0, Real x2 = 1.f, Real y2 = 1.f );

    SubTexture( const SubTexture& rhs ); ///< copy ctor

    SubTexture& operator=(const SubTexture& rhs); ///< assignment operator

    virtual ~SubTexture(); ///< virtual dtor
    //@}

    /** \name Accessors */ ///@{
    bool           IsEmpty() const;      ///< returns true if this object has no associated GG::Texture
    int            GetWidth() const;     ///< width of sub-texture in pixels
    int            GetHeight() const;    ///< height of sub-texture in pixels
    const Texture* GetTexture() const;   ///< returns the texture the SubTexture is a part of
    const Real*    GetTexCoords() const; ///< texture coordinates to use when blitting this sub-texture
    const Vector2  GetMinCoord() const;  ///< returns a 2d vector containing the minimum texture coordinates.
    const Vector2  GetMaxCoord() const;  ///< returns a 2d vector containing the maximum texture coordinates.
    //@}

};

}

#endif //__NEBULAE_ALPHA_SUBTEXTURE_H__