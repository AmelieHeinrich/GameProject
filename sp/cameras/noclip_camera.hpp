/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 14:47
 */

#pragma once

#include "math_types.hpp"

#define CAMERA_DEFAULT_YAW -90.0f
#define CAMERA_DEFAULT_PITCH 0.0f
#define CAMERA_DEFAULT_SPEED 1.0f
#define CAMERA_DEFAULT_MOUSE_SENSITIVITY 1.0f
#define CAMERA_DEFAULT_GAMEPAD_SENSITIVITY 100.0f
#define CAMERA_DEFAULT_ZOOM 90.0f

struct frustum_plane
{
    hmm_vec3 Point;
    hmm_vec3 Norm;
};

struct frustum_view
{
    frustum_plane Near;
    frustum_plane Far;
    frustum_plane Right;
    frustum_plane Left;
    frustum_plane Top;
    frustum_plane Bottom;
};

struct noclip_camera
{
    float Yaw;
    float Pitch;

    hmm_vec3 Position;
    hmm_vec3 Front;
    hmm_vec3 Up;
    hmm_vec3 Right;
    hmm_vec3 WorldUp;
    
    hmm_vec3 MousePos;
    bool FirstMouse;

    hmm_mat4 View;
    hmm_mat4 Projection;

    float Width;
    float Height;

    float Acceleration;
    float Friction;
    hmm_vec3 Velocity;
    float MaxVelocity;

    frustum_view ViewFrustum;
    hmm_vec4 Planes[6];
};

void NoClipCameraInit(noclip_camera* Camera);
void NoClipCameraUpdate(noclip_camera* Camera, float Delta);
void NoClipCameraInput(noclip_camera* Camera, float Delta);
void NoClipCameraResize(noclip_camera* Camera, int Width, int Height);
void NoClipCameraUpdateFrustum(noclip_camera* Camera);
