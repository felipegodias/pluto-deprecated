#pragma once

#include "pluto/api.h"

namespace pluto
{
    class Vector2F;

    class PLUTO_API Physics2DBody
    {
    public:
        enum class Type
        {
            Static = 0,
            Kinematic = 1,
            Dynamic = 2,
            Default = Static
        };

    protected:
        class Impl;

    private:
        Impl* impl;

    public:
        virtual ~Physics2DBody() = 0;

        Physics2DBody(Impl& impl);

        Physics2DBody(const Physics2DBody& other) = delete;
        Physics2DBody(Physics2DBody&& other) noexcept;
        Physics2DBody& operator=(const Physics2DBody& other) = delete;
        Physics2DBody& operator=(Physics2DBody&& other) noexcept;

        Type GetType(Type type) const;
        void SetType(Type value);

        Vector2F GetPosition() const;
        void SetPosition(const Vector2F& value);

        float GetAngle() const;
        void SetAngle(float value);

        Vector2F GetVelocity() const;
        void SetVelocity(const Vector2F& value);

        float GetAngularVelocity() const;
        void SetAngularVelocity(float value);
    };
}
