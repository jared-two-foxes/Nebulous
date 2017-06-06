#ifndef NEBULAE_BETA_SPRITEBATCH_H__
#define NEBULAE_BETA_SPRITEBATCH_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  class Texture;
  class SubTexture;
  class RenderSystem;
  class Material;

  struct GlyphState
  {
    Point             upperLeft;
    Point             lowerRight;
    Colour            colour;
    const SubTexture* sub_texture;
    Real              depth;
    Real              rotation;
    

    GlyphState()
    : upperLeft(0,0), 
      lowerRight(0,0),
      colour(CLR_WHITE),
      sub_texture(nullptr),
      rotation(0)
    {}

    GlyphState( const Point& ul, const Point& lr ) 
    : upperLeft(ul), 
      lowerRight(lr),
      colour(CLR_WHITE),
      sub_texture(nullptr),
      rotation(0)
    {}

    GlyphState( GlyphState&& other )
    {
      upperLeft  = other.upperLeft;
      lowerRight = other.lowerRight;
      colour     = other.colour;
      sub_texture = other.sub_texture;

      other.sub_texture = nullptr;
    }

  };

  class SpriteBatch
  ///
  /// 
  ///
  {
  public:
    #define MAX_VERTICES_PER_BUFFER 2048

  public:
    struct QuadVertex
    {
      QuadVertex() {}

      QuadVertex( const Point& point, float depth, const Colour& color, const Vector4& textureCoordinates )
        : position( point.x, point.y, -10/*depth*/, 1.0f ),
        colour( color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f ),
        textureCoordinates( textureCoordinates )
      {}

      QuadVertex( QuadVertex&& other ) = default;
      QuadVertex& operator =( QuadVertex&& other ) = default;
      QuadVertex( const QuadVertex& a ) = default;
      QuadVertex& operator =( const QuadVertex& a ) = default;

      Vector4 position;
      Vector4 colour;
      Vector4 textureCoordinates;
    };

    struct VertexArray
    /// This needs to be a linked list of VertexList.
    {
      std::array<QuadVertex, MAX_VERTICES_PER_BUFFER > vertices;
      std::size_t currentIndex;
      int32       remaining;

      VertexArray() : currentIndex(0), remaining(MAX_VERTICES_PER_BUFFER) {}

      void Clear() { currentIndex = 0; remaining = MAX_VERTICES_PER_BUFFER; }
      const std::size_t GetUsed() const { return currentIndex; } 

      void AddVertex( const QuadVertex& vertex )
      {
        memcpy( &vertices[currentIndex++], &vertex, sizeof(QuadVertex) );
        --remaining;
      }

      void AddVertex( QuadVertex&& vertex )
      {
        vertices[currentIndex++] = std::move(vertex);
        --remaining;
      }

    };

  private:
    typedef std::shared_ptr<RenderSystem >                      RenderSystemPtr;
    typedef std::unordered_map<Nebulae::Texture*, VertexArray > QuadMap; 

  private:
    RenderSystemPtr m_renderDevice;
    Material*       m_material; //< not good really.
    QuadMap         m_quads;

    public:
      SpriteBatch( const RenderSystemPtr& m_renderSystem ); //< This should take the window not the device?

      bool Init();
      
      void AddQuad( GlyphState* state, std::size_t count );
      void AddQuad( const Nebulae::Point& upperLeft, const Nebulae::Point& lowerRight,
        const Colour& colour = CLR_WHITE, const SubTexture* texture = NULL,
        Real depth = 0.0f, Real rotation = 0.0f );

      void Flush();
      void Draw();
      
  };

}

#endif // NEBULAE_BETA_SPRITEBATCH_H__