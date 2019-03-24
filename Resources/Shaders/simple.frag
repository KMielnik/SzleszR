#version 420
in vec4 vColor;
in vec2 texCoord;
out vec4 fColor;

layout(binding=0) uniform sampler2D diffuseTexture;
layout(binding=1) uniform sampler2D specularTexture;

void main()
{
   fColor = texture(diffuseTexture,texCoord);
}