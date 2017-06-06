#ifndef NEBULAE_BETA_MATERIAL_H__
#define NEBULAE_BETA_MATERIAL_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

#include <Nebulae/Beta/Material/Pass.h>

namespace Nebulae
{
  
/** This is really a single 'Technique' which will control the appearance of a rendered object.
*/
class Material
{
private:
  std::string          m_name;               ///< Identifier of the material.
  std::vector<Pass* >  m_passes;             ///< List of Passes.
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
  
    /** Add a new UniformDefinition to this shared set of parameters.
    @remarks
      Unlike GpuProgramParameters, where the parameter list is defined by the
      program being compiled, this shared parameter set is defined by the
      user. Only parameters which have been predefined here may be later
      updated.
    */
    void AddUniformDefinition( const std::string& name, UniformType type, std::size_t arraySize = 1 );

    /** Remove a UniformDefinition from this shared set of parameters.
    */
    void RemoveUniformDefinition( const std::string& name );

    /** Remove a UniformDefinition from this shared set of parameters.
    */
    void RemoveAllUniformDefinitions();


}; //Material

} //Nebulae

#endif // NEBULAE_BETA_MATERIAL_H__