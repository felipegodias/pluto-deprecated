#include "mesh_compiler.h"

#include <pluto/guid.h>
#include <pluto/service/service_collection.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>

#include <pluto/render/gl/gl_mesh_buffer.h>

#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

namespace pluto::compiler
{
    MeshCompiler::MeshCompiler(FileManager& fileManager, MeshAsset::Factory& meshAssetFactory)
        : fileManager(&fileManager),
          meshAssetFactory(&meshAssetFactory)
    {
    }

    std::vector<std::string> MeshCompiler::GetExtensions() const
    {
        return {".obj"};
    }

    std::vector<BaseCompiler::CompiledAsset> MeshCompiler::Compile(const std::string& input,
                                                                   const std::string& outputDir) const
    {
        Path inputPath = Path(input);
        Path plutoFilePath = inputPath;
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!fileManager->Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        Guid guid(plutoFile["guid"].as<std::string>());

        auto fr = fileManager->OpenRead(inputPath);

        std::vector<Vector3F> filePositions;
        std::vector<Vector2F> fileUVs;
        struct Face
        {
            Vector3I pos;
            Vector3I uv;
        };

        std::vector<Face> fileFaces;
        std::string op;
        std::ifstream& ifs = fr->GetStream();
        while (!ifs.eof())
        {
            ifs >> op;
            if (op == "v")
            {
                Vector3F pos;
                ifs >> pos.x >> pos.y >> pos.z;
                filePositions.push_back(pos);
            }
            else if (op == "vt")
            {
                Vector2F uv;
                ifs >> uv.x >> uv.y;
                fileUVs.push_back(uv);
            }
            else if (op == "f")
            {
                char c;
                Vector3I posIndex;
                Vector3I uvIndex;
                ifs >> posIndex.x >> c >> uvIndex.x >> posIndex.y >> c >> uvIndex.y >> posIndex.z >> c >> uvIndex.z;
                fileFaces.push_back({posIndex, uvIndex});
            }
        }

        std::unordered_map<std::string, int> map;
        std::vector<Vector3F> positions;
        std::vector<Vector2F> uvs;
        std::vector<Vector3I> triangles;

        const auto getOrAddFace = [&](const int pos, const int uv) -> uint32_t
        {
            std::stringstream ss;
            ss << pos << ':' << uv;
            const std::string key = ss.str();
            const auto it = map.find(key);
            if (it == map.end())
            {
                positions.push_back(filePositions[pos - 1]);
                uvs.push_back(fileUVs[uv - 1]);
                map.emplace(key, positions.size() - 1);
                return positions.size() - 1;
            }
            return it->second;
        };

        for (const auto& it : fileFaces)
        {
            Vector3I t;
            t.x = getOrAddFace(it.pos.x, it.uv.x);
            t.y = getOrAddFace(it.pos.y, it.uv.y);
            t.z = getOrAddFace(it.pos.z, it.uv.z);
            triangles.push_back(t);
        }

        auto meshAsset = meshAssetFactory->Create();

        // Evil, I know. But it's better than expose the guid to changes directly.
        const_cast<Guid&>(meshAsset->GetId()) = guid;

        meshAsset->SetName(inputPath.GetNameWithoutExtension());
        meshAsset->SetPositions(std::move(positions));
        meshAsset->SetUVs(std::move(uvs));
        meshAsset->SetTriangles(std::move(triangles));

        const auto fileWriter = fileManager->OpenWrite(Path(outputDir + "/" + meshAsset->GetId().Str()));
        meshAsset->Dump(*fileWriter);

        std::vector<CompiledAsset> assets;
        assets.push_back({meshAsset->GetId(), input});
        return assets;
    }
}
