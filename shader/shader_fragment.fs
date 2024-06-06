#version 330 core
in vec3 args_Color;
in vec3 args_Pos;
out vec4 FragColor;
uniform float ourColor;
void main()
{
   // FragColor = vec4(1.0f, ourColor, 0.2f, 1.0f);
   FragColor = vec4(args_Color, 1.0f);

}