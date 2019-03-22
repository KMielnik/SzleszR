#version 330
in vec4 vColor;
in vec2 texCoord;
out vec4 fColor;

uniform sampler2D myTexture;

void main()
{
   fColor = texture(myTexture,texCoord);
}