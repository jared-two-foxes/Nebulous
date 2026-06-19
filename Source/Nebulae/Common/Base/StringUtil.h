#ifndef NEBULAE_COMMON_BASE_STRINGUTIL_H_
#define NEBULAE_COMMON_BASE_STRINGUTIL_H_

namespace Nebulae
{

// trim from start
std::string ltrim( const std::string& s );

// trim from end
std::string rtrim( const std::string& s );

// trim from both ends
std::string trim( const std::string& s );

} // namespace Nebulae

#endif // NEBULAE_COMMON_BASE_STRINGUTIL_H_