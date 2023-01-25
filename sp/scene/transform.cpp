/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/01/2023 15:31
 */

#include "transform.hpp"

void TransformUpdate(transform *Transform)
{
    Transform->Matrix = HMM_Translate(Transform->Position) * HMM_Scale(Transform->Scale);
    Transform->Matrix = Transform->Matrix * HMM_Rotate(Transform->Rotation.X, HMM_Vec3(1.0f, 0.0f, 0.0f));
    Transform->Matrix = Transform->Matrix * HMM_Rotate(Transform->Rotation.Y, HMM_Vec3(0.0f, 1.0f, 0.0f));
    Transform->Matrix = Transform->Matrix * HMM_Rotate(Transform->Rotation.Z, HMM_Vec3(0.0f, 0.0f, 1.0f));
}
