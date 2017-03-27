#ifndef NEBULAE_BETA_FREETYPEHELPER_H__
#define NEBULAE_BETA_FREETYPEHELPER_H__

#include <Nebulae/Common/Common.h>

struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;
typedef int                  FT_Error;

namespace Nebulae {
namespace FT {

struct FontFaceProxy
{
private:
  FT_Face m_face;

  public:
    FontFaceProxy();
    ~FontFaceProxy();

    FT_Error GetFace( const char* filename );
    FT_Error GetFace( File& is );
    bool SetCharSize( int size );
    
    int    GetHeight() const;
    int    GetAscent() const;
    int    GetDecent() const;
    int    GetLineSkip() const;
    double GetUnderlineOffset() const;
    double GetUnderlineHeight() const;
    double GetItalicOffset() const;


    bool GenerateGlyph( boost::uint32_t ch );
    int  GetGlyphTop() const;
    int  GetGlyphWidth() const; 
    int  GetGlyphHeight() const; 
    int  GetGlyphLeftBearing() const;
    int  GetGlyphVerticalBearing() const;
    int  GetGlyphAdvance() const;
    
    
    uint8* GetBitmapBuffer() const;
    int    GetBitmapPitch() const;
    int    GetBitmapHeight() const;
    int    GetBitmapWidth() const;

  private:
    void CheckFace( FT_Error error );

};

} }

#endif // NEBULAE_BETA_FREETYPEHELPER_H__
