#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 args_Color;
out vec2 args_Tex;

void main()
{
   args_Color = aColor;
   args_Tex = aTex;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}