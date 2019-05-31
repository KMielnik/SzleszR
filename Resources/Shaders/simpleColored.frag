#version 420
#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLightVectors[MAX_LIGHTS];
in vec3 toCameraVector;

uniform vec3 lightsColors[MAX_LIGHTS];
uniform int lightsCount;
uniform vec3 color;

out vec4 fColor;

vec4 calculateLight(vec3 toLightVector,vec3 lightColor);

void main()
{
	fColor = vec4(0,0,0,0);
	for(int i=0;i<lightsCount;i++)
		fColor += calculateLight(toLightVectors[i],lightsColors[i]); 

	fColor = min(vec4(color,1),vec4(1,1,1,1));
}

vec4 calculateLight(vec3 toLightVector,vec3 lightColor)
{
	vec3 unitSurfaceNormal = normalize(surfaceNormal);
	vec3 unitToLightVector = normalize(toLightVector);
	
	float attenuation = 1.0/ (1.0 + 0.1*pow(length(toLightVector),2));

	float dotLight = dot(unitSurfaceNormal,unitToLightVector);
	float brightness = dotLight;

	vec3 diffuse = brightness * lightColor;
	vec4 diffuseLighting = vec4(diffuse,1.0) * vec4(color,1);

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitToLightVector;

	vec3 reflectedLightDirection = reflect(lightDirection,unitSurfaceNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor,0.0);

	vec4 specularLighting = vec4(vec4(0.0,0.0,0.0,1).xyz* lightColor, 1.0);

	vec4 ambientLighting = vec4(0.2,0.2,0.2,1) * vec4(color,1);

	return ambientLighting + (attenuation * (diffuseLighting + specularLighting));
}