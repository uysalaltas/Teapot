#include "Model.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
    Model::Model(std::string pathObject, std::string nameObject)
        : path(pathObject)
        , name(nameObject)
    {
        LoadModel(path);
    }

    Model::Model(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& colors, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices, std::string nameObject)
    {
        std::vector<Texture> textures;
        std::vector<Vertex> vertices;
        for (int i = 0; i < positions.size(); i++)
        {
            Vertex temp;
            temp.position = positions[i];
            temp.color = colors[i];
            temp.normal = normals[i];

            vertices.push_back(temp);
        }

        std::cout << nameObject << " Pos Size: " << positions.size() << std::endl;

        meshes.push_back(new Renderer(vertices, indices, textures));
    }

    Model::~Model()
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            delete(meshes[i]);
        }
    }

    void Model::Draw(Shader& shader)
    {
        shader.SetUniformMat4f("model", objModel);

        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i]->DrawTriangle(shader);
        }
    }

    void Model::Translate(glm::vec3 translation)
    {
        objTranslation = translation;
        objModel = glm::translate(glm::mat4(1.0f), objTranslation);
    }

    void Model::LoadTextureToModel(std::string textureType, std::string texturePath, int unit)
    {
        Texture texture(texturePath.c_str(), textureType, unit);
        meshes[0]->textures.push_back(texture);
    }

    void Model::LoadModel(std::string path)
    {
        Assimp::Importer import;
        unsigned int importOptions = aiProcess_Triangulate
            | aiProcess_OptimizeMeshes
            | aiProcess_JoinIdenticalVertices
            | aiProcess_Triangulate
            | aiProcess_CalcTangentSpace;
        const aiScene* scene = import.ReadFile(path, importOptions);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
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

    Renderer* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
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

        //std::cout << "Face Count " << mesh->mNumFaces << std::endl;
        //std::cout << "Vertex Count " << mesh->mNumVertices << std::endl;

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        //aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        //std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return new Renderer(vertices, indices, textures);
    }

    std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;

            for (unsigned int j = 0; j < texturesLoaded.size(); j++)
            {
                if (std::strcmp(texturesLoaded[j].path.c_str(), str.C_Str()) == 0)
                {
                    std::cout << "SAME PATH " << std::endl;
                    textures.push_back(texturesLoaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                Texture texture(str.C_Str(), typeName, i);

                textures.push_back(texture);
                texturesLoaded.push_back(texture);
            }
        }

        return textures;
    }
}