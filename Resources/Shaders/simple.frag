#version 420

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform vec3 lightColor;
uniform float shineDamper;

layout(binding=0) uniform sampler2D diffuseTexture;
layout(binding=1) uniform sampler2D specularTexture;

out vec4 fColor;

void main()
{
	vec3 unitSurfaceNormal = normalize(surfaceNormal);
	vec3 unitToLightVector = normalize(toLightVector);;

	float dotLight = dot(unitSurfaceNormal,unitToLightVector);
	float brightness = max(dotLight,0.0);

	vec4 ambientLighting = min(texture(diffuseTexture,texCoord),0.1);

	vec3 diffuse = brightness * lightColor;
	vec4 diffuseLighting = vec4(diffuse,1.0) * texture(diffuseTexture,texCoord);

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitToLightVector;

	vec3 reflectedLightDirection = reflect(lightDirection,unitSurfaceNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor,0.0);

	float dampedFactor = pow(specularFactor,shineDamper);

	vec4 specularLighting = vec4(dampedFactor *texture(specularTexture,texCoord).xyz* lightColor, 1.0);

	fColor = ambientLighting + diffuseLighting + specularLighting; 
}