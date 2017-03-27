#ifndef __NEBULAE_VARIANT_H__
#define __NEBULAE_VARIANT_H__

namespace Nebulae {
	
/** Variant
	*/
class Variant
{
public:
	enum Type {
		Unknown,
		String,
		Int,
		Float
	};

protected:
	Type m_Type;
	union {
		int m_iValue;
		float m_fValue;
		char* m_szValue;
	};

public:
	Variant();
	Variant( int iValue );
	Variant( float fValue );
	Variant( const char* szValue );
	Variant( const Variant& other );

	~Variant();

	Variant& operator= (const Variant& other);

	Type GetType() const;
		
	bool IsInt() const;
	bool IsFloat() const;
	bool IsString() const;
		
	int ToInt() const;
	float ToFloat() const;
	const char* ToString() const;

}; //Variant

} //Nebulae

#endif //__NEBULAE_VARIANT_H__