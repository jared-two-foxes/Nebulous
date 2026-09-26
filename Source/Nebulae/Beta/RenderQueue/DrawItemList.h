#ifndef NEBULAE_BETA_RENDERQUEUE_DRAWITEMLIST_H_
#define NEBULAE_BETA_RENDERQUEUE_DRAWITEMLIST_H_

#include <Nebulae/Beta/RenderQueue/DrawItem.h>
#include <Nebulae/Common/Common.h>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <map>

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

  void Clear() { m_Items.clear(); m_NodeWorlds.clear(); }

  void RecordNodeWorld( const SceneNode* node, const Matrix4& world ) { m_NodeWorlds[node] = world; }
  const Matrix4* GetNodeWorld( const SceneNode* node ) const
  {
    auto it = m_NodeWorlds.find( node );
    return it == m_NodeWorlds.end() ? nullptr : &it->second;
  }

  std::size_t Size() const { return m_Items.size(); }

  DrawItem& operator[]( std::size_t index ) { return m_Items[index]; }

  const DrawItem& operator[]( std::size_t index ) const { return m_Items[index]; }

private:
  std::vector<DrawItem> m_Items;
  std::map<const SceneNode*, Matrix4> m_NodeWorlds;
};

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_DRAWITEMLIST_H_
