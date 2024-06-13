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
#include "camera/camera.h"

// - test

typedef struct {
    glm::dvec2 last_down;
    bool is_last_down{false};
} MouseInfo;


const int kWidth = 800, kHeight = 600;
MouseInfo mouse_left_info, mouse_right_info;
Camera camera;
float delta_time = 0.5f;
float last_time = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset ){
    camera.update_forward(yoffset);
}


void processInput(GLFWwindow* window){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, true);
    }
    // float  speed = 0.05f;;
    float  speed = delta_time * 10;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.update_forward(speed);
        camera.info();

    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.update_forward(speed);
        camera.info();

    }
    else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.update_camera(0, -speed);
        camera.info();
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.update_camera(0, speed);
        camera.info();
    }

    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        camera.reset();
        camera.info();
    }

    // ====== 鼠标右键设置 ===========


    auto process_mouse_key = [&](unsigned int left_or_right, MouseInfo& mouse_info){
        if(glfwGetMouseButton(window, left_or_right) == GLFW_PRESS){
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            glm::dvec2 cur_pos = glm::vec2(x, y);
            if(mouse_info.is_last_down){
                glm::dvec2 delta_pos = cur_pos - mouse_info.last_down;
                delta_pos *= 0.1;
                // camera.update_camera((float)(delta_pos[1]), (float)(delta_pos[0]));
                if(left_or_right == GLFW_MOUSE_BUTTON_RIGHT){
                    camera.update_camera((float)(delta_pos[1]), (float)(delta_pos[0]));
                }
                else{
                    camera.update_object((float)(delta_pos[1]), (float)(delta_pos[0]));
                }

            }
            else{
                mouse_info.is_last_down = true;
            }
            mouse_info.last_down = cur_pos;

        }

        if(glfwGetMouseButton(window, left_or_right) == GLFW_RELEASE){
            mouse_info.is_last_down = false;
        }
    };

    process_mouse_key(GLFW_MOUSE_BUTTON_RIGHT, mouse_right_info);
    process_mouse_key(GLFW_MOUSE_BUTTON_LEFT, mouse_left_info);

    // ====== 鼠标左设置 ===========
    
}

void show_mat4(const glm::mat4& mat4){
    std::cout << "[";
    for(size_t i=0; i<4; i++){
        std::cout << "[";
        for(size_t j=0; j<4; j++){
            if(j+1 < 4)
              std::cout << mat4[j][i] << ", ";
            else
              std::cout << mat4[j][i];
        }

        if(i < 3)
            std::cout << "]," << std::endl;
        else
            std::cout << "]";
    }
    std::cout << "]" << std::endl;

}

GLFWwindow* InitWindow(){
    // ============== 窗口初始化 start

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "LearnOpenGL", NULL, NULL);
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
    const std::string vertex_path = ROOT_PATH + "/shader/shader_vertex_1_7.vs";
    const std::string frag_path = ROOT_PATH + "/shader/shader_fragment_1_7.fs";
    const std::string texture_wall_path = ROOT_PATH + "/data/wall.jpg";
    const std::string texture_face_path = ROOT_PATH + "/data/awesomeface.png";
    Shader::PathMap path_map{{"vertex",vertex_path},
                            {"frag",frag_path}};
    
    glEnable(GL_DEPTH_TEST);
    Shader shader(path_map);

    // ===========================
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
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

    int SIZEs[] = {5, 3, 2}; // 总长度，点长度，颜色长度，纹理长度
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZEs[0]*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SIZEs[0]*sizeof(float), (void*)((SIZEs[1] ) * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    stbi_set_flip_vertically_on_load(true);
    Texture texture_1(texture_wall_path);
    Texture texture_2(texture_face_path);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.use(); 
    shader.set_int("ourTex_1", 0);
    shader.set_int("ourTex_2", 1);



    // show_mat4(model);
    // std::cout << std::endl;
    // show_mat4(view);
    // std::cout << std::endl;
    // show_mat4(projection);
    // std::cout << std::endl;


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    auto start = std::chrono::high_resolution_clock::now();
    float sum_dt = 0.0;
    while(!glfwWindowShouldClose(window)){
        float cur_time = glfwGetTime();
        delta_time = cur_time - last_time;
        last_time = cur_time;

        glfwSetScrollCallback(window, scroll_callback);

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        texture_1.use();
        glActiveTexture(GL_TEXTURE1);
        texture_2.use();


        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          =glm::mat4(1.0f);
        glm::mat4 projection    =glm::mat4(1.0f);

        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
        // view = glm::rotate(view, -(float)cur_time, glm::vec3(0.0f, 1.0f, 0.0f));
        float radius = 10.0f;
        float cam_x = sin(cur_time) * radius;
        float cam_z = cos(cur_time) * radius;
        // view = glm::lookAt(glm::vec3(cam_x, 10.0, cam_z), glm::vec3(0, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f));
        // view = glm::lookAt(camera_pos, camera_end, camera_up);
        view = camera.view_mat4_;
        projection = glm::perspective(glm::radians(45.0f), (float)kWidth / (float)kHeight, 0.1f, 100.0f);
        // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        // projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

        shader.use();
        shader.set_mat4("model", glm::value_ptr(model));
        shader.set_mat4("view", glm::value_ptr(view));
        shader.set_mat4("projection", glm::value_ptr(projection));

        for(size_t i=0; i< 10; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            // model = glm::rotate(model, i * cur_time + cur_time, glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, i * 0.1f, glm::vec3(1.0f, 0.3f, 0.5f));
            shader.set_mat4("model", glm::value_ptr(model));
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }


        // auto end = std::chrono::high_resolution_clock::now();
        // auto dt = std::chrono::duration<double>(end - start).count();   
        // sum_dt += dt;     
        // std::cout << " - fps: " << 1 / dt << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

        // start = std::chrono::high_resolution_clock::now();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glfwTerminate();
    return 0;
}

