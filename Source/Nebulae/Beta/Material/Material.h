#ifndef NEBULAE_BETA_MATERIAL_MATERIAL_H_
#define NEBULAE_BETA_MATERIAL_MATERIAL_H_

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

#include <Nebulae/Beta/Material/Pass.h>

namespace Nebulae
{

class RenderSystem;

/** This is really a single 'Technique' which will control the appearance of a rendered object.
 */
class Material
{
private:
  std::string m_name;                        ///< Identifier of the material.
  std::vector<Pass*> m_passes;               ///< List of Passes.
  UniformDefinitionMap m_uniformDefinitions; ///< List of all of the uniforms registered for this material.

public:
  /** Constructor. */
  Material( const std::string& name );

  /** Destructor. */
  virtual ~Material();


  /** Returns the name of the material. */
  const std::string& GetName() const;

  /** Gets the indexed pass. */
  Pass* GetPass( std::size_t index ) const;

  /** Returns the number of Passes. */
  std::size_t GetPassCount() const;

  /** Returns the registered UniformDefinitions. */
  const UniformDefinitionMap& GetUniformDefinitions() const;


  /** Creates a new Pass for his Material. */
  Pass* CreatePass();

  /** Removes a Pass at a given index. */
  void RemovePass( unsigned short index );

  /** Removes all Passes from Material. */
  void RemoveAllPasses();

  void RefreshUniformSchemas( RenderSystem* renderSystem );

}; // Material

} // namespace Nebulae

#endif // NEBULAE_BETA_MATERIAL_MATERIAL_H_
