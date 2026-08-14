#ifndef NEBULAE_BETA_RENDERQUEUE_DRAWITEMLIST_H_
#define NEBULAE_BETA_RENDERQUEUE_DRAWITEMLIST_H_

#include <Nebulae/Beta/RenderQueue/DrawItem.h>
#include <cstddef>
#include <vector>
#include <algorithm>

namespace Nebulae
{

class DrawItemList
{
public:
  void Add( const DrawItem& item ) { m_Items.push_back( item ); }

  void Sort()
  {
    std::stable_sort( m_Items.begin(), m_Items.end(),
                      []( const DrawItem& a, const DrawItem& b ) { return a.sortKey < b.sortKey; } );
  }

  void Clear() { m_Items.clear(); }

  std::size_t Size() const { return m_Items.size(); }

  DrawItem& operator[]( std::size_t index ) { return m_Items[index]; }

  const DrawItem& operator[]( std::size_t index ) const { return m_Items[index]; }

private:
  std::vector<DrawItem> m_Items;
};

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_DRAWITEMLIST_H_
