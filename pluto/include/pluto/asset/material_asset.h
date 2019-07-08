#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <string>

namespace pluto
{
	class ShaderAsset;
	class FileReader;
	class Vector2I;
	class Vector2F;
	class Vector3I;
	class Vector3F;
	class Vector4I;
	class Vector4F;
	class Matrix2X2;
	class Matrix3X3;
	class Matrix4X4;

	/*
	 * File layout in disk. (Version 1)
	 * +--------------+------+------------------------------+
	 * | Type         | Size | Description                  |
	 * +--------------+------+------------------------------+
	 * | GUID         | 16   | File signature.              |
	 * | uint8_t      | 1    | Serializer version.          |
	 * | uint8_t      | 1    | Asset type.                  |
	 * | GUID         | 16   | Asset unique identifier.     |
	 * | uint8_t      | 1    | Asset name length.           |
	 * | string       | *    | Asset name.                  |
	 * +--------------+------+------------------------------+
	 */
	class PLUTO_API MaterialAsset final : public Asset
	{
	public:
		class PLUTO_API Factory final : public BaseFactory
		{
		public:
			explicit Factory(DiContainer& diContainer);
			std::unique_ptr<MaterialAsset> Create() const;
			std::unique_ptr<MaterialAsset> Create(const MaterialAsset& original) const;
			std::unique_ptr<MaterialAsset> Create(FileReader& fileReader) const;
		};

	private:
		class Impl;
		std::unique_ptr<Impl> impl;

	public:
		explicit MaterialAsset(std::unique_ptr<Impl> impl);
		MaterialAsset(const MaterialAsset& other) = delete;
		MaterialAsset(MaterialAsset&& other) noexcept;
		~MaterialAsset() override;

		MaterialAsset& operator=(const MaterialAsset& rhs) = delete;
		MaterialAsset& operator=(MaterialAsset&& rhs) noexcept;

		const Guid& GetId() const override;
		const std::string& GetName() const override;
		void SetName(std::string value) override;
		void Dump(FileWriter& fileWriter) const override;

		ShaderAsset& GetShader() const;
		void SetShader(ShaderAsset& shader);

		bool HasProperty(const std::string& propertyName) const;
		bool RemoveProperty(const std::string& propertyName);

		bool GetBool(const std::string& propertyName) const;
		void SetBool(const std::string& propertyName, bool value);

		int GetInt(const std::string& propertyName) const;
		void SetInt(const std::string& propertyName, int value);

		float GetFloat(const std::string& propertyName) const;
		void SetFloat(const std::string& propertyName, float value);

		const Vector2I& GetVector2I(const std::string& propertyName) const;
		void SetVector2I(const std::string& propertyName, Vector2I value);

		const Vector2F& GetVector2F(const std::string& propertyName) const;
		void SetVector2F(const std::string& propertyName, Vector2F value);

		const Vector3I& GetVector3I(const std::string& propertyName) const;
		void SetVector3I(const std::string& propertyName, Vector3I value);

		const Vector3F& GetVector3F(const std::string& propertyName) const;
		void SetVector3F(const std::string& propertyName, Vector3F value);

		const Vector4I& GetVector4I(const std::string& propertyName) const;
		void SetVector4I(const std::string& propertyName, Vector4I value);

		const Vector4F& GetVector4F(const std::string& propertyName) const;
		void SetVector4F(const std::string& propertyName, Vector4F value);

		const Matrix2X2& GetMatrix2X2(const std::string& propertyName) const;
		void SetMatrix2X2(const std::string& propertyName, Matrix2X2 value);

		const Matrix3X3& GetMatrix3X3(const std::string& propertyName) const;
		void SetMatrix3X3(const std::string& propertyName, Matrix3X3 value);

		const Matrix4X4& GetMatrix4X4(const std::string& propertyName) const;
		void SetMatrix4X4(const std::string& propertyName, Matrix4X4 value);
	};
}