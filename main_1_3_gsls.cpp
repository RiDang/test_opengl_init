#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "shader/shader.h"


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
    const std::string ROOT_PATH = "/home/ubt/Projects/opengl/test_opengl_init";
    const std::string vertex_path = ROOT_PATH + "/shader/shader_vertex.vs";
    const std::string frag_path = ROOT_PATH + "/shader/shader_fragment.fs";
    Shader::PathMap path_map{{"vertex",vertex_path},
                            {"frag",frag_path}};
    Shader shader(path_map);

    // ===========================



    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
        1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, // right 
         0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  // top   
         0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, 0.0f,  // top   
         1.0f, 0.0f, 1.0f
    }; 

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBOs[3], VAOs[3];
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.use();
        float timeVallue = glfwGetTime();
        float greenValue = (sin(timeVallue) / 2.0f) + 0.5f;
        // shader.set_float("ourColor", greenValue);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 7);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glfwTerminate();
    return 0;
}

