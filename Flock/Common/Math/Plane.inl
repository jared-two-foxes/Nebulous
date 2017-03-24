
inline float Distance( const plane_t& plane, const vector4_t& pt )
{
	return Dot3( plane.p, pt ) + plane.p.w;
}