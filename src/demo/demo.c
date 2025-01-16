#include "demo.h"

GLFWwindow *window = 0;
camera *cam = 0;
GLuint program = 0;

GLint camera_loc = 0;
GLint model_loc = 0;
GLint normalMatrix_loc = 0;
GLint lightColor_loc = 0;
GLint lightDir_loc = 0;
GLint ambient_loc = 0;
GLint time_loc = 0;
GLint last_movement_normal_loc = 0;
GLint last_movement_time_loc = 0;

vec3 lightColor = {1.0f, 1.0f, 1.0f};
vec3 lightDir = {20, 50, 20};
float ambient = 0.5f;
float time = 0.0f;
vec3 last_movement_normal = {0, 0, 0};
float last_movement_time = 0.0f;

GLuint VAO = 0, VBO = 0, EBO = 0;
mat4 translation = GLM_MAT4_IDENTITY_INIT;
mat4 rotation = GLM_MAT4_IDENTITY_INIT;
mat4 scale = GLM_MAT4_IDENTITY_INIT;
mat4 model = GLM_MAT4_IDENTITY_INIT;
mat4 normal = GLM_MAT4_IDENTITY_INIT;

// vec3 pos, vec3 norm, vec4 color, float jiggle_coefficient, float damping_coefficient
// bottom of the cube isn't jiggling, top of the cube is jiggling
float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0, 0,      // A 0
    -0.5f, 0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0.1f, 0.5f, // B 1
    0.5f, 0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0.1f, 0.5f,  // C 2
    0.5f, -0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0, 0,       // D 3

    -0.5f, -0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0, 0,      // E 4
    0.5f, -0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0, 0,       // F 5
    0.5f, 0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0.1f, 0.5f,  // G 6
    -0.5f, 0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0.1f, 0.5f, // H 7

    -0.5f, 0.5f, -0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0.1f, 0.5f, // B 8
    -0.5f, -0.5f, -0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0, 0,      // A 9
    -0.5f, -0.5f, 0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0, 0,       // E 10
    -0.5f, 0.5f, 0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0.1f, 0.5f,  // H 11

    0.5f, -0.5f, -0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0, 0,      // D 12
    0.5f, 0.5f, -0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0.1f, 0.5f, // C 13
    0.5f, 0.5f, 0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0.1f, 0.5f,  // G 14
    0.5f, -0.5f, 0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0, 0,       // F 15

    -0.5f, -0.5f, -0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0, // A 16
    0.5f, -0.5f, -0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0,  // D 17
    0.5f, -0.5f, 0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0,   // F 18
    -0.5f, -0.5f, 0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0,  // E 19

    0.5f, 0.5f, -0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.1f, 0.5f,  // C 20
    -0.5f, 0.5f, -0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.1f, 0.5f, // B 21
    -0.5f, 0.5f, 0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.1f, 0.5f,  // H 22
    0.5f, 0.5f, 0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.1f, 0.5f,   // G 23
};

float cube_indices[] = {
    // front and back
    2, 1, 0,
    0, 3, 2,
    6, 5, 4,
    4, 7, 6,
    // left and right
    9, 8, 11,
    11, 10, 9,
    14, 13, 12,
    12, 15, 14,
    // bottom and top
    18, 17, 16,
    16, 19, 18,
    22, 21, 20,
    20, 23, 22};

void init_demo()
{
    window = create_window(800, 600, 0, 1, 4);
    if (window == 0)
    {
        return;
    }
    cam = create_camera(800, 600, (vec3){0.0f, 5, 60.0f}, 60, 0.1f, 1000, 1, 100, -90, (vec3){0, 1, 0});
    init_shader_program();
    program = get_shader_program();

    glUseProgram(program);

    camera_loc = glGetUniformLocation(program, "camera");
    model_loc = glGetUniformLocation(program, "model");
    normalMatrix_loc = glGetUniformLocation(program, "normalMatrix");
    lightColor_loc = glGetUniformLocation(program, "lightColor");
    lightDir_loc = glGetUniformLocation(program, "lightDir");
    ambient_loc = glGetUniformLocation(program, "ambient");
    time_loc = glGetUniformLocation(program, "time");
    last_movement_normal_loc = glGetUniformLocation(program, "last_movement_normal");
    last_movement_time_loc = glGetUniformLocation(program, "last_movement_time");

    glm_vec3_normalize(lightDir);
}

void play_demo()
{
    while (!get_key_down(GLFW_KEY_ESCAPE))
    {
        glfwPollEvents();
        poll_events(window);
        run_input_free_camera(cam, window);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }
}

void delete_demo()
{
    delete_window(window);
    delete_camera(cam);
    delete_shader_program();
}