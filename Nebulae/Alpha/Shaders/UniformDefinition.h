#ifndef NEBULAE_ALPHA_UNIFORMDEFINITION_H__
#define NEBULAE_ALPHA_UNIFORMDEFINITION_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  
/** Enumeration of the types of constant we may encounter in programs. */
enum UniformType
{
  UT_FLOAT1,
  UT_FLOAT2,
  UT_FLOAT3,
  UT_FLOAT4,
  UT_SAMPLER1D,
  UT_SAMPLER2D,
  UT_SAMPLER3D,
  UT_SAMPLERCUBE,
  UT_SAMPLER1DSHADOW,
  UT_SAMPLER2DSHADOW0,
  UT_SAMPLER2DARRAY1,

  UT_MATRIX_2X2,
  UT_MATRIX_2X3,
  UT_MATRIX_2X4,
  UT_MATRIX_3X2,
  UT_MATRIX_3X3,
  UT_MATRIX_3X4,
  UT_MATRIX_4X2,
  UT_MATRIX_4X3,
  UT_MATRIX_4X4,
  
  UT_INT1,
  UT_INT2,
  UT_INT3,
  UT_INT4,

  UT_UNKNOWN
};

/** UniformDefinition. */
struct UniformDefinition
{
  UniformType type;          ///< Data type.
  std::size_t logicalIndex;  ///< Logical index - used to communicate this constant to the rendersystem
  std::size_t physicalIndex; ///< Physical start index in buffer (either float or int buffer)
  std::size_t elementSize;   ///< Number of raw buffer slots per element.
  std::size_t arraySize;     ///< Length of array.

  UniformDefinition()
    : type(UT_UNKNOWN),
      logicalIndex(0),
      physicalIndex((std::numeric_limits<size_t>::max)()),
      elementSize(0),
      arraySize(1)
  {
  }
    
  static std::size_t GetElementSize( UniformType uniformType, bool padToMultiplesOf4 ) {
    if( padToMultiplesOf4 ) {
      switch( uniformType ) {
        case UT_FLOAT1:
        case UT_INT1:
        //case UT_SAMPLER1D:
        case UT_SAMPLER2D:
        //case UT_SAMPLER2DARRAY:
        //case UT_SAMPLER3D:
        case UT_SAMPLERCUBE:
        //case UT_SAMPLER1DSHADOW:
        //case UT_SAMPLER2DSHADOW:
        case UT_FLOAT2:
        case UT_INT2:
        case UT_FLOAT3:
        case UT_INT3:
        case UT_FLOAT4:
        case UT_INT4:
          return 4;
        case UT_MATRIX_2X2:
        //case UT_MATRIX_2X3:
        //case UT_MATRIX_2X4:
          return 8; // 2 float4s
        //case UT_MATRIX_3X2:
        case UT_MATRIX_3X3:
        //case UT_MATRIX_3X4:
          return 12; // 3 float4s
        //case UT_MATRIX_4X2:
        //case UT_MATRIX_4X3:
        case UT_MATRIX_4X4:
          return 16; // 4 float4s
        default:
          return 4;
      }
    } else {
      switch( uniformType ) {
        case UT_FLOAT1:
        case UT_INT1:
        //case UT_SAMPLER1D:
        case UT_SAMPLER2D:
        //case UT_SAMPLER2DARRAY:
        //case UT_SAMPLER3D:
        case UT_SAMPLERCUBE:
        //case UT_SAMPLER1DSHADOW:
        //case UT_SAMPLER2DSHADOW:
          return 1;
        case UT_FLOAT2:
        case UT_INT2:
          return 2;
        case UT_FLOAT3:
        case UT_INT3:
          return 3;
        case UT_FLOAT4:
        case UT_INT4:
        case UT_MATRIX_2X2:
          return 4;
        //case UT_MATRIX_2X3:
        //case UT_MATRIX_3X2:
          return 6;
        //case UT_MATRIX_2X4:
        //case UT_MATRIX_4X2:
          return 8; 
        case UT_MATRIX_3X3:
          return 9;
        //case UT_MATRIX_3X4:
        //case UT_MATRIX_4X3:
        //  return 12; 
        case UT_MATRIX_4X4:
          return 16; 
        default:
          return 4;
      }
    }
  }


  static bool IsFloat( UniformType uniformType ) {
    switch( uniformType ) {
    case UT_INT1:
    case UT_INT2:
    case UT_INT3:
    case UT_INT4:
    case UT_SAMPLER1D:
    case UT_SAMPLER2D:
    case UT_SAMPLER3D:
    case UT_SAMPLERCUBE:
    case UT_SAMPLER1DSHADOW:
      return false;
    default:
      return true;
    }
  }

  bool IsFloat() const {
    return IsFloat( type );
  }

  bool IsSampler() const 
  {
    switch( type )
    {
    case UT_SAMPLER1D:
    case UT_SAMPLER2D:
    case UT_SAMPLER3D:
    case UT_SAMPLERCUBE:
    case UT_SAMPLER1DSHADOW:
      return true;
    default:
      return false;
    } 
  }

};

typedef std::map<std::string, UniformDefinition> UniformDefinitionMap;

} //Nebulae

#endif // NEBULAE_ALPHA_UNIFORMDEFINITION_H__