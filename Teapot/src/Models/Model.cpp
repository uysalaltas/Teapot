#include "Model.h"

namespace Teapot
{
    Model::Model(const std::string& pathObject,const std::string& nameObject)
    {
        path = pathObject;
        name = nameObject;
        LoadModel(path);
        hasTexture = true;
    }

    Model::Model(const Shapes::Shape& shapes, const std::string& nameObject)
    {
        std::vector<Texture> textures;
        std::cout << nameObject << " Pos Size: " << shapes.positions.size() << std::endl;
        name = nameObject;
        meshes.push_back(std::make_unique<Renderer>(shapes.vertices, shapes.indices, textures));
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
        Texture texture(texturePath.c_str(), textureType, unit);
        meshes[0]->textures.push_back(texture);
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

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(const aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    std::unique_ptr<Renderer> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex{};
            glm::vec3 vector{};
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            // Color
            vertex.color.x = modelColor.x;
            vertex.color.y = modelColor.y;
            vertex.color.z = modelColor.z;
            // Normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            // Texture coordinates
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;

                vertex.texUV = vec;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.bitangent = vector;
            }
            else
                vertex.texUV = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "material.diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "material.specular");
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        //std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return std::make_unique<Renderer>(vertices, indices, textures);
    }

    std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName)
    {
        std::vector<Texture> textures{};
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;

            for (const auto& textureLoaded : m_texturesLoaded)
            {
                if (std::strcmp(textureLoaded.path.c_str(), str.C_Str()) == 0)
                {
                    std::cout << "SAME PATH " << std::endl;
                    textures.push_back(textureLoaded);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                std::string texturePath = (m_directory + "//").append(str.C_Str());
                Texture texture(texturePath.c_str(), typeName, i);
                textures.push_back(texture);
                m_texturesLoaded.push_back(texture);
            }
        }
        return textures;
    }
}