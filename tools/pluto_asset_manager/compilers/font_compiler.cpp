#include "font_compiler.h"

#include <pluto/file/file_reader.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>

#include <array>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace pluto::compiler
{
    FontCompiler::FontCompiler(FontAsset::Factory& fontAssetFactory)
        : fontAssetFactory(&fontAssetFactory)
    {
    }

    std::vector<std::string> FontCompiler::GetExtensions() const
    {
        return {".ttf"};
    }

    std::vector<BaseCompiler::CompiledAsset> FontCompiler::Compile(const std::string& input,
                                                                   const std::string& outputDir) const
    {
        /*
        std::unique_ptr<FileReader> reader = fileManager->OpenRead(path);
        std::vector<unsigned char> bytes(reader->GetSize());
        reader->Read(bytes.data(), bytes.size());

        const char first = 32; // Space;
        const char last = 127; // Del
        const float fontSize = 64;
        const uint16_t bitmapWidth = 512;
        const uint16_t bitmapHeight = 512;

        std::array<stbtt_bakedchar, last - first + 1> bakedChars{};
        std::vector<uint8_t> bitmap(bitmapWidth *   bitmapHeight ) ;
          stbtt_BakeFontBitmap ( bytes . data ( ) ,   0 ,   fontSize ,   bitmap . data ( ) ,   bitmapWidth ,
        bitmapHeight ,   first ,
              bakedChars . size ( ) ,
              bakedChars . data ( ) ) ;

          std :: vector < FontAsset :: Glyph >   glyphs ( bakedChars . size ( ) ) ;
          for   ( size_t   i   =   0 ;   i   <   glyphs . size ( ) ;   ++ i )
          {
              glyphs [ i ] . character   =   static_cast < char > ( first   +   i ) ;
              glyphs [ i ] . xMin   =   bakedChars [ i ] . x0 ;
              glyphs [ i ] . yMin   =   bakedChars [ i ] . y0 ;
              glyphs [ i ] . xMax   =   bakedChars [ i ] . x1 ;
              glyphs [ i ] . yMax   =   bakedChars [ i ] . y1 ;
              glyphs [ i ] . xBearing   =   bakedChars [ i ] . xoff ;
              glyphs [ i ] . yBearing   =   bakedChars [ i ] . yoff ;
              glyphs [ i ] . advance   =   bakedChars [ i ] . xadvance ;
          }

          std :: unique_ptr < FontAsset >   fontAsset   =   fontAssetFactory -> Create ( fontSize ,   glyphs ,
        bitmapWidth ,   bitmapHeight ,
              bitmap ) ;
          return   fontAsset ;
        */
        std::vector<CompiledAsset> assets;
        return assets;
    }
}
