#include "font_asset_compiler.h"

namespace pluto
{
    FontAssetCompiler::~FontAssetCompiler() = default;

    FontAssetCompiler::FontAssetCompiler() = default;

    FontAssetCompiler::FontAssetCompiler(FontAssetCompiler&& other) noexcept = default;

    FontAssetCompiler& FontAssetCompiler::operator=(FontAssetCompiler&& rhs) noexcept = default;

    void FontAssetCompiler::Create()
    {
    }
}
