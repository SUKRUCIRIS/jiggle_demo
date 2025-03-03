#pragma once
#include "tp.h"

typedef struct camera
{
    vec3 position;
    vec3 orientation;
    vec3 up;
    unsigned char firstclick;
    int width;
    int height;
    float speed;
    float sensitivity;
    mat4 result;
    float FOVdeg;
    float nearPlane;
    float farPlane;
    mat4 view;
    mat4 projection;
} camera;

camera *create_camera(int width, int height, vec3 position, float FOVdeg, float nearPlane,
                      float farPlane, float speed, float sensitivity, float angle, vec3 angle_axis);

void delete_camera(camera *cam);

void run_input_fp_camera(camera *cam, GLFWwindow *window);

void run_input_free_camera(camera *cam, GLFWwindow *window);

void calculate_camera(camera *cam, float near_plane, float far_plane);