#include "Model.h"

namespace Teapot
{
    Model::Model(const std::string& pathObject,const std::string& nameObject)
    {
        path = pathObject;
        name = nameObject;
        modelType = Teapot::ModelType::model;
        LoadModel(path);
    }

    Model::Model(Shapes::Shape& shapes, const std::string& nameObject)
    {
        std::vector<Teapot::Texture> textures;

        std::cout << nameObject << " Pos Size: " << shapes.positions.size() << std::endl;
        name = nameObject;
        modelType = Teapot::ModelType::model;
        meshes.push_back(std::make_unique<Teapot::Renderer>(
            std::move(shapes.vertices), 
            std::move(shapes.indices), 
            std::move(textures))
        );
    }

    void Model::Draw(Teapot::Shader& shader)
    {
        for (const auto& mesh : meshes)
        {
            mesh->DrawTriangle(shader);
        }
    }

    void Model::LoadTextureToModel(const std::string& textureType, const std::string& texturePath, int unit)
    {
        Teapot::Texture texture(texturePath.c_str(), textureType, unit);
        for(auto& mesh : meshes)
        {
            mesh->PushTexture(texture);
		}
    }

    void Model::LoadModel(const std::string& modelPath)
    {
        Assimp::Importer importer;
        unsigned int importOptions = aiProcess_Triangulate
            | aiProcess_OptimizeMeshes
            | aiProcess_JoinIdenticalVertices
            | aiProcess_Triangulate
            | aiProcess_CalcTangentSpace;
        const aiScene* scene = importer.ReadFile(modelPath, importOptions);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout 
                << "ERROR::ASSIMP:: Failed to load model from path: "
                << modelPath
                << " with error: " 
                << importer.GetErrorString() 
                << std::endl;
            return;
        }
        m_directory = modelPath.substr(0, modelPath.find_last_of('/'));
        hasTexture = true;

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(const aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    std::unique_ptr<Teapot::Renderer> Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Teapot::Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Teapot::Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex{};
            // Positions
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            // Color
            vertex.color.x = modelColor.x;
            vertex.color.y = modelColor.y;
            vertex.color.z = modelColor.z;
            // Normals
            if (mesh->HasNormals())
            {
                vertex.normal.x = mesh->mNormals[i].x;
                vertex.normal.y = mesh->mNormals[i].y;
                vertex.normal.z = mesh->mNormals[i].z;
            }
            // Texture coordinates
            if (mesh->mTextureCoords[0])
            {
                vertex.texUV.x = mesh->mTextureCoords[0][i].x;
                vertex.texUV.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                vertex.texUV = glm::vec2(0.0f, 0.0f);
            }

            if (mesh->HasTangentsAndBitangents())
            {
                // tangent
                vertex.tangent.x = mesh->mTangents[i].x;
                vertex.tangent.y = mesh->mTangents[i].y;
                vertex.tangent.z = mesh->mTangents[i].z;
                // bitangent
                vertex.bitangent.x = mesh->mBitangents[i].x;
                vertex.bitangent.y = mesh->mBitangents[i].y;
                vertex.bitangent.z = mesh->mBitangents[i].z;
            }
            else
            {
                vertex.tangent = glm::vec3(0.0f);
				vertex.bitangent = glm::vec3(0.0f);
            }

            vertices.push_back(std::move(vertex));
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        LoadMaterialTextures(textures, material, aiTextureType_DIFFUSE, "material.diffuse");

        return std::make_unique<Teapot::Renderer>(std::move(vertices), std::move(indices), std::move(textures));
    }

    void Model::LoadMaterialTextures(
        std::vector<Texture>& textures, 
        const aiMaterial* mat, 
        const aiTextureType type, 
        const std::string& typeName
    )
    {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
			hasTexture = true;

            for (const auto& textureLoaded : textures)
            {
                if (std::strcmp(textureLoaded.path.c_str(), str.C_Str()) == 0)
                {
                    std::cout << "SAME PATH " << std::endl;
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                std::string texturePath = (m_directory + "//").append(str.C_Str());
                textures.emplace_back(texturePath.c_str(), typeName, i);
            }
        }
    }
}