#pragma once

#include "pluto/api.h"

namespace pluto
{
    class Vector2F;

    class PLUTO_API Physics2DShape
    {
    protected:
        class Impl;

    private:
        Impl* impl;

    public:
        virtual ~Physics2DShape() = 0;
        explicit Physics2DShape(Impl& impl);

        Physics2DShape(const Physics2DShape& other) = delete;
        Physics2DShape(Physics2DShape&& other) noexcept;
        Physics2DShape& operator=(const Physics2DShape& other) = delete;
        Physics2DShape& operator=(Physics2DShape&& other) noexcept;

        float GetFriction() const;
        void SetFriction(float value);

        float GetRestitution() const;
        void SetRestitution(float value);

        bool IsTrigger() const;
        void SetTrigger(bool value);

        virtual Vector2F GetOffset() const = 0;
        virtual void SetOffset(const Vector2F& value) = 0;
    };
}
