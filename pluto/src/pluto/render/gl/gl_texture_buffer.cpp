#include "pluto/render/gl/gl_texture_buffer.h"
#include "pluto/asset/texture_asset.h"
#include "pluto/math/vector2i.h"

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
            glDeleteTextures(1, &textureBufferObjectId);
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& rhs) = delete;

        Impl& operator=(Impl&& rhs) noexcept = default;

        void Update(TextureAsset& textureAsset)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureBufferObjectId);

            const GLint wrap = WRAPS[static_cast<int>(textureAsset.GetWrap())];
            const GLint filter = FILTERS[static_cast<int>(textureAsset.GetFilter())];

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

            const GLint format = FORMATS[static_cast<int>(textureAsset.GetFormat())];

            const Vector2I& size = textureAsset.GetSize();

            glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE,
                         textureAsset.Data().data());

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Bind(const uint8_t location)
        {
            if (isBound)
            {
                return;
            }

            glActiveTexture(GL_TEXTURE0 + location);
            glBindTexture(GL_TEXTURE_2D, textureBufferObjectId);
            isBound = true;
            lastBindLocation = location;
        }

        void Unbind()
        {
            if (!isBound)
            {
                return;
            }

            glActiveTexture(GL_TEXTURE0 + lastBindLocation);
            glBindTexture(GL_TEXTURE_2D, 0);
            isBound = false;
        }
    };

    GlTextureBuffer::Factory::Factory(DiContainer& diContainer) : TextureBuffer::Factory(diContainer)
    {
    }

    std::unique_ptr<TextureBuffer> GlTextureBuffer::Factory::Create() const
    {
        GLuint textureBufferObject;
        glGenTextures(1, &textureBufferObject);
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
