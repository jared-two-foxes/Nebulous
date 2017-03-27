
#include <Nebulae/Common/Common.h>

namespace Nebulae
{
	//constructor
	Variant::Variant()
		: m_Type( Variant::Unknown )
		, m_iValue( -1 )
	{
	}

	//constructor
	Variant::Variant( int iValue )
		: m_Type( Variant::Int )
		, m_iValue( iValue )
	{
	}

	//constructor
	Variant::Variant( float fValue )
		: m_Type( Variant::Float )
		, m_fValue( fValue )
	{
	}

	//constructor
	Variant::Variant( const char* szValue )
		: m_Type( Variant::String )
	{
		std::size_t iLen = strlen( szValue );
		m_szValue = new char[ iLen+1 ];
		strcpy( m_szValue, szValue );
	}

	//constructor
	Variant::Variant( const Variant& other )
		: m_Type( other.m_Type )
	{
		switch (m_Type)
		{
		case Variant::String:
			{
				std::size_t iLen = strlen( other.m_szValue );
				m_szValue = new char[ iLen+1 ];
				strcpy( m_szValue, other.m_szValue );
			}
			break;
		case Variant::Int:
			m_iValue = other.m_iValue;
			break;
		case Variant::Float:
			m_fValue = other.m_fValue;
			break;
		}
	}

	//destructor
	Variant::~Variant()
	{
		if (IsString())
		{
			delete [] m_szValue;
		}
	}

	// assignment operator.
	Variant& Variant::operator= (const Variant& other)
	{
    if( &other != this )
    {
		  m_Type = other.m_Type;

		  switch (m_Type)
		  {
		  case Variant::String:
			  {
				  std::size_t iLen = strlen( other.m_szValue );
				  m_szValue = new char[ iLen+1 ];
				  strcpy( m_szValue, other.m_szValue );
			  }
			  break;
		  case Variant::Int:
			  m_iValue = other.m_iValue;
			  break;
		  case Variant::Float:
			  m_fValue = other.m_fValue;
			  break;
		  }
    }

	  return *this;
	}

	//GetType
	Variant::Type Variant::GetType() const
	{
		return m_Type;
	}
		
	//IsInt
	bool Variant::IsInt() const
	{
		return (m_Type == Variant::Int);
	}

	//IsFloat
	bool Variant::IsFloat() const
	{
		return (m_Type == Variant::Float);
	}

	//IsString
	bool Variant::IsString() const
	{
		return (m_Type == Variant::String);
	}
		
	//ToInt
	int Variant::ToInt() const
	{
		NE_ASSERT( m_Type == Variant::Int, "Expected type Int" )( m_Type );
		return m_iValue;
	}

	//ToFloat
	float Variant::ToFloat() const
	{
    NE_ASSERT( m_Type == Variant::Float, "Expected type Float" )( m_Type );
		return m_fValue;
	}

	//ToString
	const char* Variant::ToString() const
	{
    NE_ASSERT( m_Type == Variant::Int, "Expected type String" )( m_Type );
		return m_szValue;
	}
}