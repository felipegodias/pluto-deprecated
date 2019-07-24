#include "pluto/render/gl/gl_texture_buffer.h"
#include "pluto/asset/texture_asset.h"
#include "pluto/math/vector2i.h"

#include "pluto/render/gl/gl_call.h"

#include <GL/glew.h>

#include <array>

namespace pluto
{
    static const std::array<GLint, static_cast<int>(TextureAsset::Wrap::Count)> WRAPS{
        GL_CLAMP_TO_BORDER,
        GL_MIRRORED_REPEAT,
        GL_REPEAT,
    };

    static const std::array<GLint, static_cast<int>(TextureAsset::Format::Count)> FORMATS{
        GL_ALPHA,
        GL_RGB,
        GL_RGBA,
    };

    static const std::array<GLint, static_cast<int>(TextureAsset::Filter::Count)> FILTERS{
        GL_NEAREST,
        GL_LINEAR,
    };

    class GlTextureBuffer::Impl
    {
        GLuint textureBufferObjectId;
        bool isBound;
        uint8_t lastBindLocation;

    public:
        explicit Impl(const GLuint textureBufferObjectId) : textureBufferObjectId(textureBufferObjectId),
                                                            isBound(false), lastBindLocation(0)

        {
        }

        ~Impl()
        {
            GL_CALL(glDeleteTextures(1, &textureBufferObjectId));
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& rhs) = delete;

        Impl& operator=(Impl&& rhs) noexcept = default;

        void Update(TextureAsset& textureAsset)
        {
            GL_CALL(glActiveTexture(GL_TEXTURE0));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, textureBufferObjectId));

            const GLint wrap = WRAPS[static_cast<int>(textureAsset.GetWrap())];
            const GLint filter = FILTERS[static_cast<int>(textureAsset.GetFilter())];

            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));

            const GLint format = FORMATS[static_cast<int>(textureAsset.GetFormat())];

            const Vector2I& size = textureAsset.GetSize();

            std::vector<uint8_t> data = textureAsset.Data();

            int channelCount = 0;
            switch (textureAsset.GetFormat())
            {
            case TextureAsset::Format::Alpha8:
                channelCount = 1;
                break;
            case TextureAsset::Format::RGB24:
                channelCount = 3;
                break;
            case TextureAsset::Format::RGBA32:
                channelCount = 4;
                break;
            default: ;
            }

            const int bytesPerLine = size.x * channelCount;
            const int halfHeight = size.y / 2;
            for (int i = 0; i < bytesPerLine; ++i)
            {
                for (int j = 0; j < halfHeight; ++j)
                {
                    const int topDownIndex = j * bytesPerLine + i;
                    const int bottomUpIndex = (size.y - j - 1) * bytesPerLine + i;
                    std::swap(data[topDownIndex], data[bottomUpIndex]);
                }
            }

            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, data.data()));

            GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        }

        void Bind(const uint8_t location)
        {
            if (isBound)
            {
                return;
            }

            GL_CALL(glActiveTexture(GL_TEXTURE0 + location));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, textureBufferObjectId));

            isBound = true;
            lastBindLocation = location;
        }

        void Unbind()
        {
            if (!isBound)
            {
                return;
            }

            GL_CALL(glActiveTexture(GL_TEXTURE0 + lastBindLocation));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
            isBound = false;
        }
    };

    GlTextureBuffer::Factory::Factory(DiContainer& diContainer) : TextureBuffer::Factory(diContainer)
    {
    }

    std::unique_ptr<TextureBuffer> GlTextureBuffer::Factory::Create() const
    {
        GLuint textureBufferObject;
        GL_CALL(glGenTextures(1, &textureBufferObject));
        return std::make_unique<GlTextureBuffer>(std::make_unique<Impl>(textureBufferObject));
    }

    GlTextureBuffer::GlTextureBuffer(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    GlTextureBuffer::~GlTextureBuffer() = default;

    GlTextureBuffer::GlTextureBuffer(GlTextureBuffer&& other) noexcept = default;

    GlTextureBuffer& GlTextureBuffer::operator=(GlTextureBuffer&& rhs) noexcept = default;

    void GlTextureBuffer::Update(TextureAsset& textureAsset)
    {
        impl->Update(textureAsset);
    }

    void GlTextureBuffer::Bind(const uint8_t location)
    {
        impl->Bind(location);
    }

    void GlTextureBuffer::Unbind()
    {
        impl->Unbind();
    }
}
