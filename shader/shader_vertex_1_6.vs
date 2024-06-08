#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec3 args_Color;
out vec2 args_Tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   args_Tex = aTex;
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}