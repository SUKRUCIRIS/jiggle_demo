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
GLint first_movement_time_loc = 0;

vec3 lightColor = {1.0f, 1.0f, 1.0f};
vec3 lightDir = {20, 50, 20};
float ambient = 0.5f;
float time = 0.0f;
vec3 last_movement_normal = {0, 0, 0};
float last_movement_time = 0.0f;
float first_movement_time = 0.0f;

GLuint VAO = 0, VBO = 0, EBO = 0;
mat4 translation = GLM_MAT4_IDENTITY_INIT;
mat4 rotation = GLM_MAT4_IDENTITY_INIT;
mat4 scale = GLM_MAT4_IDENTITY_INIT;
mat4 model = GLM_MAT4_IDENTITY_INIT;
mat4 normal = GLM_MAT4_IDENTITY_INIT;

// vec3 pos, vec3 norm, vec4 color, float jiggle_coefficient, float damping_coefficient
// bottom of the cube isn't jiggling, top of the cube is jiggling
GLfloat cube_vertices[] = {
    -0.5f, -0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0, 0,       // A 0
    -0.5f, 0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0.2f, 0.05f, // B 1
    0.5f, 0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0.2f, 0.05f,  // C 2
    0.5f, -0.5f, -0.5f, 0, 0, -1, 1.0f, 0.2f, 0.2f, 1.0f, 0, 0,        // D 3

    -0.5f, -0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0, 0,       // E 4
    0.5f, -0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0, 0,        // F 5
    0.5f, 0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0.2f, 0.05f,  // G 6
    -0.5f, 0.5f, 0.5f, 0, 0, 1, 0.2f, 0.4f, 1.0f, 1.0f, 0.2f, 0.05f, // H 7

    -0.5f, 0.5f, -0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0.2f, 0.05f, // B 8
    -0.5f, -0.5f, -0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0, 0,       // A 9
    -0.5f, -0.5f, 0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0, 0,        // E 10
    -0.5f, 0.5f, 0.5f, -1, 0, 0, 0.2f, 1.0f, 0.4f, 1.0f, 0.2f, 0.05f,  // H 11

    0.5f, -0.5f, -0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0, 0,       // D 12
    0.5f, 0.5f, -0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0.2f, 0.05f, // C 13
    0.5f, 0.5f, 0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0.2f, 0.05f,  // G 14
    0.5f, -0.5f, 0.5f, 1, 0, 0, 1.0f, 0.9f, 0.2f, 1.0f, 0, 0,        // F 15

    -0.5f, -0.5f, -0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0, // A 16
    0.5f, -0.5f, -0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0,  // D 17
    0.5f, -0.5f, 0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0,   // F 18
    -0.5f, -0.5f, 0.5f, 0, -1, 0, 0.6f, 0.2f, 1.0f, 1.0f, 0, 0,  // E 19

    0.5f, 0.5f, -0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, 0.05f,  // C 20
    -0.5f, 0.5f, -0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, 0.05f, // B 21
    -0.5f, 0.5f, 0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, 0.05f,  // H 22
    0.5f, 0.5f, 0.5f, 0, 1, 0, 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, 0.05f,   // G 23
};

GLuint cube_indices[] = {
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
    cam = create_camera(800, 600, (vec3){0.0f, 2, 5.0f}, 60, 0.1f, 1000, 1, 100, -30, (vec3){1, 0, 0});
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
    first_movement_time_loc = glGetUniformLocation(program, "first_movement_time");

    glm_vec3_normalize(lightDir);

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
    }
    glBindVertexArray(0);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 288 * sizeof(GLfloat), cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(10 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(11 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), cube_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void play_demo()
{
    glUseProgram(program);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    vec3 move = {0, 0, 0};
    vec3 oldmove = {0, 0, 0};
    float speed = 0.02f;
    while (!get_key_down(GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        poll_events(window);
        run_input_free_camera(cam, window);
        calculate_camera(cam, 0.1f, 1000);

        time = (float)glfwGetTime() * 10;
        glm_vec3_zero(move);
        if (get_key_down(GLFW_KEY_UP))
        {
            move[2] += speed;
        }
        if (get_key_down(GLFW_KEY_DOWN))
        {
            move[2] -= speed;
        }
        if (get_key_down(GLFW_KEY_LEFT))
        {
            move[0] -= speed;
        }
        if (get_key_down(GLFW_KEY_RIGHT))
        {
            move[0] += speed;
        }
        glm_vec3_normalize(move);
        if (glm_vec3_norm(move) > 0.0f)
        {
            if (glm_vec3_norm(oldmove) == 0.0f)
            {
                first_movement_time = time;
            }
            last_movement_time = time;
            glm_vec3_copy(move, last_movement_normal);
            glm_vec3_scale(last_movement_normal, speed, move);
            glm_vec3_add(move, translation[3], translation[3]);
        }
        glm_vec3_copy(move, oldmove);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm_mat4_mulN((mat4 *[]){&translation, &rotation, &scale}, 3, model);
        glm_mat4_inv(model, normal);
        glm_mat4_transpose(normal);

        glUniformMatrix4fv(camera_loc, 1, GL_FALSE, cam->result[0]);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model[0]);
        glUniformMatrix4fv(normalMatrix_loc, 1, GL_FALSE, normal[0]);
        glUniform3fv(lightColor_loc, 1, lightColor);
        glUniform3fv(lightDir_loc, 1, lightDir);
        glUniform1f(ambient_loc, ambient);
        glUniform1f(time_loc, time);
        glUniform3fv(last_movement_normal_loc, 1, last_movement_normal);
        glUniform1f(last_movement_time_loc, last_movement_time);
        glUniform1f(first_movement_time_loc, first_movement_time);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }
}

void delete_demo()
{
    delete_window(window);
    delete_camera(cam);
    delete_shader_program();
}