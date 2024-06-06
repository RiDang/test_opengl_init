#ifndef OPENGL_INIT_SHADER_H_
#define OPENGL_INIT_SHADER_H_
#include <string>
#include <unordered_map>

class Shader{
public:
    using PathMap = std::unordered_map<std::string, const std::string>;
public:
    Shader(const PathMap& path_map);
    
    std::string read_file(const std::string& path);

    void use();
    void set_bool(const std::string& name, const bool value);
    void set_int(const std::string& name, const int value);
    void set_float(const std::string& name, const float value);

public:
    unsigned int shader_program_;

};
#endif