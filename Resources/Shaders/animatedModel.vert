#version 420
layout(location = 0) in vec3 defaultPosition;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texcoords;
layout(location = 3) in vec3 cooldownPosition;
layout(location = 4) in vec3 attackPosition;


uniform mat4 modelTransformations;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;

#define MAX_LIGHTS 10

uniform vec3 lightsPosition[MAX_LIGHTS];
uniform int lightsCount;

uniform int actualAnimation;

out vec2 texCoord;
out vec3 surfaceNormal;
out vec3 toLightVectors[MAX_LIGHTS];
out vec3 toCameraVector;

void main()
{
	vec3 position = actualAnimation==1?cooldownPosition:attackPosition;
	vec4 worldPosition = modelTransformations * vec4(position,1.0);

	gl_Position = projectionMatrix * cameraMatrix * worldPosition;
	texCoord = texcoords;

	surfaceNormal = (modelTransformations * vec4(normals, 0.0)).xyz;
	for(int i=0;i<lightsCount;i++)
		toLightVectors[i] = lightsPosition[i] - worldPosition.xyz;

	toCameraVector = (inverse(cameraMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;
}