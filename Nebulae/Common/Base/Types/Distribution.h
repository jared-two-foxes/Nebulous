#ifndef __NEBULAE_COMMON_BASE_DISTRIBUTION_H__
#define __NEBULAE_COMMON_BASE_DISTRIBUTION_H__

namespace Nebulae {

template <class T>
class ConstantDistribution
{
private:
  T m_value; //constant

  public:
    ConstantDistribution() {}
    ConstantDistribution( const T& value )
    : m_value(value) {}

    void Set( const T& value )
    {
      m_value = value;
    }

    const T Value() const
    {
      return m_value;
    }

};


template <class T>
class UniformDistribution
{ 
private:
  T m_min;
  T m_max;

  public:
    UniformDistribution() {}
  
    UniformDistribution( const T& min, const T& max )
    : m_min( min ),
      m_max( max )
    {}

    void Set( const T& min, const T& max )
    {
      m_min = min;
      m_max = max;
    }

    const T Value() const
    {
      return m_min + ((float(rand()) / RAND_MAX) * (m_max - m_min));
    }

};


template <class T>
class ConstantCurveDistribution
{
public:
  struct CurvePoint
  {
    T in;               ///< this is the location of the point along the graphs horizontal axis, generally calculated as time.
    T out;              ///< this is the location of the point along the graph's vertical axis generally calculated as property value.
    T arrive_tangent;   ///< Provides the angle of the incoming tangent for the point.  Useful values range from about -150 to 150.
    T leave_tangent;    ///< Provides the angle of the outgoing tangent for the point.  Useful values range from about -150 to 150.
    //interp mode.      ///< Allows the user to choose between a variety of interpolation modes for the curve. 
  };

private:
  std::vector<CurvePoint > m_points;

  public:
    ConstantCurveDistribution() {}

    const T Value() const
    {
      return std::numeric_limits<T>::min();
    }

};


template <class T>
class Distribution
{
private:
  enum class Type {
    CONSTANT, 
    UNIFORM, 
    CURVE
  };

private:
  ConstantDistribution<T >      m_constant_distribution;
  UniformDistribution<T >       m_uniform_distribution;
  //ConstantCurveDistribution<T > m_constant_curve_distribution;
  Type                          m_distribution_type;

  public:
    Distribution() {}
    
    void SetConstant( const T& value ) 
    {
      m_constant_distribution.Set( value );
      m_distribution_type = Type::CONSTANT;
    }

    void SetUniform( const T& min, const T& max ) 
    {
      m_uniform_distribution.Set( min, max );
      m_distribution_type = Type::UNIFORM;
    }

    virtual const T Value() const
    {
      switch( m_distribution_type )
      {
      case Type::CONSTANT:
        return m_constant_distribution.Value();
      case Type::UNIFORM:
        return m_uniform_distribution.Value();
      //case Type::CURVE:
      //  return m_constant_curve_distribution.Value();
      }

      NE_ASSERT( false, "Unknown distribution type found" )();
      return m_constant_distribution.Value();
    }

};


} // Nebulae

#endif // __NEBULAE_COMMON_BASE_DISTRIBUTION_H__