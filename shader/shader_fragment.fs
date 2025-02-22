#version 330 core
in vec3 args_Color;
in vec3 args_Pos;
in vec2 args_Tex;

out vec4 FragColor;
uniform float ourColor;
uniform sampler2D ourTex;

void main()
{
   // FragColor = vec4(1.0f, ourColor, 0.2f, 1.0f);
   // FragColor = vec4(args_Color, 1.0f);
   FragColor = texture(ourTex, args_Tex) * vec4(args_Color, 1.0f);

}