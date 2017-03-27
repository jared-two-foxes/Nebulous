
#include <Nebulae/Common/Common.h>

using namespace Nebulae;

PixelBox::PixelBox( const Box &extents, PixelFormat pixelFormat, void *pixelData )
  : Box(extents), 
    data(pixelData), 
    format(pixelFormat) {
  setConsecutive(); }


PixelBox::PixelBox( std::size_t width, std::size_t height, std::size_t depth, PixelFormat pixelFormat, void *pixelData )
  : Box(0, 0, 0, width, height, depth),
    data(pixelData), 
    format(pixelFormat) {
  setConsecutive(); }


void 
PixelBox::setConsecutive() {
  rowPitch   = getWidth();
  slicePitch = getWidth()*getHeight();
}


std::size_t 
PixelBox::getRowSkip() const { 
  return rowPitch - getWidth(); }


std::size_t 
PixelBox::getSliceSkip() const { 
  return slicePitch - (getHeight() * rowPitch); }


bool 
PixelBox::isConsecutive() const { 
  return rowPitch == getWidth() && slicePitch == getWidth()*getHeight(); }