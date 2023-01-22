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

void ProcessMesh(loaded_model *Model, aiMesh *Mesh, const aiScene *Scene)
{
    // TODO(amelie.h)
}

void ProcessNode(loaded_model *Model, aiNode *Node, const aiScene *Scene)
{
    for (int MeshIndex = 0; MeshIndex < Node->mNumMeshes; MeshIndex++)
    {
        aiMesh *Mesh = Scene->mMeshes[MeshIndex];
        ProcessMesh(Model, Mesh, Scene);
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

}
