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
   
/** \file UnicodeCharsets.h \brief Contains the UnicodeCharsets class, and
    functions related to the character sets defined in the Unicode
    standard. */

#ifndef _UnicodeCharsets_h_
#define _UnicodeCharsets_h_

#include <Nebulae/Common/Common.h> //Should this be IN common?

#include <boost/cstdint.hpp>

#include <set>


namespace Nebulae {

/** \brief Represents the name and character range of a set of Unicode
    characters.

    Such sets are known as "scripts" in Unicode parlance.  Note that the last
    character in the range is actually one past the last character, in the style
    of the STL. */
struct UnicodeCharset
{
  UnicodeCharset();
  UnicodeCharset( std::string script_name, boost::uint32_t first_char, boost::uint32_t last_char );

  std::string     m_script_name;
  boost::uint32_t m_first_char;
  boost::uint32_t m_last_char;
};

/** Returns true iff all of \a lhs's and \a rhs's members compare equal. */
bool operator==(const UnicodeCharset& lhs, const UnicodeCharset& rhs);

/** Returns true iff \a lhs.m_first_char < \a rhs.m_first_char. */
bool operator<(const UnicodeCharset& lhs, const UnicodeCharset& rhs);


/** Returns a vector containing all defined UnicodeCharset's. */
const std::vector<UnicodeCharset>& AllUnicodeCharsets();

/** Returns the set of the UnicodeCharset's required to render \a str. */
std::set<UnicodeCharset> UnicodeCharsetsToRender(const std::string& str);

/** Returns the UnicodeCharset in which \a c can be found, or 0 if no such
    UnicodeCharset exists. */
const UnicodeCharset* CharsetContaining(boost::uint32_t c);

/** Returns the UnicodeCharset called \a name, or 0 if no such UnicodeCharset
    exists. */
const UnicodeCharset* CharsetWithName(const std::string& name);

} // namespace Nebulae

#endif
