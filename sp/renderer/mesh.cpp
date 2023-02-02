/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 11:34
 */

#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "systems/log_system.hpp"

mesh ProcessMesh(loaded_model *Model, aiMesh *Mesh, const aiScene *Scene)
{
    mesh Out;

    std::vector<mesh_vertex> Vertices;
    std::vector<uint32_t> Indices;

    for (uint32_t VertexIndex = 0; VertexIndex < Mesh->mNumVertices; VertexIndex++)
    {
        mesh_vertex Vertex;
        
        Vertex.Position.X = Mesh->mVertices[VertexIndex].x;
        Vertex.Position.Y = Mesh->mVertices[VertexIndex].y;
        Vertex.Position.Z = Mesh->mVertices[VertexIndex].z;

        if (Mesh->HasNormals())
        {
            Vertex.Normal.X = Mesh->mNormals[VertexIndex].x;
            Vertex.Normal.Y = Mesh->mNormals[VertexIndex].y;
            Vertex.Normal.Z = Mesh->mNormals[VertexIndex].z;
        }

        if (Mesh->mTextureCoords[0])
        {
            Vertex.UV.X = Mesh->mTextureCoords[VertexIndex]->x;
            Vertex.UV.Y = Mesh->mTextureCoords[VertexIndex]->y;
        } 
        else
        {
            Vertex.UV = HMM_Vec2(0.0f, 0.0f);
        }

        Vertices.push_back(Vertex);
    }

    for (uint32_t FaceIndex = 0; FaceIndex < Mesh->mNumFaces; FaceIndex++)
    {
        aiFace Face = Mesh->mFaces[FaceIndex];
        for (uint32_t IndexIndex = 0; IndexIndex < Face.mNumIndices; IndexIndex++)
            Indices.push_back(Face.mIndices[IndexIndex]);
    }

    Out.VertexCount = Vertices.size();
    Out.IndexCount = Indices.size();

    GpuBufferInit(&Out.VertexBuffer, Vertices.size() * sizeof(mesh_vertex), sizeof(mesh_vertex), gpu_buffer_type::Vertex);
    GpuBufferUpload(&Out.VertexBuffer, Vertices.data(), Vertices.size() * sizeof(mesh_vertex));
    GpuBufferInit(&Out.IndexBuffer, Indices.size() * sizeof(uint32_t), sizeof(uint32_t), gpu_buffer_type::Index);
    GpuBufferUpload(&Out.IndexBuffer, Indices.data(), Indices.size() * sizeof(uint32_t));

    // TODO(amelie.h): Texture loading

    return Out;
}

void ProcessNode(loaded_model *Model, aiNode *Node, const aiScene *Scene)
{
    for (int MeshIndex = 0; MeshIndex < Node->mNumMeshes; MeshIndex++)
    {
        aiMesh *Mesh = Scene->mMeshes[MeshIndex];
        Model->Meshes.push_back(ProcessMesh(Model, Mesh, Scene));
    }
    for (int ChildIndex = 0; ChildIndex < Node->mNumChildren; ChildIndex++)
    {
        ProcessNode(Model, Node->mChildren[ChildIndex], Scene);
    }
}

void ModelLoad(loaded_model *Model, const std::string& Path)
{
    Assimp::Importer Importer;
    const aiScene *Scene = Importer.ReadFile(Path, aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        LogError("Failed to load assimp model! (%s)", Path.c_str());
    }
    Model->WorkingDirectory = Path.substr(0, Path.find_last_of('/'));
    ProcessNode(Model, Scene->mRootNode, Scene);
}

void ModelFree(loaded_model *Model)
{
    for (auto Mesh : Model->Meshes)
    {
        GpuBufferFree(&Mesh.VertexBuffer);
        GpuBufferFree(&Mesh.IndexBuffer);
    }
    Model->Meshes.clear();
}
