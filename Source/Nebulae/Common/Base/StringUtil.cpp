
#include <Nebulae/Common/Common.h>


std::string Nebulae::ltrim( const std::string& s ) 
{
  std::string ret( s );
  ret.erase(ret.begin(), std::find_if(ret.begin(), ret.end(), [](unsigned char c) { return !std::isspace(c); }));
  return ret;
}


std::string Nebulae::rtrim( const std::string& s ) 
{
  std::string ret( s );
  ret.erase(std::find_if(ret.rbegin(), ret.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), ret.end());
  return ret;
}


std::string Nebulae::trim( const std::string& s ) 
{
  return ltrim( rtrim(s) );
}