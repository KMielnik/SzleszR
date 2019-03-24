#version 420
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texcoords;

uniform mat4 modelTransformations;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;

out vec4 vColor;
out vec2 texCoord;

void main()
{

  gl_Position = projectionMatrix *cameraMatrix* modelTransformations * vec4(position, 1.0);
  vColor = vec4(clamp(normals,0.0,1.0), 1.0);
  texCoord = texcoords;
}