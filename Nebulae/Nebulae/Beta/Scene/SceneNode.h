#ifndef SCENENODE_H__
#define SCENENODE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

// Forward Deceleration
class Camera;
class Material;
class RenderQueue;
class RenderSystem;
class SceneGraph;
class SceneObject;

/** SceneNode.
 */
class SceneNode
{
private:
  std::string                m_strName;         ///< String used to identify node.
  SceneGraph*                m_pCreator;        ///< The SceneManager instance that created this.
  SceneNode*                 m_pParent;         ///< The node to which this one is attached to.
  std::vector<SceneNode* >   m_ChildNodes;      ///< List of nodes which are child nodes of this one.
  Matrix4                    m_LocalTransform;  ///< The cached local transform. Derived from the position, rotation & scale values.
  Vector4                    m_Position;        ///< Position of the node relative to its parent.
  Quaternion                 m_Rotation;        ///< Rotation of the node relative to its parent.    
  Vector4                    m_Scale;           ///< Scale of the node relative to its parent.
  bool                       m_bVisible;        ///< Manual flag to indicate rendering status
  std::vector<SceneObject* > m_Objects;         ///< An Array of objects that are to be rendered at the position of this node.

	public:
    SceneNode( SceneGraph* creator, const std::string& name = "" );
		~SceneNode();

		//getters
    bool              IsVisible() const;
    const Vector4&    GetPosition() const;
    const Quaternion& GetRotation() const;
    const Vector4&    GetScale() const;
		std::size_t       GetChildCount() const;
		SceneNode*        GetParent() const;
    SceneNode*        GetChild( std::size_t idx );
		const SceneNode*  GetChild( std::size_t idx ) const;		
    void              GetWorldMatrix( Matrix4* pWorldMatrixOut ) const;

		//functions
		void Clear();
    void SetName( const std::string& name );
    void SetVisible( bool bVisible );
    void SetPosition( const Vector4& position );
    void SetRotation( const Quaternion& rotation );
    void SetScale( const Vector4& scale );
    void SetParent( SceneNode* pParent );
    SceneNode* CreateChild( const std::string& strName );
    SceneNode* CreateChild();
    void RemoveChild( SceneNode* pNode );
    SceneObject* CreateObject( const Material* material );
    bool DetachObject( SceneObject* subObj );
    SceneObject* FindSubObject( const Material* material ) const;
    void FindVisibleObjects_( Camera* pCam, RenderQueue* pRenderQueue );

}; //SceneNode

} //Nebulae

#endif // SCENENODE_H__