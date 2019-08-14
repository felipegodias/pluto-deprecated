#pragma once

namespace pluto
{
    class FontAssetCompiler
    {
    public:
        ~FontAssetCompiler();
        FontAssetCompiler();

        FontAssetCompiler(const FontAssetCompiler& other) = delete;
        FontAssetCompiler(FontAssetCompiler&& other) noexcept;
        FontAssetCompiler& operator=(const FontAssetCompiler& rhs) = delete;
        FontAssetCompiler& operator=(FontAssetCompiler&& rhs) noexcept;

        void Create();
    };
}
