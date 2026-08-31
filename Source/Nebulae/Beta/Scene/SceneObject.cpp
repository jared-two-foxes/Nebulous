
#include "SceneObject.h"
#include "SceneNode.h"

#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/Scene/Geometry.h>


using namespace Nebulae;


int SceneObject::ms_nextIdentifier = 0;

SceneObject::SceneObject( SceneNode* parent ) : m_identifier( ms_nextIdentifier++ ), m_node( parent ), m_visible( true )
{
}


SceneObject::~SceneObject() { m_node = nullptr; }


int SceneObject::GetIdentifier() const { return m_identifier; }


SceneNode* SceneObject::GetNode() const { return m_node; }


bool SceneObject::IsVisible() const { return m_visible; }


std::size_t SceneObject::AddSlot( const Material* material )
{
  m_slots.push_back( RenderSlot{ material } );
  return m_slots.size() - 1;
}


std::size_t SceneObject::GetSlotCount() const { return m_slots.size(); }


const RenderSlot& SceneObject::GetSlot( std::size_t index ) const { return m_slots[index]; }


void SceneObject::SetVisible( bool bVisible ) { m_visible = bVisible; }


void SceneObject::AddProvider( const std::string& key, UniformProvider provider )
{
  for ( auto& slot : m_slots )
  {
    auto it = std::find_if( slot.providers.begin(), slot.providers.end(),
                            [&key]( const std::pair<std::string, UniformProvider>& p ) { return p.first == key; } );
    if ( it != slot.providers.end() )
    {
      it->second = provider; // Update existing provider
    }
    else
    {
      slot.providers.emplace_back( key, provider ); // Add new provider
    }
  }
}

void SceneObject::EmitDrawItems( DrawItemList& items, int layer, int depth )
{
  for ( const auto& slot : m_slots )
  {
    if ( slot.material == nullptr )
    {
      continue; // Skip if no material assigned
    }

    for ( std::size_t p = 0, n = slot.material->GetPassCount(); p < n; ++p )
    {
      const Pass* pass = slot.material->GetPass( p );
      assert( pass != nullptr );

      int sortKey = MakeSortKey( pass, layer );

      DrawItem item;
      item.sortKey = sortKey;
      item.submissionOrder = static_cast<int>( items.Size() );
      items.Add( item );
    }
  }
}


void SceneObject::SetSlotGeometry( std::size_t slotIndex, Geometry* geometry )
{
  if ( slotIndex < m_slots.size() )
  {
    m_slots[slotIndex].geometry = geometry;
  }
}


void SceneObject::SetSlotInputLayout( std::size_t slotIndex, InputLayout* inputLayout )
{
  if ( slotIndex < m_slots.size() )
  {
    m_slots[slotIndex].inputLayout = inputLayout;
  }
}
