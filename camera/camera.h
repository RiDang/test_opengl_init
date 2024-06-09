#ifndef OPENGL_CAMERA_H_
#define OPENGL_CAMERA_H_

#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
    Camera();
    ~Camera();

    int init();

    /**
     * 根据输入进行计算
     *@ parameters: pitch: x/z - y,  yaw: x - y,  
    */ 
    void compute_camera();

    void compute_object(const glm::vec4& rotate);

    void compute_object(const float angle, const glm::vec3& axis);
    
    void compute_zoom(const float zoom);

    /**
     * 进行连续更新
    */

   void update_camera(const float delta_pitch, const float delta_yaw, bool constrain = true);

   void update_object(const float delta_phi, const float delta_theta, bool constrain = true);

   void update_forward(const float delta_forward, bool constrain = true);

    void info() const;
    void reset();


public:
    glm::vec3 camera_pos_{0.0f, 0.0f, 1.0f};
    glm::vec3 camera_front_{0.0f, 0.0f, -1.0f};
    glm::vec3 camera_up_{0.0f, 1.0f, 0.0f};
    glm::vec3 camera_right{1.0f, 0.0f, 0.0f};
    const glm::vec3 kWorldUp_{0.0f, 1.0f, 0.0f};
    glm::mat4 view_;

    float pitch_{0.0f};
    // float yaw_{-90.0f};
    float yaw_{0.0f};

    const float kSensitive{0.1}; // 鼠标
    const float kSpeed{2.5}; // 按键    
};


Camera::Camera(){
    // 计算相机姿态
    compute_camera();

    // 计算旋转矩阵
    view_ = glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);    
}
Camera::~Camera(){}

void Camera::info() const {
        std::cout << " camera_pos_ " << camera_pos_.x << ", " << camera_pos_.y << ", " << camera_pos_.z << std::endl;
        std::cout << " camera_front_ " << camera_front_.x << ", " << camera_front_.y << ", " << camera_front_.z << std::endl;
        std::cout << " camera_up_ " << camera_up_.x << ", " << camera_up_.y << ", " << camera_up_.z << std::endl;

        std::cout << " angle: pitch_ " << pitch_  << ", yaw_ " << yaw_ << std::endl;
}

void Camera::reset(){
    camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_pos_ = glm::vec3(0.0f, 0.0f, 1.0f);
    camera_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    pitch_ = 0.0f;
    yaw_ = 0.0f;
    view_ = glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);
}


void Camera::compute_camera(){
    
    glm::vec3 direction;
    // direction.x = cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
    // direction.y = sin(glm::radians(pitch_));
    // direction.z = cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));

    direction.z = -cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
    direction.y = sin(glm::radians(pitch_));
    direction.x = cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));


    camera_front_ = glm::normalize(direction);

    glm::vec3 camera_right_ = glm::normalize(glm::cross(camera_front_, kWorldUp_));
    glm::vec3 camera_up_ = glm::normalize(glm::cross(camera_right_, camera_front_));

}

void Camera::update_camera(const float delta_pitch, const float delta_yaw, bool constrain){
    // 首先进行更新
    pitch_ += delta_pitch;
    yaw_ += delta_yaw;

    // 边界约束
    if(constrain){
        if (pitch_ > 89.0){ pitch_ = 89.0;}
        else if(pitch_ < -89.0){pitch_ = -89.0;}

        if (yaw_ > 89.0){ yaw_ = 89.0;}
        else if(yaw_ < -89.0){yaw_ = -89.0;}
    }

    // 计算相机姿态
    compute_camera();

    // 计算旋转矩阵
    view_ = glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);    
}


void Camera::update_forward(const float delta_forward, bool constrain){
     camera_pos_ -= delta_forward * camera_front_;
     view_ = glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);    
}








#endif