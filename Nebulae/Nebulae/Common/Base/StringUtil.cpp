
#include <Nebulae/Common/Common.h>


std::string Nebulae::ltrim( const std::string& s ) 
{
  std::string ret( s );
  ret.erase(ret.begin(), std::find_if(ret.begin(), ret.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return ret;
}


std::string Nebulae::rtrim( const std::string& s ) 
{
  std::string ret( s );
  ret.erase(std::find_if(ret.rbegin(), ret.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), ret.end());
  return ret;
}


std::string Nebulae::trim( const std::string& s ) 
{
  return ltrim( rtrim(s) );
}