#pragma once
#include "Shader.h"

class AnimatedShader : public Shader
{
public:
	AnimatedShader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights) : Shader(projectionMatrix, cameraMatrix, lights)
	{
		vertexFile = "Resources/Shaders/animatedModel.vert";
		fragmentFile = "Resources/Shaders/simpleTextured.frag";
	}

	void CompileShader() override;
	void SetAnimation(int animation);
	void bindUniformLocations() override;

	void SetVertexVBOData() override;
private:
	int actualAnimation;
	int actualAnimationLoc = 0;
};
