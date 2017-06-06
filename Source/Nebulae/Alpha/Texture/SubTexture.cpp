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

#include "SubTexture.h"
#include <Nebulae/Alpha/Texture/Texture.h>


using namespace Nebulae;


SubTexture::SubTexture()
  : m_width(0),
    m_height(0),
    m_tex_coords()
{}


SubTexture::SubTexture( const Texture* texture, Real x1, Real y1, Real x2, Real y2 )  
  : m_texture(texture),
    m_width(x2 - x1),
    m_height(y2 - y1),
    m_tex_coords()
{
  NE_ASSERT( m_texture != NULL, "Attempted to construct subtexture from invalid texture" )();
  NE_ASSERT( x2 >= x1 && y2 >= y1, "Attempted to construct subtexture from invalid coordinates")();

  m_tex_coords[0] = x1; //Value(x1 * 1.0f / texture->GetWidth());
  m_tex_coords[1] = y1; //Value(y1 * 1.0f / texture->GetHeight());
  m_tex_coords[2] = x2; //Value(x2 * 1.0f / texture->GetWidth());
  m_tex_coords[3] = y2; //Value(y2 * 1.0f / texture->GetHeight());

  m_width  = (x2 - x1) * texture->GetWidth();
  m_height = (y2 - y1) * texture->GetHeight();
}


SubTexture::~SubTexture()
{}


SubTexture::SubTexture( const SubTexture& rhs )
{ *this = rhs; }


SubTexture& SubTexture::operator=( const SubTexture& rhs )
{
  if( this != &rhs ) {
    m_texture       = rhs.m_texture;
    m_width         = rhs.m_width;
    m_height        = rhs.m_height;
    m_tex_coords[0] = rhs.m_tex_coords[0];
    m_tex_coords[1] = rhs.m_tex_coords[1];
    m_tex_coords[2] = rhs.m_tex_coords[2];
    m_tex_coords[3] = rhs.m_tex_coords[3];
  }

  return *this;
}


bool 
SubTexture::IsEmpty() const
{ return !m_texture; }


const Real*
SubTexture::GetTexCoords() const
{ return m_tex_coords; }


int
SubTexture::GetWidth() const
{ return m_width; }


int
SubTexture::GetHeight() const
{ return m_height; }


const Texture* 
SubTexture::GetTexture() const
{ return m_texture; }


const Vector2
SubTexture::GetMinCoord() const
{ return Vector2(m_tex_coords[0], m_tex_coords[1]); }


const Vector2                    
SubTexture::GetMaxCoord() const 
{ return Vector2(m_tex_coords[2], m_tex_coords[3]); }