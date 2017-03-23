/** \file ptRect.h \brief Contains the utility classes pt and Rect. */

#ifndef _NEBULAE_COMMON_POINT_RECT_h_
#define _NEBULAE_COMMON_POINT_RECT_h_


namespace Nebulae 
{

struct Point
{
  Point() {}
  Point( int x_, int y_ ) : x( x_ ), y( y_ ) {}
		
	/** Returns true if x < \a rhs.x or returns true if x == \a rhs.x and y
	 *	<\a rhs.y.  This is useful for sorting pts in STL containers and
	 *	algorithms. 
	 */
	bool Less( const Point& rhs ) const
	{ 
		return x < rhs.x ? true : (x == rhs.x ? (y < rhs.y ? true : false) : false); 
	}
		
	void operator+=( const Point& rhs ) { x += rhs.x; y += rhs.y; } ///< Adds \a rhs to pt.
	void operator-=( const Point& rhs ) { x -= rhs.x; y -= rhs.y; } ///< Subtracts \a rhs to pt.
	Point operator-() const { return Point(-x, -y); }               ///< Negates pt.
		
	int x; ///< The x component.
	int y; ///< The y component.

};


/** \brief A GG rectangle class.
 *
 *  This is essentially just two pts that bound the rectangle. 
 */
struct Rect
{
	/** \name Structors */ ///@{
	Rect();                                                    ///< default ctor
	Rect( const Point& pt1, const Point& pt2 );                ///< ctor that constructs a Rect from two corners; any two opposing corners will do
	Rect( int x1, int y1, int x2, int y2 );                    ///< ctor that constructs a Rect from its left, upper, right, and bottom boundaries
	//@}

	/** \name Accessors */ ///@{
	int   GetLeft() const         { return ul.x; }             ///< returns the left boundary of the Rect
	int   GetRight() const        { return lr.x; }             ///< returns the right boundary of the Rect
	int   GetTop() const          { return ul.y; }             ///< returns the top boundary of the Rect
	int   GetBottom() const       { return lr.y; }             ///< returns the bottom boundary of the Rect
	Point GetUpperLeft() const    { return ul; }               ///< returns the upper-left corner of the Rect
	Point GetLowerRight() const   { return lr; }               ///< returns the lower-right corner of the Rect
	int   GetWidth() const        { return lr.x - ul.x; }      ///< returns the width of the Rect
	int   GetHeight() const       { return lr.y - ul.y; }      ///< returns the height of the Rect
	bool  Contains( const Point& pt ) const;                   ///< returns true iff \a pt falls inside the Rect
	//@}

	/** \name Mutators */ ///@{
	void operator+=( const Point& pt ) { ul += pt; lr += pt; } ///< shifts the Rect by adding \a pt to each corner
	void operator-=( const Point& pt ) { ul -= pt; lr -= pt; } ///< shifts the Rect by subtracting \a pt from each corner
	//@}

	Point ul; ///< the upper-left corner of the Rect
	Point lr; ///< the lower-right corner of the Rect

};

inline Rect::Rect(){}  
inline Rect::Rect( const Point& pt1, const Point& pt2 ) : ul(pt1), lr(pt2) {}
inline Rect::Rect( int x1, int y1, int x2, int y2 ) : ul(x1,y1), lr(x2,y2) {}

inline bool operator==( const Point& lhs, const Point& rhs )    { return lhs.x == rhs.x && lhs.y == rhs.y; }    ///< returns true if \a lhs is identical to \a rhs
inline bool operator!=( const Point& lhs, const Point& rhs )    { return !(lhs == rhs); }                       ///< returns true if \a lhs differs from \a rhs
inline bool operator<( const Point& lhs, const Point& rhs )     { return lhs.x < rhs.x && lhs.y < rhs.y; }      ///< returns true if \a lhs.x and \a lhs.y are both less than the corresponding components of \a rhs
inline bool operator>( const Point& lhs, const Point& rhs )     { return lhs.x > rhs.x && lhs.y > rhs.y; }      ///< returns true if \a lhs.x and \a lhs.y are both greater than the corresponding components of \a rhs
inline bool operator<=( const Point& lhs, const Point& rhs )    { return lhs.x <= rhs.x && lhs.y <= rhs.y; }    ///< returns true if \a lhs.x and \a lhs.y are both less than or equal to the corresponding components of \a rhs
inline bool operator>=( const Point& lhs, const Point& rhs )    { return lhs.x >= rhs.x && lhs.y >= rhs.y; }    ///< returns true if \a lhs.x and \a lhs.y are both greater than or equal to the corresponding components of \a rhs
inline Point   operator+( const Point& lhs, const Point& rhs )  { return Point(lhs.x + rhs.x, lhs.y + rhs.y); } ///< returns the vector sum of \a lhs and \a rhs
inline Point   operator-( const Point& lhs, const Point& rhs )  { return Point(lhs.x - rhs.x, lhs.y - rhs.y); } ///< returns the vector difference of \a lhs and \a rhs

/** returns true if \a lhs is identical to \a rhs */
inline bool operator==( const Rect& lhs, const Rect& rhs ) { return lhs.ul.x == rhs.ul.x && lhs.lr.x == rhs.lr.x && lhs.lr.x == rhs.lr.x && lhs.lr.y == rhs.lr.y; }

/** returns true if \a lhs differs from \a rhs */
inline bool operator!=( const Rect& lhs, const Rect& rhs ) { return !(lhs == rhs); }

inline Rect operator+( const Rect& rect, const Point& pt ) { return Rect(rect.ul + pt, rect.lr + pt); } ///< returns \a rect shifted by adding \a pt to each corner
inline Rect operator-( const Rect& rect, const Point& pt ) { return Rect(rect.ul - pt, rect.lr - pt); } ///< returns \a rect shifted by subtracting \a pt from each corner
inline Rect operator+( const Point& pt, const Rect& rect ) { return rect + pt; } ///< returns \a rect shifted by adding \a pt to each corner
inline Rect operator-( const Point& pt, const Rect& rect ) { return rect - pt; } ///< returns \a rect shifted by subtracting \a pt from each corner

} // namepace Nebulae

#endif //_NEBULAE_pt_RECT_h_
