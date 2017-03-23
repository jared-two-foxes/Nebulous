/** \file Box.h \brief Contains the utility class Box, which represents a simple
    box in Nebulae. */

#ifndef __NEBULAE_COMMON_BOX_H__
#define __NEBULAE_COMMON_BOX_H__


namespace Nebulae {

/** Structure used to define a box in a 3-D integer space.
    Note that the left, top, and front edges are included but the right, 
    bottom and back ones are not.
  */
struct Box
{
  std::size_t left, top, right, bottom, front, back;
    
  /// Parameterless constructor for setting the members manually
  Box() : left(0), top(0), right(1), bottom(1), front(0), back(1)
  {
  }

  /** Define a box from left, top, right and bottom coordinates
    This box will have depth one (front=0 and back=1).
    @param  l x value of left edge
    @param  t y value of top edge
    @param  r x value of right edge
    @param  b y value of bottom edge
    @note Note that the left, top, and front edges are included 
      but the right, bottom and back ones are not.
  */
  Box( std::size_t l, std::size_t t, std::size_t r, std::size_t b ):
      left(l),
      top(t),   
      right(r),
      bottom(b),
      front(0),
      back(1)
  {
    NE_ASSERT(right >= left && bottom >= top && back >= front, "Invalid box shape" )( right, left, top, bottom, front, back );
  }
    
  /** Define a box from left, top, front, right, bottom and back
    coordinates.
    @param  l x value of left edge
    @param  t y value of top edge
    @param  ff  z value of front edge
    @param  r x value of right edge
    @param  b y value of bottom edge
    @param  bb  z value of back edge
    @note Note that the left, top, and front edges are included 
      but the right, bottom and back ones are not.
  */
  Box( std::size_t l, std::size_t t, std::size_t ff, std::size_t r, std::size_t b, std::size_t bb ):
      left(l),
      top(t),   
      right(r),
      bottom(b),
      front(ff),
      back(bb)
  {
    NE_ASSERT(right >= left && bottom >= top && back >= front, "Invalid box shape" )( right, left, top, bottom, front, back );
  }
    
  /// Return true if the other box is a part of this one
  bool contains( const Box &def ) const
  {
    return (def.left >= left && def.top >= top && def.front >= front &&
            def.right <= right && def.bottom <= bottom && def.back <= back);
  }
    
  /// Get the width of this box
  std::size_t getWidth() const { return right-left; }

  /// Get the height of this box
  std::size_t getHeight() const { return bottom-top; }

  /// Get the depth of this box
  std::size_t getDepth() const { return back-front; }
};

}

#endif // __NEBULAE_COMMON_BOX_H__
