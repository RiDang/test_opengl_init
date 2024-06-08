#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

#include "shader/shader.h"
#include "texture/texture.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


GLFWwindow* InitWindow(){
    // ============== 窗口初始化 start
    const int kWdith = 800, kHeight = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(kWdith, kHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD " << std::endl;
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

int main()
{

    // ============== 窗口初始化 end

    GLFWwindow* window = InitWindow();
    glGetError(); 

    const std::string ROOT_PATH = "/home/ubt/Projects/opengl/test_opengl_init";
    const std::string vertex_path = ROOT_PATH + "/shader/shader_vertex_1_5.vs";
    const std::string frag_path = ROOT_PATH + "/shader/shader_fragment_1_5.fs";
    const std::string texture_wall_path = ROOT_PATH + "/data/wall.jpg";
    const std::string texture_face_path = ROOT_PATH + "/data/awesomeface.png";
    Shader::PathMap path_map{{"vertex",vertex_path},
                            {"frag",frag_path}};
    Shader shader(path_map);
 
    // ===========================
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f, // left  
    //     1.0f, 0.0f, 0.0f, 0, 0,
    //      0.5f, -0.5f, 0.0f, // right 
    //      0.0f, 1.0f, 0.0f, 1, 0,
    //      0.5f,  0.5f, 0.0f,  // top   
    //      0.0f, 0.0f, 1.0f, 1, 1,
    //      -0.5f,  0.5f, 0.0f,  // top   
    //      1.0f, 0.0f, 1.0f, 0, 1
    // }; 

    // unsigned int indices[] = {
    //     0, 1, 2,
    //     0, 2, 3
    // };

    unsigned int VBOs[3], VAOs[3];
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int SIZEs[] = {8, 3, 3, 2}; // 总长度，点长度，颜色长度，纹理长度
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZEs[0]*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SIZEs[0]*sizeof(float), (void*)(SIZEs[1] * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, SIZEs[0]*sizeof(float), (void*)((SIZEs[1] + SIZEs[2]) * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  
 
    stbi_set_flip_vertically_on_load(true);
    Texture texture_1(texture_wall_path);
    Texture texture_2(texture_face_path);
  
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.use(); 
    shader.set_int("ourTex_2", 1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    auto start = std::chrono::high_resolution_clock::now();
    float sum_dt = 0.0;
    while(!glfwWindowShouldClose(window)){
        

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        texture_1.use();
        glActiveTexture(GL_TEXTURE1);
        texture_2.use();

        shader.use();

        if (sum_dt > 0.1){
            glm::mat4 trans = glm::mat4(1.0f);

            float timeVallue = glfwGetTime();
            trans = glm::scale(trans, glm::vec3(sin(timeVallue), sin(timeVallue), 1.0f));
            trans = glm::rotate(trans, timeVallue, glm::vec3(0.0f, 0.0f, 1.0f));
            trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
            unsigned int trans_loc = glGetUniformLocation(shader.shader_program_, "transform");
            glUniformMatrix4fv(trans_loc, 1, GL_FALSE, glm::value_ptr(trans));
            sum_dt = 0;
        }        
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        auto end = std::chrono::high_resolution_clock::now();
        auto dt = std::chrono::duration<double>(end - start).count();   
        sum_dt += dt;     
        std::cout << " - fps: " << 1 / dt << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

        start = std::chrono::high_resolution_clock::now();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glfwTerminate();
    return 0;
}

