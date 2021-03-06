#ifndef __NEBULAE_PREFABFACTORY_H__
#define __NEBULAE_PREFABFACTORY_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

//Forward Decleration
class Geometry;
class HardwareBuffer;
class Resource;
class RenderSystem;

class PlaneLoader
{
private:
  RenderSystem& m_renderDevice;
	int           m_Rows;
	int           m_Columns;
	float         m_Width;
	float         m_Depth;

  public:
	  PlaneLoader( RenderSystem& renderDevice );
	  virtual ~PlaneLoader();

	  Geometry* Create( float width, float depth, int rows = 2, int columns = 2 );

  private:
	  static void UpdateVertexBuffer( HardwareBuffer* vbuf, int rows, int columns, float dx, float dz );
	  static void UpdateIndexBuffer( HardwareBuffer* ibuf, int rows, int columns );

}; //PlaneLoader

class CubeLoader
{
private:
  RenderSystem& m_renderDevice;
	int           m_Rows;
	int           m_Columns;
	int           m_Slices;
	float         m_Width;
	float         m_Height;
	float         m_Depth;

  public:
	  CubeLoader( RenderSystem& renderDevice );
	  virtual ~CubeLoader();

	  Geometry* Create( float width, float height, float depth, int rows = 2, int columns = 2, int slices = 2 );

  private:
	  static void UpdateVertexBuffer( HardwareBuffer* vbuf, int rows, int columns, int slices, float dx, float dy, float dz );
	  static void UpdateIndexBuffer( HardwareBuffer* ibuf, int rows, int columns, int slices );
	
}; //CubeLoader

class SphereLoader
{
private:
  RenderSystem& m_renderSystem;

  public:
    SphereLoader( RenderSystem& renderDevice );
	  Geometry* Create();

}; //SphereLoader

} //Nebulae

#endif // PREFABFACTOR_H__