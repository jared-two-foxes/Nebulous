#include <Nebulae/Alpha/RenderStream/RenderStream.h>


namespace Nebulae
{


void
RenderStream::Clear()
{
  // Reset size to zero but retain allocated capacity.
  m_data.clear();
}


} // namespace Nebulae
