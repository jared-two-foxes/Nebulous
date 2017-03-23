#ifndef NEBULAE_ALPHA_UNIFORMPARAMETERS_H__
#define NEBULAE_ALPHA_UNIFORMPARAMETERS_H__

#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

namespace Nebulae {


/** UniformParameters. */
class UniformParameters
{
private:
  UniformDefinitionMap           m_definitions;
  std::vector<Real >             m_realUniformBuffer; /// Packed list of floating-point constants (physical indexing)
  std::vector<int32 >            m_intUniformBuffer;  /// Packed list of integer constants (physical indexing)
 
  public:
    UniformParameters();


    /** Get a specific UniformDefinition for a named parameter. */
    const UniformDefinition& GetUniformDefinition( const std::string& name ) const;

    /** Get the full list of UniformDefinition instances. */
    const UniformDefinitionMap& GetUniformDefinitions() const;
  
    /** Get a pointer to the 'nth' item in the float buffer. */
    float* GetFloatPointer( std::size_t pos );

    /** Get a pointer to the 'nth' item in the float buffer. */
    const float* GetFloatPointer( std::size_t pos ) const;

    /** Get a pointer to the 'nth' item in the int buffer. */
    int* GetIntPointer( std::size_t pos );

    /** Get a pointer to the 'nth' item in the int buffer. */
    const int* GetIntPointer( std::size_t pos ) const;



    /** Add a new UniformDefinition to this shared set of parameters.
    @remarks
      Unlike GpuProgramParameters, where the parameter list is defined by the
      program being compiled, this shared parameter set is defined by the
      user. Only parameters which have been predefined here may be later
      updated.
    */
    void AddUniformDefinition( const std::string& name, UniformType uniformType, std::size_t arraySize = 1 );

    /** Remove a UniformDefinition from this shared set of parameters.
    */
    void RemoveUniformDefinition( const std::string& name );

    /** Remove a UniformDefinition from this shared set of parameters.
    */
    void RemoveAllUniformDefinitions();


    /** Sets a single value constant floating-point parameter to the program.
		@remarks
		Different types of GPU programs support different types of constant parameters.
		For example, it's relatively common to find that vertex programs only support
		floating point constants, and that fragment programs only support integer (fixed point)
		parameters. This can vary depending on the program version supported by the
		graphics card being used. You should consult the documentation for the type of
		low level program you are using, or alternatively use the methods
		provided on RenderSystemCapabilities to determine the options.
		@par
		Another possible limitation is that some systems only allow constants to be set
		on certain boundaries, e.g. in sets of 4 values for example. Again, see
		RenderSystemCapabilities for full details.
		@note
		This named option will only work if you are using a parameters object created
		from a high-level program (HighLevelGpuProgram).
		@param name The name of the parameter
		@param val The value to set
		*/
		void SetNamedUniform( const std::string& name, Real val );

		/** Sets a single value constant integer parameter to the program.
		@remarks
		Different types of GPU programs support different types of constant parameters.
		For example, it's relatively common to find that vertex programs only support
		floating point constants, and that fragment programs only support integer (fixed point)
		parameters. This can vary depending on the program version supported by the
		graphics card being used. You should consult the documentation for the type of
		low level program you are using, or alternatively use the methods
		provided on RenderSystemCapabilities to determine the options.
		@par
		Another possible limitation is that some systems only allow constants to be set
		on certain boundaries, e.g. in sets of 4 values for example. Again, see
		RenderSystemCapabilities for full details.
		@note
		This named option will only work if you are using a parameters object created
		from a high-level program (HighLevelGpuProgram).
		@param name The name of the parameter
		@param val The value to set
		*/
		void SetNamedUniform( const std::string& name, int val );

		/** Sets a Vector4 parameter to the program.
		@param name The name of the parameter
		@param vec The value to set
		*/
		void SetNamedUniform( const std::string& name, const Vector4& vec );

		/** Sets a Colour parameter to the program.
		@param name The name of the parameter
		@param vec The value to set
		*/
		void SetNamedUniform( const std::string& name, const Colour& vec );

		/** Sets a Matrix4 parameter to the program.
		@param name The name of the parameter
		@param m The value to set
		*/
		void SetNamedUniform( const std::string& name, const Matrix4& m );

		/** Sets a list of Matrix4 parameters to the program.
		@param name The name of the parameter; this must be the first index of an array,
		for examples 'matrices[0]'
		NB since a Matrix4 is 16 floats long, so each entry will take up 4 indexes.
		@param m Pointer to an array of matrices to set
		@param numEntries Number of Matrix4 entries
		*/
		void SetNamedUniform( const std::string& name, const Matrix4* m, std::size_t numEntries );

		/** Sets a multiple value constant floating-point parameter to the program.
		@par
		Some systems only allow constants to be set on certain boundaries, 
		e.g. in sets of 4 values for example. The 'multiple' parameter allows
		you to control that although you should only change it if you know
		your chosen language supports that (at the time of writing, only
		GLSL allows constants which are not a multiple of 4).
		@note
		This named option will only work if you are using a parameters object created
		from a high-level program (HighLevelGpuProgram).
		@param name The name of the parameter
		@param val Pointer to the values to write
		@param count The number of 'multiples' of floats to write
		@param multiple The number of raw entries in each element to write, 
		the default is 4 so count = 1 would write 4 floats.
		*/
		void SetNamedUniform( const std::string& name, const float *val, std::size_t count );

		/** Sets a multiple value constant floating-point parameter to the program.
		@par
		Some systems only allow constants to be set on certain boundaries, 
		e.g. in sets of 4 values for example. The 'multiple' parameter allows
		you to control that although you should only change it if you know
		your chosen language supports that (at the time of writing, only
		GLSL allows constants which are not a multiple of 4).
		@note
		This named option will only work if you are using a parameters object created
		from a high-level program (HighLevelGpuProgram).
		@param name The name of the parameter
		@param val Pointer to the values to write
		@param count The number of 'multiples' of floats to write
		@param multiple The number of raw entries in each element to write, 
		the default is 4 so count = 1 would write 4 floats.
		*/
		void SetNamedUniform( const std::string& name, const double *val, std::size_t count );

		/** Sets a multiple value constant floating-point parameter to the program.
		@par
		Some systems only allow constants to be set on certain boundaries, 
		e.g. in sets of 4 values for example. The 'multiple' parameter allows
		you to control that although you should only change it if you know
		your chosen language supports that (at the time of writing, only
		GLSL allows constants which are not a multiple of 4).
		@note
		This named option will only work if you are using a parameters object created
		from a high-level program (HighLevelGpuProgram).
		@param name The name of the parameter
		@param val Pointer to the values to write
		@param count The number of 'multiples' of floats to write
		@param multiple The number of raw entries in each element to write, 
		the default is 4 so count = 1 would write 4 floats.
		*/
		void SetNamedUniform( const std::string& name, const int *val, std::size_t count );

}; 

} //Nebulae

#endif // NEBULAE_ALPHA_UNIFORMPARAMETERS_H__