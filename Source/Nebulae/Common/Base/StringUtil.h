#ifndef NEBULAE_STRING_UTIL_H__
#define NEBULAE_STRING_UTIL_H__

namespace Nebulae {

// trim from start
std::string ltrim( const std::string& s );

// trim from end
std::string rtrim( const std::string& s );

// trim from both ends
std::string trim( const std::string& s );

}

#endif // NEBULAE_STRING_UTIL_H__